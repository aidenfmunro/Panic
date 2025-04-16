#include "Core/IP.hpp"

namespace panic {

u_int8_t IPv4::operator[] (int num) const {
    if (num >= OCTEN_COUNT) {
        throw std::out_of_range{"IP address has only 4 parts"};
    }

    return reinterpret_cast<const u_int8_t*>(&intRepresentation_)[OCTEN_COUNT - 1 - num];
}

std::ostream& operator<< (std::ostream& os, const IPv4 &IP) {
    return os << to_string(IP);
}

std::string to_string(const IPv4 &IP) {
    std::string out;

    for (int i = 0; i < OCTEN_COUNT - 1; i++) {
        out.append(std::to_string(IP[i]));
        out.append(1, '.');
    }

    out.append(std::to_string(IP[OCTEN_COUNT - 1]));

    return out;
}

} // namespace panic