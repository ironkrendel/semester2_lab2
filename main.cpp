#include <iostream>
#include <chrono>
#include <thread>

#include <datetime.hpp>

int main() {
    TetoDatetime::Datetime datetime(TetoDatetime::Time{0, 10, 0}, TetoDatetime::Date{1, 1, 0});

    while (true) {
        datetime.printDateString("Aboba - %h:%m:%s %D/%M/%Y");
        datetime.addSeconds(-10);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}