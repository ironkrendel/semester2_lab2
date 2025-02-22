#include <iostream>
#include <chrono>
#include <thread>

#include <datetime.hpp>
#include <colors.hpp>

int main() {
    // TetoDatetime::Datetime datetime(TetoDatetime::Time{1, 0, 0}, TetoDatetime::Date{1, 1, 0});
    TetoDatetime::Datetime datetime(TetoDatetime::Datetime::getDatetime());
    datetime.addDays(datetime.getDay() * -1 + 1);
    TetoDatetime::Datetime datetime2(datetime);

    // try {
    //     datetime.printDateString("current day: %D/%M/%Y");
    // }
    // catch (std::exception& e) {
    //     std::cout << e.what() << std::endl;
    // }
    
    datetime.printDateString();
    datetime2.printDateString();

    datetime2.addDays(TetoDatetime::Date::getDayCount(datetime.getMonth()));

    datetime.printDateString();
    datetime2.printDateString();

    TetoDatetime::Datetime datetime3(TetoDatetime::Datetime::stringToDatetime("23:59:59 - 02/01/2025 05","%h:%m:%s - %D/%M/%Y %s"));

    datetime3.printDateString();
    
    return 0;
}