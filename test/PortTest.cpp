#include "gtest/gtest.h"

#include "Core/port.hpp"

TEST(PortTest, SpaceShipTest) {
    panic::port port(1);

    EXPECT_EQ(port == 1, true);
    EXPECT_EQ(port > 0, true);
    EXPECT_EQ(port > 2, false);
}