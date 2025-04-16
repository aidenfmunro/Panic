// PANIC host structure that describe host
#pragma once

#include "Core/IP.hpp"
#include "Core/MAC.hpp"

namespace panic {

enum HostStatus {
  OFFLINE,
  ONLINE
};

class Host
{
private:
    std::string hostName_;  // Host name
    IPv4 hostIP_;           // IP address of host
    MAC_48 hostMAC_;        // MAC-48 address of host
    HostStatus status_;     // Status of host (offline/online)
    // TODO ping status (last response time, response time history)
    // TODO ports list  { "port": 22, "status": "closed"}
public:
    Host() : hostIP_(0), hostName_("noname"), hostMAC_(0), status_(OFFLINE) {};

    // Getters
    std::string get_name() const {return hostName_;}
    IPv4 get_IPv4() const {return hostIP_;}
    MAC_48 get_MAC() const {return hostMAC_;}
    HostStatus getStatus() const {return status_;}

    // Setters
    void set_name(const std::string &hostName) {hostName_ = hostName;}
    void set_IPv4(const IPv4 &IP) {hostIP_ = IP;}
    void set_MAC(const MAC_48 &MAC) {hostMAC_ = MAC;}
    void set_status(HostStatus status) {status_ = status;}
};

} // namespace panic