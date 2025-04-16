#include "gtest/gtest.h"
#include <Core/IP.hpp>

#define TEST_IP 2864434397  // 0xaabbccdd

TEST(IPTest, GetValueTest) {
    panic::IPv4 IP{TEST_IP};

    EXPECT_EQ(IP.get_value(), TEST_IP);
}

TEST(IPTest, SquareBracketsTest) {
    panic::IPv4 IP{TEST_IP};

    EXPECT_EQ(IP[0], 0xaa);
    EXPECT_EQ(IP[1], 0xbb);
    EXPECT_EQ(IP[2], 0xcc);
    EXPECT_EQ(IP[3], 0xdd);
}

TEST(IPTest, SquareBracketsTestException) {
    panic::IPv4 IP{TEST_IP};

    EXPECT_THROW(IP[4], std::out_of_range);
}

TEST(IPTest, ToStringTest) {
    panic::IPv4 IP{TEST_IP};

    EXPECT_EQ(panic::to_string(IP), "170.187.204.221");
}