#pragma once

#include <datetime.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <chrono>
#include <thread>

namespace TetoNetworking {
    constexpr unsigned int DEFAULT_PORT_SERVER = 50104;
    constexpr auto DOMAIN = AF_INET;
    constexpr int OPT = 1;
    constexpr int BROADCAST_ENABLE = 1;
    constexpr int QUEUE_SIZE = 10;

    struct DatetimePacket {
        TetoDatetime::Date date;
        TetoDatetime::Time time;

        DatetimePacket() = default;
        DatetimePacket(TetoDatetime::Date _date, TetoDatetime::Time _time) {
            date = _date;
            time = _time;
        }
    };

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
        unsigned int port;
        std::string ip;
    public:
        Client() = delete;
        Client(std::string serverIP, int serverPort = DEFAULT_PORT_SERVER);
        ~Client() = default;

        auto getDatetime() const -> TetoDatetime::Datetime;
        auto getTimestamp() const -> long long;
    };
}