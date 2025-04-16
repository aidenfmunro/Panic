#include "Core/MAC.hpp"
#include <sstream>

namespace panic {
constexpr int OCTEN_COUNT = 6;

u_int8_t MAC_48::operator[] (int num) const {
    if (num >= OCTEN_COUNT) {
        throw std::out_of_range{"MAC-48 address has only 6 parts"};
    }

    return reinterpret_cast<const u_int8_t*>(&intRepresentation_)[OCTEN_COUNT - 1 - num];
}

std::ostream& operator<< (std::ostream& os, const MAC_48 &IP) {
    for (int i = 0; i < OCTEN_COUNT - 1; i++) {
        os << std::hex << static_cast<int>(IP[i]) << ':';
    }
    os << std::hex << static_cast<int>(IP[OCTEN_COUNT - 1]);

    return os;
}

std::string to_string(const MAC_48 &IP) {
    std::stringstream ostream_out;
    ostream_out << IP;
    
    return ostream_out.str();
}
} // namespace panic
