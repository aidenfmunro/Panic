#include "Gui/MonitorController.hpp"
#include <QtConcurrent/QtConcurrent>

MonitorController::MonitorController(QObject *parent) : QObject(parent) {
    connect(&timer, &QTimer::timeout, this, &MonitorController::checkHosts);
    timer.start(3000);
}

void MonitorController::addHost(const QString &host) {
    if (!hosts.contains(host))
        hosts.append(host);
}

void MonitorController::removeHost(const QString &host) {
    hosts.removeAll(host);            // ✅ Remove host from list
    rttHistory.remove(host);          // ✅ Clear stored RTT history
}

void MonitorController::checkHosts() {
    for (const auto &host : hosts) {
        QThreadPool::globalInstance()->start([this, host]() {
            int rtt;
            bool ok = PingWorker::ping(host, rtt);
            QMetaObject::invokeMethod(this, [this, host, ok, rtt] {
                if (ok) {
                    rttHistory[host].append(rtt);
                }
                emit hostChecked(host, ok, rtt);
            }, Qt::QueuedConnection);
        });
    }
}

QVector<int> MonitorController::getRttHistory(const QString &host) const {
    return rttHistory.value(host);
}
