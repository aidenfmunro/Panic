#pragma once

#include <QAbstractListModel>
#include <vector>
#include <QString>
#include <QDate>
#include <optional>
#include "Ping.h"

struct HostStatus {
    HostIpInfo hostInfo;
    std::optional<IpInfo> lastReachableIp;
    bool reachable = false;
    int pingMs = -1;
    QString lastError;
    QDateTime lastChecked;
};

class HostListModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
            HostNameRole = Qt::UserRole + 1,
            StatusRole,
            PingRole,
            LastIpRole,
            ErrorRole,
            LastCheckedRole
        };

    explicit HostListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool containsHost(const QString& hostName) const;
    void addHost(const HostIpInfo& hostInfo);
    void removeHost(int row);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    HostIpInfo hostAt(int row) const;

    void updateHostStatus(int row, bool reachable, int pingMs, const IpInfo& ip);

signals:
    void hostStatusChanged(int row);

private:
    std::vector<HostStatus> hosts_;
};
