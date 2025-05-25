#pragma once

#include <QProcess>
#include <QString>

class PingWorker {
public:
    bool ping(const QString &host, int &rtt); // true if host responds, rtt in ms
    void stop();

private:
    QProcess pingProcess_;
};
