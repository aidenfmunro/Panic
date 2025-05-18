#include "ping.h"

/*
    usage: sudo ./ping_demo google.com
*/

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Enter hostname\n";
        return 1;
    }
    
    try {
        HostIpInfo hostIpInfo(argv[1], { .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM });

        for (const auto& ipInfo : hostIpInfo.getIpList()) {
            std::cout << "ping for " << ipInfo.type << ' ' << ipInfo.printable_addr << ":\n";
            std::cout << std::visit(Ping{}, ipInfo.sockaddr).count() << "ms\n";
        }
    } catch (std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }

    return 0;
}
