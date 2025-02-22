#include <datetime.hpp>
#include <colors.hpp>

using namespace TetoDatetime;

// #define DEBUG

Datetime::Datetime(Time time, Date date) {
    #ifdef DEBUG
    std::cout << "birth" << std::endl;
    #endif
    if (checkTime(time) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Time out of range! Passed value - " + time.toString());
    }
    if (checkDate(date) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Date out of range! Passed value - " + date.toString());
    }
    seconds = time.seconds;
    minutes = time.minutes;
    hours = time.hours;
    day = date.day;
    month = date.month;
    year = date.year;
}

Datetime::Datetime(int _seconds, int _minutes, int _hours, int _day, int _month, int _year) {
    #ifdef DEBUG
    std::cout << "birth" << std::endl;
    #endif
    if (checkTime(Time{_seconds, _minutes, _hours}) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Time out of range! Passed value - " + Time{_seconds, _minutes, _hours}.toString());
    }
    if (checkDate(Date{_day, _month, _year}) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Date out of range! Passed value - " + Date{_day, _month, _year}.toString());
    }
    seconds = _seconds;
    minutes = _minutes;
    hours = _hours;
    day = _day;
    month = _month;
    year = _year;
}

Datetime::Datetime(Datetime& src) : Datetime::Datetime(src.seconds, src.minutes, src.hours, src.day, src.month, src.year) {
    #ifdef DEBUG
    std::cout << "Copy" << std::endl;
    #endif
}

Datetime::Datetime(Datetime&& src) noexcept : Datetime::Datetime(src.seconds, src.minutes, src.hours, src.day, src.month, src.year) {
    #ifdef DEBUG
    std::cout << "Move" << std::endl;
    #endif
}

auto Datetime::operator=(const Datetime& src) -> Datetime& {
    #ifdef DEBUG
    std::cout << "copy" << std::endl;
    #endif
    seconds = src.seconds;
    minutes = src.minutes;
    hours = src.hours;
    day = src.day;
    month = src.month;
    year = src.year;
    return *this;
}

auto Datetime::operator=(Datetime&& src) noexcept -> Datetime& {
    #ifdef DEBUG
    std::cout << "move" << std::endl;
    #endif
    seconds = src.seconds;
    minutes = src.minutes;
    hours = src.hours;
    day = src.day;
    month = src.month;
    year = src.year;
    return *this;
}

Datetime::~Datetime() {
    #ifdef DEBUG
    std::cout << "Death" << std::endl;
    #endif
}

auto Datetime::checkTime(Time time) -> int {
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

auto Datetime::checkDate(Date date) -> int {
    if ((date.month < 1) || (date.month > 12)) {
        return TETO_ERROR_DATE_OUT_OF_RANGE;
    }
    if ((date.day < 1) || (date.day > Date::getDayCount(date.month))) {
        return TETO_ERROR_DATE_OUT_OF_RANGE;
    }
    return TETO_SUCCESS;
}

auto Datetime::getDatetime() -> Datetime {
    std::time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time{};
    localtime_r(&nowTime, &time);
    if (time.tm_mon == 1 && time.tm_mday == 29) {
        std::cout << ANSI_COLOR_RED << "Ignoring leap year according to specification and setting day value to 28!" << ANSI_COLOR_RESET << std::endl;
        time.tm_mday = 28;
    }
    return Datetime(time.tm_sec, time.tm_min, time.tm_hour, time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);
}

void Datetime::syncDatetime() {
    std::time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time{};
    localtime_r(&nowTime, &time);
    if (time.tm_mon == 1 && time.tm_mday == 29) {
        std::cout << ANSI_COLOR_RED << "Ignoring leap year according to specification and setting day value to 28!" << ANSI_COLOR_RESET << std::endl;
        time.tm_mday = 28;
    }
    seconds = time.tm_sec;
    minutes = time.tm_min;
    hours = time.tm_hour;
    day = time.tm_mday;
    month = time.tm_mon + 1;
    year = time.tm_year + 1900;
}

void Datetime::setTime(Time _time) {
    if (checkTime(_time) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Time out of range! Passed value - " + _time.toString());
    }
    seconds = _time.seconds;
    minutes = _time.minutes;
    hours = _time.hours;
}

void Datetime::setDate(Date _date) {
    if (checkDate(_date) != TETO_SUCCESS) {
        throw std::runtime_error("Error! Date out of range! Passed value - " + _date.toString());
    }
    day = _date.day;
    month = _date.month;
    year = _date.year;
}

void Datetime::printDateString() const {
    std::cout << "Datetime - " << hours << ':' << minutes << ':' << seconds << ' ' << day << '/' << month << '/' << year << std::endl;
}

void Datetime::printDateString(const std::string& format) const {
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
            else if (format[i + 1] == '%') {
                std::cout << '%';
            }
            else {
                std::cout << '%';
                continue;
            }
            i++;
        }
        else {
            std::cout << format[i];
        }
    }
    std::cout << std::endl;
}

