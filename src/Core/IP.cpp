#include "Core/IP.hpp"
#include <sstream>
#include <iostream>

namespace {

constexpr int OCTEN_COUNT = 4;
constexpr char IPv4_SEPARATOR = '.';

}

namespace panic {

IPv4::IPv4(const std::string &IPAdress) : intRepresentation_(0) {
    std::istringstream iss(IPAdress);
    std::string token;
    u_int8_t *octens = reinterpret_cast<u_int8_t*>(&intRepresentation_);
    for(size_t token_index = 0; token_index < OCTEN_COUNT, std::getline(iss, token, IPv4_SEPARATOR);) {
        if (!token.empty()) {
            octens[OCTEN_COUNT - 1 - token_index] = static_cast<u_int8_t>(std::stoi(token));
            token_index++;
        }
    }
}

u_int8_t IPv4::operator[] (int num) const {
    if (num >= OCTEN_COUNT) {
        throw std::out_of_range{"IP address has only 4 parts"};
    }

    return reinterpret_cast<const u_int8_t*>(&intRepresentation_)[OCTEN_COUNT - 1 - num];
}

std::ostream& operator<< (std::ostream& os, const IPv4 &IP) {
    for (int i = 0; i < OCTEN_COUNT - 1; i++) {
        os << +IP[i] << '.';
    }
    os << +IP[OCTEN_COUNT - 1];

    return os;
}

std::string to_string(const IPv4 &IP) {
    std::stringstream ostream_out;
    ostream_out << IP;
    
    return ostream_out.str();
}

} // namespace panic