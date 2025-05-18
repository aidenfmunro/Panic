#include "Gui/PingWorker.hpp"
#include <QRegularExpression>
#include <QProcess>

PingWorker::PingWorker(QObject* parent)
    : QObject(parent) {}

void PingWorker::pingHost(const QString& host) {
    try {
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC; // Both IPv4 and IPv6
        hints.ai_socktype = SOCK_RAW;

        HostIpInfo resolver(host.toStdString(), hints);
        auto ipList = resolver.getIpList();
        Ping ping;

        for (const auto& ip : ipList) {
            try {
                std::chrono::duration<double, std::milli> rtt;

                if (ip.type == IP::v4) {
                    const sockaddr_in& addr = std::get<sockaddr_in>(ip.sockaddr);
                    rtt = ping(addr);
                } else {
                    const sockaddr_in6& addr6 = std::get<sockaddr_in6>(ip.sockaddr);
                    rtt = ping(addr6);
                }

                emit pingSuccess(host, QString::fromStdString(ip.printable_addr), rtt.count());
            } catch (const std::exception& e) {
                emit pingFailure(host, QString::fromStdString(ip.printable_addr) + " → " + e.what());
            }
        }
    } catch (const std::exception& e) {
        emit pingFailure(host, e.what());
    }
}

