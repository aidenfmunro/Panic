#pragma once

#include <mutex>

#include "Core/host.hpp"

namespace panic {

struct ThreadData {
    int port;
    Host* host;
    std::mutex* mutex;
};

class PortScanner {
public:
    static void scanPorts(Host& host, int startPort, int endPort);

private:
    static void* checkPort(void* arg);
    static bool isPortOpen(const IPv4& ip, int port);

};

} // namespace panic
