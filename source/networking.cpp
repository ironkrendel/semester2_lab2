#include <networking.hpp>
#include <colors.hpp>

using namespace TetoNetworking;

Server::Server(unsigned int socket_port) {
    port = socket_port;
    sockfd = socket(DOMAIN, SOCK_STREAM, 0);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &OPT, sizeof(OPT)) < 0) {
        throw std::runtime_error("setsockopt");
    }

    address = {};
    address.sin_family = DOMAIN;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    addrlen = sizeof(address);

    while (true) {
        if (bind(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
            int errorCode = errno;
            std::cout << errno << std::endl;
            if (errno == 98 || errno == 22) {
                port++;
                address.sin_port = htons(port);
                addrlen = sizeof(address);
                std::cout << port << std::endl;
            }
            else {
                shutdown(sockfd, SHUT_RDWR);
                throw std::runtime_error("bind " + std::to_string(errorCode));   
            }
        }
        else {
            break;
        }
    }

    if (listen(sockfd, QUEUE_SIZE) < 0) {
        throw std::runtime_error("listen");
    }
}

Server::Server() : Server(DEFAULT_PORT_SERVER) {
    std::cout << "Creating server" << std::endl;
}

void Server::loop() {
    std::cout << "Server started on port " << port << std::endl;
    int cnt = 0;
    while (true) {
        // std::cout << cnt << std::endl;
        int new_socket = accept(sockfd, (sockaddr*)&address, &addrlen);

        char buffer[1024] = {0};
        recv(new_socket, buffer, 1024 - 1, 0);
        std::cout << "---------------------------------------------\n";
        std::cout << "MESSAGE: \n\n";
        std::cout << buffer << '\n' << std::endl;
        std::cout << "REPLY: \n\n";

        std::string message(buffer);

        if (message == "datetime") {
            TetoDatetime::Datetime dt;
            dt.syncDatetime();
            DatetimePacket packet{dt.date(), dt.time()};
            send(new_socket, &packet, sizeof(packet), 0);
            std::cout << "__DATETIME_PACKET__\n" << std::endl;
        }
        else if (message == "timestamp") {
            TetoDatetime::Datetime dt;
            dt.syncDatetime();
            long long ts = dt.getTimestamp();
            send(new_socket, &ts, sizeof(ts), 0);
            std::cout << "__TIMESTAMP__\n" << std::endl;
        }
        else {
            std::string response = "501 Not Implemented\n";
            send(new_socket, response.data(), response.length(), 0);
            std::cout << response << std::endl;
        }

        std::cout << "---------------------------------------------\n";

        shutdown(new_socket, SHUT_RDWR);
        cnt++;
    }
}

Server::~Server() {
    shutdown(sockfd, SHUT_RDWR);
}

Client::Client(std::string serverIP, int serverPort) {
    port = serverPort;
    ip = serverIP;
}

auto Client::getDatetime() const -> TetoDatetime::Datetime {
    int sockfd = socket(DOMAIN, SOCK_STREAM, 0);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &OPT, sizeof(OPT));

    sockaddr_in address;
    address.sin_family = DOMAIN;
    inet_pton(DOMAIN, ip.data(), &address.sin_addr);
    address.sin_port = htons(DEFAULT_PORT_SERVER);

    connect(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    std::string message = "datetime";
    send(sockfd, message.data(), message.length(), 0);

    DatetimePacket packet;
    recv(sockfd, &packet, sizeof(packet), 0);

    shutdown(sockfd, SHUT_RDWR);

    TetoDatetime::Datetime dt(packet.time, packet.date);

    return dt;
}

auto Client::getTimestamp() const -> long long {
    int sockfd = socket(DOMAIN, SOCK_STREAM, 0);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &OPT, sizeof(OPT));

    sockaddr_in address;
    address.sin_family = DOMAIN;
    inet_pton(DOMAIN, ip.data(), &address.sin_addr);
    address.sin_port = htons(DEFAULT_PORT_SERVER);

    connect(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    std::string message = "timestamp";
    send(sockfd, message.data(), message.length(), 0);

    long long timestamp;
    recv(sockfd, &timestamp, sizeof(timestamp), 0);

    shutdown(sockfd, SHUT_RDWR);

    return timestamp;
}