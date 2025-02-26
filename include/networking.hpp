#pragma once

#include <datetime.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace TetoNetworking {
    constexpr unsigned int DEFAULT_PORT_SERVER = 50104;
    constexpr auto DOMAIN = AF_INET;
    constexpr int OPT = 1;
    constexpr int BROADCAST_ENABLE = 1;
    constexpr int QUEUE_SIZE = 10;

    class Server {
    private:
        unsigned int port = DEFAULT_PORT_SERVER;
        int sockfd;
        sockaddr_in address;
        socklen_t addrlen;
    public:
        Server();
        Server(unsigned int socket_port);
        ~Server();

        void loop();
    };

    class Client {
    private:
        unsigned int port = DEFAULT_PORT_SERVER;
        int sockfd;
        int bcastsockdf;
        sockaddr_in address;
        socklen_t addrlen;
    public:
        Client();
        Client(unsigned int socket_port);
        ~Client();
    };
}