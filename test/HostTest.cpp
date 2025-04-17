#include "gtest/gtest.h"
#include "Core/host.hpp"

TEST(HostTest, GetNameTest) {
    panic::Host host{};

    EXPECT_EQ(host.get_name(), "noname");
}

TEST(HostTest, AddPortTest) {
    panic::Host host{};

    panic::port port(2);

    host.add_port(1);
    host.add_port(port);

    EXPECT_EQ(host.find_port(1)->second, 1);
    EXPECT_EQ(host.find_port(2)->second, 2);
    EXPECT_EQ(host.find_port(3), host.ports_end());
}

TEST(HostTest, PortEraseTest) {
    panic::Host host{};

    host.add_port(1);

    EXPECT_EQ(host.erase_port(1), 1);
    EXPECT_EQ(host.find_port(1), host.ports_end());
}