#include <datetime.hpp>

using namespace TetoDatetime;

Datetime::Datetime() {
    
}

Datetime::Datetime(Time time, Date date) {
    if (checkTime(time) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Time out of range!");
    }
    if (checkDate(date) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Date out of range!");
    }
    seconds = time.seconds;
    minutes = time.minutes;
    hours = time.hours;
    day = date.day;
    month = date.month;
    year = date.year;
}

Datetime::Datetime(int _seconds, int _minutes, int _hours, int _day, int _month, int _year) {
    if (checkTime(Time{_seconds, _minutes, _hours}) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Time out of range!");
    }
    if (checkDate(Date{_day, _month, _year}) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Date out of range!");
    }
    seconds = _seconds;
    minutes = _minutes;
    hours = _hours;
    day = _day;
    month = _month;
    year = _year;
}

int Datetime::checkTime(Time time) {
    if ((time.seconds < 0) || (time.seconds >= 60)) {
        return TETO_ERROR_TIME_OUT_OF_RANGE;
    }
    if ((time.minutes < 0) || (time.minutes >= 60)) {
        return TETO_ERROR_TIME_OUT_OF_RANGE;
    }
    if ((time.hours < 0) || (time.hours >= 24)) {
        return TETO_ERROR_TIME_OUT_OF_RANGE;
    }
    return TETO_SUCCESS;
}

int Datetime::checkDate(Date date) {
    if ((date.month < 1) || (date.month > 12)) {
        return TETO_ERROR_DATE_OUT_OF_RANGE;
    }
    if ((date.day < 1) || (date.day > Date::getDayCount(date.month))) {
        return TETO_ERROR_DATE_OUT_OF_RANGE;
    }
    return TETO_SUCCESS;
}

void Datetime::setDate(Date _date) {
    if (checkDate(_date) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Date out of range!");
    }
    day = _date.day;
    month = _date.month;
    year = _date.year;
}

void Datetime::setTime(Time _time) {
    if (checkTime(_time) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Time out of range!");
    }
    seconds = _time.seconds;
    minutes = _time.minutes;
    hours = _time.hours;
}

void Datetime::printDateString() {
    std::cout << "Datetime - " << hours << ':' << minutes << ':' << seconds << ' ' << day << '/' << month << '/' << year << std::endl;
}

void Datetime::printDateString(const std::string format) {
    if (format.length() == 0) {
        std::cout << "Datetime - " << hours << ':' << minutes << ':' << seconds << ' ' << day << '/' << month << '/' << year << std::endl;
        return;
    }

    const std::size_t strlen = format.length();
    for (std::size_t i = 0;i < strlen;i++) {
        if (format[i] == '%' && i + 1 < strlen) {
            if (format[i + 1] == 's') {
                std::cout << seconds;
            }
            else if (format[i + 1] == 'm') {
                std::cout << minutes;
            }
            else if (format[i + 1] == 'h') {
                std::cout << hours;
            }
            else if (format[i + 1] == 'D') {
                std::cout << day;
            }
            else if (format[i + 1] == 'M') {
                std::cout << month;
            }
            else if (format[i + 1] == 'Y') {
                std::cout << year;
            }
            i++;
        }
        else {
            std::cout << format[i];
        }
    }
    std::cout << std::endl;
}

void Datetime::addSeconds(const int _seconds) {
    seconds += _seconds;
    if (seconds < 0) {
        this->addMinutes(abs(seconds) / -60 - ((abs(seconds) < 60) ? 1 : 0));
        seconds = ((60 * (abs(seconds) / 60 + 1)) + seconds) % 60;
    }
    else {
        this->addMinutes(seconds / 60);
        seconds = seconds % 60;
    }
}

void Datetime::addMinutes(const int _minutes) {
    minutes += _minutes;
    if (minutes < 0) {
        this->addHours(abs(minutes) / -60 - ((abs(minutes) < 60) ? 1 : 0));
        minutes = ((60 * (abs(minutes) / 60 + 1)) + minutes) % 60;
    }
    else {
        this->addHours(minutes / 60);
        minutes = minutes % 60;
    }
}

void Datetime::addHours(const int _hours) {
    hours += _hours;
    if (hours < 0) {
        this->addDays(abs(hours) / -24 - ((abs(hours) < 24) ? 1 : 0));
        hours = ((24 * (abs(hours) / 24 + 1)) + hours) % 24;
    }
    else {
        this->addDays(hours / 24);
        hours = hours % 24;
    }
}

void Datetime::addDays(const int _days) {
    day--;
    day += _days;
    if (_days < 0) {
        while (day < 0) {
            this->addMonths(-1);
            day += Date::getDayCount(month);
        }
        
    }
    else {
        while (day > Date::getDayCount(month) - 1) {
            day -= Date::getDayCount(month);
            this->addMonths(1);
        }
    }
    day++;
}

void Datetime::addMonths(const int _months) {
    month--;
    month += _months;
    if (_months < 0) {
        if (month < 0) {
            year -= abs(month) / 12 + 1;
            month = (12 * (abs(month) / 12 + 1)) + month;
        }
    }
    else {
        year += month / 12;
        month = month % 12;
    }
    month++;
}

void Datetime::addYears(const int _years) {
    year += _years;
}