static void NEGATE_BUFFER(std::optional<int>& buffer, bool negation) {
    if (!buffer.has_value()) return;
    if (negation) buffer = buffer.value() * -1;
};

auto Datetime::stringToDatetime(const std::string& string, const std::string& format) -> Datetime {
    std::optional<int> seconds_buffer;
    bool seconds_negation = false;
    std::optional<int> minutes_buffer;
    bool minutes_negation = false;
    std::optional<int> hours_buffer;
    bool hours_negation = false;
    std::optional<int> day_buffer;
    bool day_negation = false;
    std::optional<int> month_buffer;
    bool month_negation = false;
    std::optional<int> year_buffer;
    bool year_negation = false;

    std::size_t iterStr = 0;
    for (std::size_t iterFormat = 0;iterFormat < format.length();iterFormat++) {
        if (iterFormat + 1 < format.length() && format[iterFormat] == '%') {
            // clear buffer
            switch (format[iterFormat + 1])
            {
            case 's':
                seconds_buffer.reset();
                break;
            case 'm':
                minutes_buffer.reset();
                break;
            case 'h':
                hours_buffer.reset();
                break;
            case 'D':
                day_buffer.reset();
                break;
            case 'M':
                month_buffer.reset();
                break;
            case 'Y':
                year_buffer.reset();
                break;
            default:
                break;
            }
            // parse
            while (iterStr < string.length() && (isdigit(string[iterStr]) || string[iterStr] == '-')) {
                // flip negation flag for buffer
                if (string[iterStr] == '-') {
                    switch (format[iterFormat + 1])
                    {
                    case 's':
                        seconds_negation = true;
                        break;
                    case 'm':
                        minutes_negation = true;
                        break;
                    case 'h':
                        hours_negation = true;
                        break;
                    case 'D':
                        day_negation = true;
                        break;
                    case 'M':
                        month_negation = true;
                        break;
                    case 'Y':
                        year_negation = true;
                        break;
                    default:
                        break;
                    }
                }
                // add number to buffer
                else {
                    switch (format[iterFormat + 1])
                    {
                    case 's':
                        seconds_buffer = seconds_buffer.value_or(0) * 10;
                        seconds_buffer = seconds_buffer.value() + (string[iterStr] - '0');
                        break;
                    case 'm':
                        minutes_buffer = minutes_buffer.value_or(0) * 10;
                        minutes_buffer = minutes_buffer.value() + (string[iterStr] - '0');
                        break;
                    case 'h':
                        hours_buffer = hours_buffer.value_or(0) * 10;
                        hours_buffer = hours_buffer.value() + (string[iterStr] - '0');
                        break;
                    case 'D':
                        day_buffer = day_buffer.value_or(0) * 10;
                        day_buffer = day_buffer.value() + (string[iterStr] - '0');
                        break;
                    case 'M':
                        month_buffer = month_buffer.value_or(0) * 10;
                        month_buffer = month_buffer.value() + (string[iterStr] - '0');
                        break;
                    case 'Y':
                        year_buffer = year_buffer.value_or(0) * 10;
                        year_buffer = year_buffer.value() + (string[iterStr] - '0');
                        break;
                    default:
                        break;
                    }
                }
                iterStr++;
            }
            iterFormat++;
        }
        else if (string[iterStr] != format[iterFormat]) {
            break;
        }
        else {
            iterStr++;
        }
    }

    NEGATE_BUFFER(seconds_buffer, seconds_negation);
    NEGATE_BUFFER(minutes_buffer, minutes_negation);
    NEGATE_BUFFER(hours_buffer, hours_negation);
    NEGATE_BUFFER(day_buffer, day_negation);
    NEGATE_BUFFER(month_buffer, month_negation);
    NEGATE_BUFFER(year_buffer, year_negation);

    return Datetime(
        seconds_buffer.value_or(DEFAULT_VALUE_SECONDS),
        minutes_buffer.value_or(DEFAULT_VALUE_MINUTES),
        hours_buffer.value_or(DEFAULT_VALUE_HOURS),
        day_buffer.value_or(DEFAULT_VALUE_DAY),
        month_buffer.value_or(DEFAULT_VALUE_MONTH),
        year_buffer.value_or(DEFAULT_VALUE_YEAR)
    );
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