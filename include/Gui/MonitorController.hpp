#pragma once

#include "PingWorker.hpp"

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
    void addHost(const QString &host);
    void removeHost(const QString &host);
    QVector<int> getRttHistory(const QString &host) const;

signals:
    void hostChecked(const QString &host, bool alive, int rtt);

private slots:
    void checkHosts();

private:
    QTimer timer;
    QStringList hosts;
    QMap<QString, QVector<int>> rttHistory;
};
