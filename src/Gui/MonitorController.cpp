#include "Gui/MonitorController.hpp"
#include <QtConcurrent/QtConcurrent>

class PingTask : public QRunnable {
public:
    PingTask(const QString& host, MonitorController* controller)
        : host_(host), controller_(controller) {}

void MonitorController::addHost(const QString &hostName) {

    if (!hosts.contains(hostName))
    {
        panic::IPv4 ip(hostName.toStdString());
        panic::Host host{};
        host.set_IPv4(ip);
        hosts.insert(hostName, std::move(host));
    }
}

void MonitorController::removeHost(const QString &hostName) {
    hosts.remove(hostName);
}

panic::Host* MonitorController::getHost(const QString &hostName) const {
    auto it = hosts.find(hostName);
    return it != hosts.end() ? const_cast<panic::Host*>(&it.value()) : nullptr;
}


void MonitorController::checkHosts() {
    for (auto it = hosts.begin(); it != hosts.end(); ++it) {
        QString hostName = it.key();
        panic::Host &host = it.value();

        QThreadPool::globalInstance()->start([this, host, hostName]() {
            int rtt;
            bool ok = PingWorker::ping(hostName, rtt);
            QMetaObject::invokeMethod(this, [this, host, hostName, ok, rtt] {
                auto it = hosts.find(hostName);
                if (it != hosts.end()) {
                    panic::Host &host = it.value();
                    if (ok) {
                        host.set_status(panic::ONLINE);
                        host.push_back_response_time(rtt);
                    } else {
                        host.set_status(panic::OFFLINE);
                    }
                }
                emit hostChecked(hostName, ok, rtt);
            }, Qt::QueuedConnection);
        });
    }

private:
    QString host_;
    MonitorController* controller_;
};

MonitorController::MonitorController(QObject* parent) : QObject(parent) {
    pool_.setMaxThreadCount(4); // TODO: config 
}

QVector<int> MonitorController::getRttHistory(const QString &hostName) const {
    QVector<int> result;
    auto it = hosts.find(hostName);
    if (it != hosts.end()) {
        const panic::Host &host = it.value();
        for (auto it = host.response_time_begin(); it != host.response_time_end(); ++it) {
            result.append(static_cast<int>(*it));  // assuming time_t fits into int
        }
    }
    return result;
}
