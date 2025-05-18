// PANIC host structure that describe host
#pragma once

#include "Core/IP.hpp"
#include "Core/MAC.hpp"
#include "Core/port.hpp"
#include <map>
#include <vector>

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
    std::vector<time_t> responseTime_;  // response time history 
    std::map<long, port> ports_; // Port vector contains ports that were/are open
public:
    Host() : hostIP_(0), hostName_("noname"), hostMAC_(0),
             status_(OFFLINE), responseTime_(), ports_() {};

    // Port manipulation
    auto add_port(const port& port) {return ports_.insert({port.get_value(), port});}
    auto add_port(long port_value) {
      return ports_.emplace(std::make_pair(port_value, port(port_value)));
    }

    size_t erase_port(long port_value) {return ports_.erase(port_value);}

    auto find_port(long port_value) {return ports_.find(port_value);}
    auto find_port(const port& port) {return ports_.find(port.get_value());}
    // TODO write iterator adapter to port map
    // Port iterators
    auto ports_begin() {return ports_.begin();}
    auto ports_end() {return ports_.end();}

    // Response time manipulation
    void push_back_response_time(time_t time) {responseTime_.push_back(time);}
    void append_response_time_vector(const std::vector<time_t> &time_vector) {
      for (time_t time : time_vector) {
        responseTime_.push_back(time);
      }
    }

    auto& get_last_response_time() {return responseTime_.back();}

    // Response time iterators
    auto response_time_begin() {return responseTime_.begin();} 
    auto response_time_end() {return responseTime_.end();}

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

/// @brief Get Host name by IP address 
/// @param ipAddress 
/// @return string with host name
std::string getHostnameByIP(const IPv4& ipAddress);

} // namespace panic
