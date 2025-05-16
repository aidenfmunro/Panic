#include "Ping.h"

#include <cstring>
#include <unistd.h>
#include <format>
#include <iostream>
#include <stdexcept>
#include <array>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/icmp6.h>
#include <sys/time.h>

constexpr size_t kPacketSize = 64;
constexpr size_t kDefaultBufSize = 1024;

std::ostream& operator<<(std::ostream& out, IP ip) {
    return (out << (ip == IP::v4 ? "Ipv4" : "Ipv6"));
}

std::string ConvertToString(int ipType, const void* address) {
    char result[INET6_ADDRSTRLEN];
    inet_ntop(ipType, address, result, sizeof(result));
    return std::string(result);
}

unsigned short CheckSum(void* b, int len) {
    unsigned short* buf = static_cast<unsigned short*>(b);
    unsigned int sum = 0;
    for (; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *reinterpret_cast<unsigned char*>(buf);
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

int OpenSocket(int inetType, int socketType, int protocol) {
    int sockfd = socket(inetType, socketType, protocol);
    if (sockfd < 0) {
        throw std::runtime_error(std::format("Unable to open socket. socket() return is {}", sockfd));
    }
    return sockfd;
}

HostIpInfo::HostIpInfo(std::string_view hostName, const addrinfo& settings)
    : hostName_(hostName) {
    if (const auto status = getaddrinfo(hostName.data(), nullptr, &settings, &ipList_); status != 0) {
        throw std::runtime_error(std::format("{}", gai_strerror(status)));
    }
}

HostIpInfo::~HostIpInfo() {
    freeaddrinfo(ipList_);
}

const std::string& HostIpInfo::getHostName() const {
    return hostName_;
}

std::vector<IpInfo> HostIpInfo::getIpList() const {
    std::vector<IpInfo> result;
    for (auto cur_node = ipList_; cur_node != nullptr; cur_node = cur_node->ai_next) {
        switch (cur_node->ai_family) {
            case AF_INET: {
                auto address = reinterpret_cast<sockaddr_in*>(cur_node->ai_addr);
                result.push_back({
                    *address,
                    ConvertToString(AF_INET, &address->sin_addr),
                    IP::v4
                });
                break;
            }
            case AF_INET6: {
                auto address = reinterpret_cast<sockaddr_in6*>(cur_node->ai_addr);
                result.push_back({
                    *address,
                    ConvertToString(AF_INET6, &address->sin6_addr),
                    IP::v6
                });
                break;
            }
            default:
                throw std::runtime_error(std::format("unknown ip format: {}", cur_node->ai_family));
        }
    }
    return result;
}

std::chrono::duration<double, std::milli> Ping::operator()(const sockaddr_in& addr) const {
    int sockfd = OpenSocket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct timeval timeout = { .tv_sec = 1 };
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    std::array<char, kPacketSize> packet{};
    auto* icmp = reinterpret_cast<struct icmp*>(packet.data());

    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = getpid();
    icmp->icmp_seq = 1;
    icmp->icmp_cksum = CheckSum(icmp, packet.size());

    auto begin = std::chrono::steady_clock::now();

    if (sendto(sockfd, packet.data(), packet.size(), 0,
               reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in)) < 0) {
        close(sockfd);
        throw std::runtime_error("sendto fatal failure");
    }

    std::array<char, kDefaultBufSize> buf;
    if (recvfrom(sockfd, buf.data(), buf.size(), 0, nullptr, nullptr) < 0) {
        close(sockfd);
        throw std::runtime_error("recvfrom timeout or failure");
    }

    auto end = std::chrono::steady_clock::now();
    close(sockfd);

    return end - begin;
}

std::chrono::duration<double, std::milli> Ping::operator()(const sockaddr_in6& addr) const {
    int sockfd = OpenSocket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    struct timeval timeout = { .tv_sec = 1 };
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    std::array<char, kPacketSize> packet{};
    auto* icmp6 = reinterpret_cast<struct icmp6_hdr*>(packet.data());

    icmp6->icmp6_type = ICMP6_ECHO_REQUEST;
    icmp6->icmp6_code = 0;
    icmp6->icmp6_id = getpid();
    icmp6->icmp6_seq = 1;
    icmp6->icmp6_cksum = 0;

    auto begin = std::chrono::steady_clock::now();

    if (sendto(sockfd, packet.data(), packet.size(), 0,
               reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in6)) < 0) {
        close(sockfd);
        throw std::runtime_error("sendto fatal failure");
    }

    std::array<char, kDefaultBufSize> buf;
    if (recvfrom(sockfd, buf.data(), buf.size(), 0, nullptr, nullptr) < 0) {
        close(sockfd);
        throw std::runtime_error("recvfrom timeout or failure");
    }

    auto end = std::chrono::steady_clock::now();
    close(sockfd);

    return end - begin;
}
