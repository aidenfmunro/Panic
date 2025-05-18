#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>

#include "Core/host.hpp"

namespace panic {

std::string getHostnameByIP(const IPv4& ipAddress) {
    struct sockaddr_in sa;
    char hostname[NI_MAXHOST];
    
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, to_string(ipAddress).c_str(), &sa.sin_addr);
    
    int res = getnameinfo((struct sockaddr*)&sa, sizeof(sa), 
                      hostname, NI_MAXHOST, 
                      nullptr, 0, NI_NAMEREQD);
    
    if (res != 0) {
        throw std::system_error(res, std::system_category(), 
                              "Failed to resolve hostname");
    }
    
    return std::string(hostname);
}

} // namespace panic