#include <iostream>
#include <chrono>
#include <thread>

#include <datetime.hpp>

int main() {
    TetoDatetime::Datetime datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 0});

    try {
        datetime.printDateString("current day: %D/%M/%Y");
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    datetime.printDateString();

    return 0;
}