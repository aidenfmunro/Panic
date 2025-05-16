#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <QtNetwork/QNetworkInterface>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanNetwork();
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::scanNetwork);
    connect(ui->deviceList, &QListWidget::itemClicked, this, &MainWindow::showDeviceDetails);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::scanNetwork()
{
    QVector<QHostAddress> newDevices;

    // Get all local IPs
    const auto addresses = QNetworkInterface::allAddresses();
    for (const auto &address : addresses)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress::LocalHost)
        {
            newDevices.append(address);
        }
    }

    // Check ARP cache
    QProcess arp;
    arp.start("arp", QStringList() << "-a");
    arp.waitForFinished();
    QString output = arp.readAllStandardOutput();

    QRegularExpression ipRegex(R"((\d+\.\d+\.\d+\.\d+))");
    auto matches = ipRegex.globalMatch(output);

    while (matches.hasNext())
    {
        QHostAddress address(matches.next().captured(1));
        if (!newDevices.contains(address))
        {
            newDevices.append(address);
        }
    }

    updateDeviceList(newDevices);
}

void MainWindow::showDeviceDetails(QListWidgetItem *item)
{
    QString ip = item->text();
    DeviceDetailsDialog *dialog = new DeviceDetailsDialog(ip, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::updateDeviceList(const QVector<QHostAddress>& newDevices)
{
    ui->deviceList->clear();
    devices = newDevices;

    // Add new devices with status indicators
    for (const auto& device : devices)
    {
        QListWidgetItem* item = new QListWidgetItem(device.toString(), ui->deviceList);

        // Set icon based on responsiveness
        QProcess ping;
        ping.start("ping", QStringList() << "-c" << "1" << "-W" << "1" << device.toString());
        ping.waitForFinished();

        if (ping.exitCode() == 0)
        {
            item->setIcon(QIcon(":/icons/online.png"));
            item->setData(Qt::UserRole, "online");
        }
        else
        {
            item->setIcon(QIcon(":/icons/offline.png"));
            item->setData(Qt::UserRole, "offline");
        }
    }

    ui->deviceList->sortItems();
}

PingWorker::PingWorker(const QString &ip, QObject *parent)
    : QObject(parent), targetIp(ip)
{
    pingProcess = new QProcess(this);
    connect(pingProcess, &QProcess::readyReadStandardOutput, this, &PingWorker::processOutput);
}

void PingWorker::startPing()
{
    if (running) return;
    running = true;

#ifdef Q_OS_WINDOWS
    pingProcess->start("ping", QStringList() << "-t" << targetIp);
#else
    pingProcess->start("ping", QStringList() << "-i" << "1" << targetIp);
#endif
}

void PingWorker::processOutput()
{
    QString output = pingProcess->readAllStandardOutput();
    QRegularExpression timeRegex("time=([\\d.]+)");
    QRegularExpressionMatch match = timeRegex.match(output);

    if (match.hasMatch()) {
        double latency = match.captured(1).toDouble();
        emit pingResult(latency);
    }
}

void PingWorker::stopPing()
{
    pingProcess->terminate();
    running = false;
}

PingWorker::~PingWorker() { stopPing(); }

DeviceDetailsDialog::DeviceDetailsDialog(const QString &ip, QWidget *parent)
    : QDialog(parent), currentIp(ip)
{
    // Setup window
    setWindowTitle("Device Details: " + ip);
    resize(800, 600);

    // Create chart
    chart = new QChart();
    chart->setTitle("Ping Latency (ms)");
    chart->setAnimationOptions(QChart::NoAnimation);

    // Create and customize series
    series = new QLineSeries();
    QPen pen(QColor(65, 105, 225));  // Royal blue color
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    series->setPen(pen);

    // Add markers
    series->setPointsVisible(true);
    series->setPointLabelsVisible(true);
    series->setPointLabelsFormat("@yPoint ms");
    series->setPointLabelsClipping(false);

    // Setup axes
    axisX = new QDateTimeAxis();
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    axisX->setGridLineVisible(true);
    axisX->setMinorGridLineVisible(true);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%.1f ms");
    axisY->setTitleText("Latency");
    axisY->setGridLineVisible(true);

    // Add everything to chart
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Visual enhancements
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setPlotAreaBackgroundVisible(true);
    chart->setPlotAreaBackgroundBrush(QBrush(Qt::lightGray));

    // Create chart view
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Setup layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);

    // Start pinging
    pingWorker = new PingWorker(ip);
    connect(pingWorker, &PingWorker::pingResult, this, &DeviceDetailsDialog::updateGraph);
    pingWorker->startPing();

    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, [this]() {
        // Remove old points to maintain 60-second window
        qint64 cutoff = QDateTime::currentMSecsSinceEpoch() - (DATA_WINDOW_SECONDS * 1000);

        while (series->count() > 0 &&
               series->at(0).x() < cutoff) {
            series->remove(0);
        }
    });
    dataTimer->start(1000);
}

void DeviceDetailsDialog::updateGraph(double latency)
{
    QDateTime now = QDateTime::currentDateTime();
    qint64 timestamp = now.toMSecsSinceEpoch();

    series->append(timestamp, latency);

    QDateTime rangeStart = QDateTime::fromMSecsSinceEpoch(timestamp - (DATA_WINDOW_SECONDS * 1000));
    axisX->setRange(rangeStart, now);

    // Adjust Y-axis range dynamically based on latest latency
    double currentMax = axisY->max();
    double newMax = qMax(100.0, latency * 1.1);
    if (newMax > currentMax || latency < axisY->min()) {
        axisY->setRange(0, newMax);
    }
}

DeviceDetailsDialog::~DeviceDetailsDialog()
{
    pingWorker->stopPing();
    delete pingWorker;
}
