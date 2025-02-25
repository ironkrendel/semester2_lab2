#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <optional>

typedef enum TetoResult {
    TETO_SUCCESS,
    TETO_ERROR_TIME_OUT_OF_RANGE,
    TETO_ERROR_DATE_OUT_OF_RANGE,
} TetoResult;

namespace TetoDatetime {
    #define ASSERT_AND_SET_TIME(src, dst, assert_func, error_msg) {\
        if (assert_func != TETO_SUCCESS) {\
            throw std::runtime_error(error_msg);\
        }\
        dst = src;\
    };

    constexpr int DEFAULT_VALUE_SECONDS = 0;
    constexpr int DEFAULT_VALUE_MINUTES = 0;
    constexpr int DEFAULT_VALUE_HOURS = 0;
    constexpr int DEFAULT_VALUE_DAY = 1;
    constexpr int DEFAULT_VALUE_MONTH = 1;
    constexpr int DEFAULT_VALUE_YEAR = 1900;

    struct Time {
        int seconds = 0;
        int minutes = 0;
        int hours = 0;

        [[nodiscard]]
        inline auto toString() const -> std::string {
            return std::string(std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds));
        }
    };

    struct Date {
        int day = 1;
        int month = 1;
        int year = 0;

        [[nodiscard]]
        inline auto toString() const -> std::string {
            return std::string(std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year));
        }

        static inline auto getDayCount(int month) -> int {
            switch (month) {
                case 1: return 31;
                case 2: return 28;
                case 3: return 31;
                case 4: return 30;
                case 5: return 31;
                case 6: return 30;
                case 7: return 31;
                case 8: return 31;
                case 9: return 30;
                case 10: return 31;
                case 11: return 30;
                case 12: return 31;
                default:
                    throw std::runtime_error("Error in getDayCount! Month out of range. Excepted 1 <= n <= 12, got n = " + std::to_string(month) + " instead.");
            }
        }
    };

    class Datetime {
    private:
        int seconds = DEFAULT_VALUE_SECONDS;
        int minutes = DEFAULT_VALUE_MINUTES;
        int hours = DEFAULT_VALUE_HOURS;
        int day = DEFAULT_VALUE_DAY;
        int month = DEFAULT_VALUE_MONTH;
        int year = DEFAULT_VALUE_YEAR;
    public:
        Datetime() = default;
        Datetime(Time time, Date date);
        Datetime(int seconds, int minutes, int hours, int day, int month, int year);
        Datetime(Datetime& src); // copy
        Datetime(Datetime&& src) noexcept; // move
        auto operator=(const Datetime& src) -> Datetime&; // copy
        auto operator=(Datetime&& src) noexcept -> Datetime&; // move
        ~Datetime();

        [[nodiscard]]
        static auto checkTime(Time time) -> int;
        [[nodiscard]]
        static auto checkDate(Date date) -> int;
        [[nodiscard]]
        static auto getDatetime() -> Datetime;
        void syncDatetime();

        [[nodiscard]]
        inline auto getTimestamp() const -> long long {
            long long result = 0;
            result += (365 * 24 * 60 * 60) * static_cast<long long>(year);
            for (int i = 1;i < month;i++) {
                result += static_cast<long long>(Date::getDayCount(i)) * (24 * 60 * 60);
            }
            result += (24 * 60 * 60) * static_cast<long long>(day - 1);
            result += (60 * 60) * static_cast<long long>(hours);
            result += 60 * static_cast<long long>(minutes);
            result += static_cast<long long>(seconds);

            return result;
        }

        [[nodiscard]]
        inline auto getSeconds() const -> decltype(seconds) {return seconds;};
        inline void setSeconds(int _seconds) {ASSERT_AND_SET_TIME(_seconds, seconds, Datetime::checkTime(Time{_seconds, 0, 0}), "Error! Time out of range!")};
        [[nodiscard]]
        inline auto getMinutes() const -> decltype(minutes) {return minutes;};
        inline void setMinutes(int _minutes) {ASSERT_AND_SET_TIME(_minutes, minutes, Datetime::checkTime(Time{0, _minutes, 0}), "Error! Time out of range!")};
        [[nodiscard]]
        inline auto getHours() const -> decltype(hours) {return hours;};
        inline void setHours(int _hours) {ASSERT_AND_SET_TIME(_hours, hours, Datetime::checkTime(Time{0, 0, _hours}), "Error! Time out of range!")};
        [[nodiscard]]
        inline auto getDay() const -> decltype(day) {return day;};
        inline void setDay(int _day) {ASSERT_AND_SET_TIME(_day, day, Datetime::checkDate(Date{_day, month, 0}), "Error! Date out of range!")};
        [[nodiscard]]
        inline auto getMonth() const -> decltype(month) {return month;};
        inline void setMonth(int _month) {ASSERT_AND_SET_TIME(_month, month, Datetime::checkDate(Date{1, _month, 0}), "Error! Date out of range!")};
        [[nodiscard]]
        inline auto getYear() const -> decltype(year) {return year;};
        inline void setYear(int _year) {ASSERT_AND_SET_TIME(_year, year, Datetime::checkDate(Date{1, 1, _year}), "Error! Date out of range!")};

        [[nodiscard]]
        inline auto time() const -> Time {return Time{seconds, minutes, hours};};
        void setTime(Time time);
        [[nodiscard]]
        inline auto date() const -> Date {return Date{day, month, year};};
        void setDate(Date date);

        void printDateString() const;
        void printDateString(const std::string& format) const;

        [[nodiscard]]
        static auto stringToDatetime(const std::string& string, const std::string& format) -> Datetime;

        void addSeconds(int seconds);
        void addMinutes(int minutes);
        void addHours(int hours);
        void addDays(int days);
        void addMonths(int months);
        void addYears(int years);

        [[nodiscard]]
        inline auto secsTo(Datetime& datetime) const -> long long {
            return static_cast<long long>(datetime.getTimestamp() - this->getTimestamp());
        }

        [[nodiscard]]
        inline auto daysTo(Datetime& datetime) const -> long long {
            return static_cast<long long>((datetime.getTimestamp() - this->getTimestamp()) / (24 * 60 * 60));
        }
    };
}