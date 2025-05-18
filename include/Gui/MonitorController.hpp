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
    void pingSuccess(QString host, QString ip, double rtt_ms);
    void pingFailure(QString host, QString error);

public slots:
    void onPingSuccess(QString host, QString ip, double rtt);
    void onPingFailure(QString host, QString error);

private:
    QTimer timer;
    QMap<QString, panic::Host> hosts;
};
