#include "gtest/gtest.h"
#include "Core/host.hpp"

TEST(HostTest, GetNameTest) {
    panic::Host host{};

    EXPECT_EQ(host.get_name(), "noname");
}

TEST(HostTest, GetLocalHostName) {
    panic::Host host{};

    host.set_IPv4(panic::IPv4{"127.0.0.1"});

    host.set_name(panic::getHostnameByIP(panic::IPv4{"127.0.0.1"}));

    EXPECT_EQ(host.get_name(), "localhost");
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

// TODO make port value const and don't changable
TEST(HostTest, PortChangeTest) {
    panic::Host host{};

    host.add_port(1);
    panic::port &port = host.find_port(1)->second;
    port.set_value(2);

    EXPECT_EQ(host.find_port(1)->second, 2);
}   

TEST(HostTest, ResponseTimeTest) {
    panic::Host host{};

    host.push_back_response_time(12);

    EXPECT_EQ(host.get_last_response_time(), 12);
}

TEST(HostTest, ResponseTimeRangeTest) {
    panic::Host host{};

    time_t time_array[] = {1, 2, 3, 4, 5};

    for (time_t time : time_array) {
        host.push_back_response_time(time);
    }

    auto time_begin = host.response_time_begin();
    for (struct {decltype(time_begin) time; int index;} s = {time_begin, 0}; 
         s.time != host.response_time_end(); s.time++, s.index++) {
        EXPECT_EQ(*s.time, time_array[s.index]);
    }
}

TEST (HostTest, AppendResponseTimeVectorTest) {
    panic::Host host{};

    std::vector<time_t> time_vector{1, 2, 3, 4, 5};

    host.append_response_time_vector(time_vector);

    auto inside_time_begin = host.response_time_begin();
    auto outside_time_begin = time_vector.begin();
    for (struct {decltype(inside_time_begin) inside; decltype(outside_time_begin) outside;} 
         s = {inside_time_begin, outside_time_begin}; s.inside != host.response_time_end();
         s.inside++, s.outside++) {
        EXPECT_EQ(*s.inside, *s.outside);
    }
}

TEST(HostTest, ChangeResponseTimeTest) {
    panic::Host host{};

    host.push_back_response_time(1);
    
    auto &time = host.get_last_response_time();
    time = 2;

    EXPECT_EQ(host.get_last_response_time(), 2);
}