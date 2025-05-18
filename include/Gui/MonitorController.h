#pragma once

#include <QObject>
#include <QThreadPool>
#include "PingWorker.h"
#include "HostListModel.h"

class MonitorController : public QObject {
    Q_OBJECT

public:
    explicit MonitorController(QObject* parent = nullptr);

    void startMonitoring(const QString& host);

signals:
    void pingSuccess(QString host, QString ip, double rtt_ms);
    void pingFailure(QString host, QString error);

public slots:
    void onPingSuccess(QString host, QString ip, double rtt);
    void onPingFailure(QString host, QString error);

private:
    QThreadPool pool_;
};
