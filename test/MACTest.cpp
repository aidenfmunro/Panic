#include "gtest/gtest.h"
#include "Core/MAC.hpp"

constexpr u_int64_t TEST_MAC = 0xaabbccddeeff;
constexpr const char* TEST_MAC_STRING = "aa:bb:cc:dd:ee:ff";

TEST(MACTest, ToStringTest) {
    panic::MAC_48 MAC{TEST_MAC};

    EXPECT_EQ(panic::to_string(MAC), TEST_MAC_STRING);
}

TEST(MACTest, StringConstructorTest) {
    panic::MAC_48 MAC{TEST_MAC_STRING};

    EXPECT_EQ(panic::to_string(MAC), TEST_MAC_STRING);
}