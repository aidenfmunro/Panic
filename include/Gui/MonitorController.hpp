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
