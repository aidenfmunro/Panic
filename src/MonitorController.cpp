#include "MonitorController.h"
#include <QRunnable>

class PingTask : public QRunnable {
public:
    PingTask(const QString& host, MonitorController* controller)
        : host_(host), controller_(controller) {}

    void run() override {
        PingWorker worker;
        QObject::connect(&worker, &PingWorker::pingSuccess,
                         controller_, &MonitorController::onPingSuccess);
        QObject::connect(&worker, &PingWorker::pingFailure,
                         controller_, &MonitorController::onPingFailure);

        worker.pingHost(host_);
    }

private:
    QString host_;
    MonitorController* controller_;
};

MonitorController::MonitorController(QObject* parent) : QObject(parent) {
    pool_.setMaxThreadCount(4); // TODO: config 
}

void MonitorController::startMonitoring(const QString& host) {
    auto* task = new PingTask(host, this);
    task->setAutoDelete(true);
    pool_.start(task);
}

void MonitorController::onPingSuccess(QString host, QString ip, double rtt) {
    emit pingSuccess(host, ip, rtt);
}

void MonitorController::onPingFailure(QString host, QString error) {
    emit pingFailure(host, error);
}
