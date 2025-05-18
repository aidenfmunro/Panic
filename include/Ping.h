#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <ostream>
#include <chrono>
#include <netinet/in.h>
#include <netdb.h>

enum class IP {
    v4,
    v6,
};

std::ostream& operator<<(std::ostream& out, IP ip);
std::string ConvertToString(int ipType, const void* address);
unsigned short CheckSum(void* b, int len);
int OpenSocket(int inetType, int socketType, int protocol);

struct IpInfo {
    std::variant<sockaddr_in, sockaddr_in6> sockaddr;
    std::string printable_addr;
    IP type;
};

class HostIpInfo {
public:
    HostIpInfo(std::string_view hostName, const addrinfo& settings);
    ~HostIpInfo();

    const std::string& getHostName() const;
    std::vector<IpInfo> getIpList() const;

private:
    std::string hostName_;
    addrinfo* ipList_;
};

class Ping {
public:
    std::chrono::duration<double, std::milli> operator()(const sockaddr_in& addr) const;
    std::chrono::duration<double, std::milli> operator()(const sockaddr_in6& addr) const;
};
