// PANIC host structure that describe host
#pragma once

#include "Core/IP.hpp"
#include "Core/MAC.hpp"
#include "Core/port.hpp"
#include <map>

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
    std::map<long, port> ports_; // Port vector contains ports that were/are open
public:
    Host() : hostIP_(0), hostName_("noname"), hostMAC_(0),
             status_(OFFLINE), ports_() {};

    // Port manipulation
    auto add_port(const port& port) {return ports_.insert({port.get_value(), port});}
    auto add_port(long port_value) {
      return ports_.emplace(std::make_pair(port_value, port(port_value)));
    }

    size_t erase_port(long port_value) {return ports_.erase(port_value);}

    auto find_port(long port_value) {return ports_.find(port_value);}
    auto find_port(const port& port) {return ports_.find(port.get_value());}

    // Port iterators
    auto ports_begin() const {return ports_.begin();}
    auto ports_end() const {return ports_.end();}

    // Getters
    std::string get_name() const {return hostName_;}
    IPv4 get_IPv4() const {return hostIP_;}
    MAC_48 get_MAC() const {return hostMAC_;}
    HostStatus getStatus() const {return status_;}
    size_t get_ports_count() const {return ports_.size();}

    // Setters
    void set_name(const std::string &hostName) {hostName_ = hostName;}
    void set_IPv4(const IPv4 &IP) {hostIP_ = IP;}
    void set_MAC(const MAC_48 &MAC) {hostMAC_ = MAC;}
    void set_status(HostStatus status) {status_ = status;}
};

} // namespace panic