#include "Core/IP.hpp"
#include <sstream>

namespace panic {

constexpr int OCTEN_COUNT = 4;

u_int8_t IPv4::operator[] (int num) const {
    if (num >= OCTEN_COUNT) {
        throw std::out_of_range{"IP address has only 4 parts"};
    }

    return reinterpret_cast<const u_int8_t*>(&intRepresentation_)[OCTEN_COUNT - 1 - num];
}

std::ostream& operator<< (std::ostream& os, const IPv4 &IP) {
    for (int i = 0; i < OCTEN_COUNT - 1; i++) {
        os << static_cast<int>(IP[i]) << '.';
    }
    os << static_cast<int>(IP[OCTEN_COUNT - 1]);

    return os;
}

std::string to_string(const IPv4 &IP) {
    std::stringstream ostream_out;
    ostream_out << IP;
    
    return ostream_out.str();
}

} // namespace panic