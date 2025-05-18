#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>       // Для fcntl() и O_NONBLOCK
    #include <sys/select.h>  // Для select()
    #include <errno.h>       // Для errno и EINPROGRESS
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define closesocket close
#endif

#include "Core/host.hpp"
#include "Core/IP.hpp"
#include "Core/MAC.hpp"
#include "Core/port.hpp"

namespace panic {

class PortScanner {
public:
    static Host scanPorts(const IPv4& ip, int startPort, int endPort) {
        Host host;
        host.set_IPv4(ip);
        host.set_status(ONLINE); // Предполагаем, что хост онлайн

        #ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                std::cerr << "Winsock initialization failed\n";
                host.set_status(OFFLINE);
                return host;
            }
        #endif

        for (int port = startPort; port <= endPort; ++port) {
            
            if (isPortOpen(ip, port)) {
                host.add_port(port);
            }
        }

        #ifdef _WIN32
            WSACleanup();
        #endif

        return host;
    }

private:
static bool isPortOpen(const IPv4& ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return false;
    }

    // Устанавливаем сокет в неблокирующий режим
    #ifdef _WIN32
        unsigned long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);
    #else
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    #endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(ip.get_value());

    // Пытаемся подключиться
    int result = connect(sock, (sockaddr*)&addr, sizeof(addr));
    
    #ifdef _WIN32
        if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
            closesocket(sock);
            return false;
        }
    #else
        if (result < 0 && errno != EINPROGRESS) {
            close(sock);
            return false;
        }
    #endif

    // Настраиваем select для таймаута
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);

    timeval timeout;
    timeout.tv_sec = 1;  // Таймаут 1 секунда
    timeout.tv_usec = 0;

    result = select(sock + 1, nullptr, &writefds, nullptr, &timeout);
    
    bool isOpen = false;
    if (result > 0) {
        // Проверяем, что соединение действительно установлено
        int error = 0;
        socklen_t len = sizeof(error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
        isOpen = (error == 0);
    }

    // Восстанавливаем блокирующий режим (опционально)
    #ifdef _WIN32
        mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);
    #else
        fcntl(sock, F_SETFL, flags);
    #endif

    closesocket(sock);
    return isOpen;
    }
};

} // namespace panic

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
        panic::Host scanResult = panic::PortScanner::scanPorts(ip, startPort, endPort);

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