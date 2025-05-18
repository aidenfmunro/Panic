#pragma once

#include <QObject>
#include <QString>
#include <chrono>
#include "Ping.h"

class PingWorker : public QObject {
    Q_OBJECT

public:
    explicit PingWorker(QObject* parent = nullptr);
    virtual ~PingWorker() = default;

public slots:
    void pingHost(const QString& host);

signals:
    void pingSuccess(QString host, QString ip, double rtt_ms);
    void pingFailure(QString host, QString error);
};
