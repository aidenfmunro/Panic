#include <iostream>
#include "Core/IP.hpp"

int main() {
    panic::IPv4 hhh(917371938);
    std::cout << "Hi\n" << std::endl;

    std::cout << "IP: " << hhh << std::endl;

    panic::IPv4 IP{"12.2.1.1.3.2"};

    std::cout << IP << std::endl;

    return 0;
}