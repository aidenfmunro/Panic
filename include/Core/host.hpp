// PANIC host structure that describe host
#pragma once

#include "Core/IP.hpp"

/*
      "devices": [
    {
      "hostname": "device-1",
      "ip_address": "192.168.1.10",
      "mac_address": "00:14:22:01:23:45",
      "status": "online",
      "ping": {
        "status": "success",
        "response_time_ms": 25
      },
      "ports": [
        {
          "port": 80,
          "status": "open"
        },
        {
          "port": 22,
          "status": "closed"
        }
*/

namespace panic {

class Host
{
private:
    std::string hostName_;  // Host name
    IPv4 hostIP_;           // IP address of host
    // TODO mac address of host
    // TODO host status: online, offline
    // TODO ping status (last response time, response time history)
    // TODO ports list  { "port": 22, "status": "closed"}
public:
    Host() : hostIP_(0), hostName_("noname") {};
    explicit Host(IPv4 IP) : hostIP_(IP), hostName_("noname") {};

    std::string get_name() const {return hostName_;}
};

} // namespace panic