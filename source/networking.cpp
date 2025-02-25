#include <networking.hpp>
#include <colors.hpp>

using namespace TetoNetworking;

Server::Server(unsigned int socket_port) {
    port = socket_port;
    sockfd = socket(DOMAIN, SOCK_STREAM, 0);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &OPT, sizeof(OPT));

    address = {};
    address.sin_family = DOMAIN;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(socket_port);
    addrlen = sizeof(address);

    bind(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address));

    listen(sockfd, QUEUE_SIZE);
}

Server::Server() : Server(DEFAULT_PORT_SERVER) {
    std::cout << "Creating server" << std::endl;
}

void Server::loop() {
    std::cout << "Server started on port " << port << std::endl;
    int cnt = 0;
    while (true) {
        std::cout << cnt << std::endl;
        int new_socket = accept(sockfd, (sockaddr*)&address, &addrlen);

        char buffer[1024] = {0};
        ssize_t valread = recv(new_socket, buffer, 1024 - 1, 0);
        std::cout << "---------------------------------------------\n";
        std::cout << valread << '\n';
        std::cout << "MESSAGE: \n\n";
        std::cout << buffer << '\n' << std::endl;
        std::cout << "---------------------------------------------\n";

        std::string requestType, requestAddr, requestProtocol;
        std::string requestFormat;
        for (int i = 0;i < 1024 && buffer[i] != '\n';i++) {
            requestFormat.push_back(buffer[i]);
        }
        std::stringstream ss(requestFormat);

        ss >> requestType >> requestAddr >> requestProtocol;
        
        if (requestType == "GET") {
            if (requestAddr == "/") {
                std::string response = "HTTP/1.1 200 OK\nServer: Teto\nContent-Type: text/html\n\n<html><head></head><body><div style=\"text-align:center\"><h1 style=\"font-size:100\">I LOVE KASANE TETO!</h1></div></body></html>";
                // std::cout << response << std::endl;
                send(new_socket, response.data(), response.length(), 0);
            }
            else if (requestAddr == "/time") {
                TetoDatetime::Datetime dt;
                dt.syncDatetime();
                std::string response = "HTTP/1.1 200 OK\nServer: Teto\nContent-Type: application/json\n\n{\"timestamp\":" + std::to_string(dt.getTimestamp()) + "}";
                std::cout << dt.getTimestamp() << std::endl;
                send(new_socket, response.data(), response.length(), 0);
            }
            else if (requestAddr == "/timestamp") {
                TetoDatetime::Datetime dt;
                dt.syncDatetime();
                std::string response = "HTTP/1.1 200 OK\nServer: Teto\nContent-Type: text\n\n" + std::to_string(dt.getTimestamp());
                std::cout << dt.getTimestamp() << std::endl;
                send(new_socket, response.data(), response.length(), 0);
            }
        }
        else {
            std::string response = "HTTP/1.1 400 Bad Request\nServer: Teto\nContent-Type: text\n\n";
            // std::cout << response << std::endl;
            send(new_socket, response.data(), response.length(), 0);
        }

        shutdown(new_socket, SHUT_RDWR);
        cnt++;
    }
}

Server::~Server() {
    shutdown(sockfd, SHUT_RDWR);
}

Client::Client(unsigned int socket_port) {
    port = socket_port;
    sockfd = socket(DOMAIN, SOCK_STREAM, 0);
    bcastsockdf = socket(DOMAIN, SOCK_DGRAM, 0);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &OPT, sizeof(OPT));
    setsockopt(bcastsockdf, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_BROADCAST, &BROADCAST_ENABLE, sizeof(BROADCAST_ENABLE));

    address = {};
    address.sin_family = DOMAIN;
    inet_pton(DOMAIN, "172.31.31.32", &address.sin_addr);
    address.sin_port = htons(DEFAULT_PORT_SERVER);
    addrlen = sizeof(address);

    connect(sockfd, reinterpret_cast<sockaddr*>(&address), addrlen);
    std::string message = "GET / HTTP/1.1";
    send(sockfd, message.data(), message.length(), 0);

    char buffer[1024] = {0};
    ssize_t valread = recv(sockfd, buffer, 1024 - 1, 0);

    std::cout << buffer << std::endl;

    // bind(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    // bind(bcastsockdf, reinterpret_cast<sockaddr*>(&address), sizeof(address));
}

Client::Client() : Client(DEFAULT_PORT_SERVER) {
    std::cout << "Creating client" << std::endl;
}

Client::~Client() {
    shutdown(sockfd, SHUT_RDWR);
    shutdown(bcastsockdf, SHUT_RDWR);
}