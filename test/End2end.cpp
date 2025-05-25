#include <gtest/gtest.h>

#include "Core/host.hpp"
#include "Gui/PingWorker.hpp"
#include "Net/portScanner.hpp"

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include <QString>


constexpr const char* IP = "127.0.0.1";
constexpr const char* HOST_NAME = "localhost";

TEST(End2end, test) {
    panic::Host host{};

    panic::IPv4 Address{IP};

    host.set_IPv4(Address);

    host.set_name(panic::getHostnameByIP(Address));

    EXPECT_EQ(host.get_name(), HOST_NAME);

    PingWorker worker{};

    int rtt = 0;

    worker.ping(QString::fromStdString(panic::to_string(host.get_IPv4())), rtt);

    worker.stop();

    EXPECT_EQ(rtt, 0);

    panic::PortScanner::scanPorts(host, 1, 1000);

    EXPECT_NE(host.get_ports_count(), 0);
}
