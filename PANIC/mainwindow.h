#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtNetwork/QHostAddress>
#include <QtCharts/QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class PingWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void scanNetwork();
    void showDeviceDetails(QListWidgetItem *item);
    void updateDeviceList(const QVector<QHostAddress>& newDevices);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QHostAddress> devices;
    QHash<QString, PingWorker*> pingWorkers;
};

class PingWorker : public QObject
{
    Q_OBJECT
public:
    explicit PingWorker(const QString &ip, QObject *parent = nullptr);
    ~PingWorker();
    void startPing();
    void stopPing();

signals:
    void pingResult(double latency);
    void errorOccurred(const QString &message);

private slots:
    void processOutput();

private:
    QProcess *pingProcess;
    QString targetIp;
    bool running = false;
};

class DeviceDetailsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DeviceDetailsDialog(const QString &ip, QWidget *parent = nullptr);
    ~DeviceDetailsDialog();

private slots:
    void updateGraph(double latency);

private:
    QChart *chart;
    QLineSeries *series;
    QChartView *chartView;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    PingWorker *pingWorker;
    QLabel *detailsLabel;
    QString currentIp;
    QTimer *dataTimer;

    static const int DATA_WINDOW_SECONDS = 60;
};
#endif // MAINWINDOW_H
