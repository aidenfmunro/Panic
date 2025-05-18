#pragma once

#include "PingWorker.hpp"
#include "Core/host.hpp"

#include <QObject>
#include <QTimer>
#include <QString>
#include <QMap>
#include <QVector>
#include <QStringList>

class MonitorController : public QObject {
    Q_OBJECT

public:
    explicit MonitorController(QObject *parent = nullptr);
    void addHost(const QString &hostName);
    void removeHost(const QString &hostName);
    panic::Host* getHost(const QString &hostName) const;
    QVector<int> getRttHistory(const QString &hostName) const;

signals:
    void hostChecked(const QString &host, bool alive, int rtt);

private slots:
    void checkHosts();

private:
    QTimer timer;
    QMap<QString, panic::Host> hosts;
};
