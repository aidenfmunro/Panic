#pragma once
#include <string>
#include <stdexcept>

namespace panic {

class MAC_48 {
private:
    u_int64_t intRepresentation_;
public:
    // Construct MAC_48 class from u_int64_t
    MAC_48(u_int64_t MACAddress) : intRepresentation_(MACAddress) {}
    MAC_48(const std::string &MACAddress);

    // Get value of MAC_48 in u_int64_t representation
    u_int32_t get_value() const {return intRepresentation_;}

    // Access to MAC_48 octens by the following scheme:
    // 000:123:323:231:468:343 
    // [0].[1].[2].[3].[4].[5]
    u_int8_t operator[] (int num) const;

    friend std::ostream& operator<< (std::ostream& os, const MAC_48 &IP);
};

std::string to_string(const MAC_48 &MAC);

} // namespace panic

