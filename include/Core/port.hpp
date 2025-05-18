// This file describe port class
#pragma once
#include <compare>

namespace panic {

class port
{
private:
    long port_;
public:
    port() : port_(0) {}
    explicit port(long port) : port_(port) {}

    // Getter
    long get_value() const {return port_;}

    // Setter
    void set_value(long port) {port_ = port;}

    bool operator== (const port &other) const {
        return port_ == other.port_;
    }
    bool operator== (long other) const {
        return port_ == other;
    }
    auto operator<=> (const port &other) const {
        return port_ - other.port_;
    }
    auto operator<=> (long other) const {
        return port_ - other;
    }
};

} // namespace panic
