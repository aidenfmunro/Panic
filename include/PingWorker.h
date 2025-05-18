#pragma once
#include <QString>

class PingWorker {
public:
    static bool ping(const QString &host, int &rtt); // true if host responds, rtt in ms
};
