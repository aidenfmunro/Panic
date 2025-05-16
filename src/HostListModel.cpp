#include "HostListModel.h"

HostListModel::HostListModel(QObject* parent)
    : QAbstractListModel(parent) {}

int HostListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(hosts_.size());
}

QVariant HostListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= (int)hosts_.size())
        return QVariant();

    const auto& hostStatus = hosts_[index.row()];

    switch (role) {
        case Qt::DisplayRole:
        case HostNameRole:
            return QString::fromStdString(hostStatus.hostInfo.getHostName());
        case StatusRole:
            return hostStatus.reachable;
        case PingRole:
            return hostStatus.pingMs;
        case LastIpRole:
            if (hostStatus.lastReachableIp)
                return QString::fromStdString(hostStatus.lastReachableIp->printable_addr);
            return QString();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> HostListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[HostNameRole] = "hostName";
    roles[StatusRole] = "reachable";
    roles[PingRole] = "pingMs";
    roles[LastIpRole] = "lastIp";
    return roles;
}

bool HostListModel::containsHost(const QString& hostName) const {
    for (const auto& host : hosts_) {
        if (QString::fromStdString(host.hostInfo.getHostName()) == hostName)
            return true;
    }
    return false;
}

void HostListModel::addHost(const HostIpInfo& hostInfo) {
    if (containsHost(QString::fromStdString(hostInfo.getHostName())))
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    hosts_.push_back(HostStatus{hostInfo, std::nullopt, false, -1});
    endInsertRows();
}

void HostListModel::removeHost(int row) {
    if (row < 0 || row >= (int)hosts_.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    hosts_.erase(hosts_.begin() + row);
    endRemoveRows();
}

HostIpInfo HostListModel::hostAt(int row) const {
    if (row < 0 || row >= (int)hosts_.size())
        throw std::out_of_range("Invalid row");
    return hosts_[row].hostInfo;
}

void HostListModel::updateHostStatus(int row, bool reachable, int pingMs, const IpInfo& ip) {
    if (row < 0 || row >= (int)hosts_.size())
        return;

    auto& host = hosts_[row];
    host.reachable = reachable;
    host.pingMs = pingMs;
    host.lastReachableIp = ip;

    QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {StatusRole, PingRole, LastIpRole});
}
