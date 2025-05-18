#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <vector>

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

#include "Net/portScanner.hpp"

namespace panic {

void PortScanner::scanPorts(Host& host, int startPort, int endPort) {
    host.set_status(ONLINE);

    std::vector<pthread_t> threads;
    std::mutex mutex;

    for (int port = startPort; port <= endPort; ++port) {
        auto* data = new ThreadData{port, &host, &mutex};
        
        pthread_t thread;
        if (pthread_create(&thread, nullptr, checkPort, data) != 0) {
            delete data;
            std::cerr << "Failed to create thread for port " << port << std::endl;
            continue;
        }
        threads.push_back(thread);
    }

    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }
}

void* PortScanner::checkPort(void* arg) {
    std::unique_ptr<ThreadData> data(static_cast<ThreadData*>(arg));

    if (isPortOpen(data->host->get_IPv4(), data->port)) {
        std::lock_guard<std::mutex> lock(*data->mutex);
        data->host->add_port(data->port);
    }

    return nullptr;
}

bool PortScanner::isPortOpen(const IPv4& ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return false;
    }

    // Устанавливаем сокет в неблокирующий режим
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(ip.get_value());

    // Пытаемся подключиться
    int result = connect(sock, (sockaddr*)&addr, sizeof(addr));

    if (result < 0 && errno != EINPROGRESS) {
        close(sock);
        return false;
    }
    
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
    fcntl(sock, F_SETFL, flags);

    closesocket(sock);
    return isOpen;
    }

} // namespace panic
