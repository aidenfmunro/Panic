#include "Core/MAC.hpp"
#include <sstream>

namespace {

constexpr int OCTEN_COUNT = 6;
constexpr char MAC_48_SEPARATOR = ':';

}

namespace panic {

MAC_48::MAC_48(const std::string &MACAddress) : intRepresentation_(0) {
    std::istringstream iss(MACAddress);
    std::string token;
    u_int8_t *octens = reinterpret_cast<u_int8_t*>(&intRepresentation_);
    for(size_t token_index = 0; token_index < OCTEN_COUNT, std::getline(iss, token, MAC_48_SEPARATOR);) {
        if (!token.empty()) {
            octens[OCTEN_COUNT - 1 - token_index] = static_cast<u_int8_t>(std::stoi(token, nullptr, 16));
            token_index++;
        }
    }
}

u_int8_t MAC_48::operator[] (int num) const {
    if (num >= OCTEN_COUNT) {
        throw std::out_of_range{"MAC-48 address has only 6 parts"};
    }

    return reinterpret_cast<const u_int8_t*>(&intRepresentation_)[OCTEN_COUNT - 1 - num];
}

std::ostream& operator<< (std::ostream& os, const MAC_48 &IP) {
    for (int i = 0; i < OCTEN_COUNT - 1; i++) {
        os << std::hex << +IP[i] << ':';
    }
    os << std::hex << +IP[OCTEN_COUNT - 1];

    return os;
}

std::string to_string(const MAC_48 &IP) {
    std::stringstream ostream_out;
    ostream_out << IP;
    
    return ostream_out.str();
}
} // namespace panic
