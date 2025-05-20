#include "Gui/MainWindow.hpp"
#include "Core/host.hpp"
#include "Net/portScanner.hpp"

#include <QComboBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      table(new QTableWidget),
      controller(new MonitorController(this))
{
    auto *central = new QWidget;
    auto *layout = new QVBoxLayout(central);

    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Host", "IP", "Status", "RTT", "Port", "Actions"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    addHostButton = new QPushButton("Add Host");
    removeHostButton = new QPushButton("Remove Host");

    layout->addWidget(table);
    layout->addWidget(addHostButton);
    layout->addWidget(removeHostButton);
    setCentralWidget(central);

    connect(addHostButton, &QPushButton::clicked, this, &MainWindow::onAddHost);
    connect(removeHostButton, &QPushButton::clicked, this, &MainWindow::onRemoveHost);
    connect(controller, &MonitorController::hostChecked, this, &MainWindow::onHostChecked);
    connect(table, &QTableWidget::cellClicked, this, &MainWindow::onTableCellClicked);
    connect(table->selectionModel(), &QItemSelectionModel::selectionChanged,
        this, [this](const QItemSelection &, const QItemSelection &) {
            table->clearSelection();
        });

}

MainWindow::~MainWindow() {
    qDeleteAll(openCharts);
    openCharts.clear();
}

void MainWindow::onAddHost() {
    bool ok;
    QString hostIP = QInputDialog::getText(this, "Add Host", "Host IP:", QLineEdit::Normal, "", &ok);
    try {
        controller->addHost(hostIP);
    } catch (const std::system_error &e) {
        QMessageBox::critical(this, "Hostname Resolution Failed",
            QString("Could not resolve hostname"));
    }
}

void MainWindow::onRemoveHost() {
    int row = table->currentRow();
    if (row < 0)
        return;

    QString host = table->item(row, 1)->text();

    controller->removeHost(host);

    table->removeRow(row);

    if (openCharts.contains(host))
    {
        openCharts[host]->close();
        delete openCharts[host];
        openCharts.remove(host);
    }
}


void MainWindow::updateResult(const QString &hostIP, bool alive, int rtt) {
    for (int i = 0; i < table->rowCount(); ++i) {
        if (table->item(i, MainWindow::Position::IP)->text() == hostIP) {
            table->item(i, MainWindow::Position::Status)->setText(alive ? "🟢" : "🔴");
            table->item(i, MainWindow::Position::RTT)->setText(alive ? QString::number(rtt) : "");
            return;
        }
    }

    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, MainWindow::Position::Host, new QTableWidgetItem(QString::fromStdString(controller->getHost(hostIP)->get_name())));
    table->setItem(row, MainWindow::Position::IP, new QTableWidgetItem(hostIP));
    table->setItem(row, MainWindow::Position::Status, new QTableWidgetItem(alive ? "🟢" : "🔴"));
    table->setItem(row, MainWindow::Position::RTT, new QTableWidgetItem(QString::number(rtt)));

    QComboBox *portCombo = new QComboBox();
    table->setCellWidget(row, MainWindow::Position::Port, portCombo);

    QPushButton *scanButton = new QPushButton("Scan Ports");
    table->setCellWidget(row, MainWindow::Position::Actions, scanButton);

    connect(scanButton, &QPushButton::clicked, this, [this, hostIP, portCombo]() {
        int startPort = QInputDialog::getInt(this, "Start Port", "Enter start port:", 20, 1, 65535);
        int endPort = QInputDialog::getInt(this, "End Port", "Enter end port:", 100, startPort, 65535);

        panic::Host* host = controller->getHost(hostIP);

        panic::PortScanner::scanPorts(*host, startPort, endPort);

        portCombo->clear();
        for (auto it = host->ports_begin(); it != host->ports_end(); ++it) {
            portCombo->addItem(QString::number(it->first));
        }
    });
}

void MainWindow::onTableCellClicked(int row, int column) {
    switch (column) {
        case MainWindow::Position::Host:
        {
            QString hostname = table->item(row, column)->text();
            QGuiApplication::clipboard()->setText(hostname);
            QMessageBox::information(this, "Copied", "Hostname copied to clipboard!");

            table->clearSelection();

            return;
        }
        case MainWindow::Position::RTT:
        {
            QString host = table->item(row, 1)->text();
            const auto &history = controller->getRttHistory(host);
            if (history.isEmpty()) {
                table->clearSelection();
                return;
            }

            if (openCharts.contains(host)) {
                openCharts[host]->show();
                openCharts[host]->raise();
                openCharts[host]->activateWindow();

                table->clearSelection();  
                return;
            }

            auto *chart = new ChartWindow(host, history);
            openCharts.insert(host, chart);

            connect(chart, &QWidget::destroyed, this, [this, host]() {
                openCharts.remove(host);
            });

            chart->show();

            table->clearSelection();

            return;
        }
    }
}

void MainWindow::onHostChecked(const QString &host, bool alive, int rtt) {
    updateResult(host, alive, rtt);

    if (alive && openCharts.contains(host)) {
        openCharts[host]->appendRtt(rtt);
    }
}
