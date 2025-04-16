#include "gtest/gtest.h"
#include "Core/host.hpp"

TEST(HostTest, GetNameTest) {
    panic::Host host{};

    EXPECT_EQ(host.get_name(), "noname");
}