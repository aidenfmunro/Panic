#include <iostream>

#include "Net/portScanner.hpp"

int main() {
    std::string ipStr;
    int startPort, endPort;

    std::cout << "Enter IP address: ";
    std::cin >> ipStr;
    std::cout << "Enter start port: ";
    std::cin >> startPort;
    std::cout << "Enter end port: ";
    std::cin >> endPort;

    try {
        panic::IPv4 ip(ipStr);
        panic::Host scanResult{};
        scanResult.set_IPv4(ip);
        panic::PortScanner::scanPorts(scanResult, startPort, endPort);

        std::cout << "\nScan results for " << to_string(ip) << ":\n";
        std::cout << "Status: " << (scanResult.getStatus() == panic::ONLINE ? "Online" : "Offline") << "\n";
        std::cout << "Open ports (" << scanResult.get_ports_count() << "):\n";

        for (auto it = scanResult.ports_begin(); it != scanResult.ports_end(); ++it) {
            std::cout << "  Port " << it->first << " is open\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}