// File describe class of IP address
#pragma once
#include <string>
#include <stdexcept>

namespace panic {

constexpr int OCTEN_COUNT = 4;

class IPv4
{
private:
    u_int32_t intRepresentation_;   // 32-bit unsigned int
public:
    // Construct IPv4 class from u_int32_t
    explicit IPv4(u_int32_t IPAddress) : intRepresentation_(IPAddress) {}

    // Get value of IPv4 in u_int32_t representation
    u_int32_t get_value() const {return intRepresentation_;}

    // Access to IPv4 octens by the following scheme:
    // 192.168.000.001 
    // [0].[1].[2].[3]
    u_int8_t operator[] (int num) const;

    friend std::ostream& operator<< (std::ostream& os, const IPv4 &IP);
};

std::string to_string(const IPv4 &IP);

} // namespace panic
