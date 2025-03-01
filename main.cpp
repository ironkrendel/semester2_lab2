#include <iostream>
#include <chrono>
#include <thread>

#include <datetime.hpp>
#include <networking.hpp>
#include <colors.hpp>

int main() {
    std::cout << "Client (1) / Server (2): ";
    
    int choice;

    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Enter server ip address: ";

        std::string ip;
        std::cin >> ip;

        std::cout << "Enter server port: ";
        int port;
        std::cin >> port;

        TetoNetworking::Client client(ip, port);

        TetoDatetime::Datetime dt = client.getDatetime();
        dt.printDateString();

        std::cout << "Timestamp - " << client.getTimestamp() << std::endl;
    }
    else if (choice == 2) {
        TetoNetworking::Server server;

        server.loop();
    }

    return 0;
}