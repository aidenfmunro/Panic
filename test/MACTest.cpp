#include "gtest/gtest.h"
#include "Core/MAC.hpp"

#define TEST_MAC 0xaabbccddeeff

TEST(MACTest, ToStringTest) {
    panic::MAC_48 MAC{TEST_MAC};

    EXPECT_EQ(panic::to_string(MAC), "aa:bb:cc:dd:ee:ff");
}