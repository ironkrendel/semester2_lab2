#pragma once

#include <iostream>
#include <string>

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

    struct Time {
        int seconds = 0;
        int minutes = 0;
        int hours = 0;
    };

    struct Date {
        int day = 1;
        int month = 1;
        int year = 0;

        static inline int getDayCount(int month) {
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
        int seconds = 0;
        int minutes = 0;
        int hours = 0;
        int day = 1;
        int month = 1;
        int year = 0;
    public:
        Datetime();
        Datetime(Time time, Date date);
        Datetime(int seconds, int minutes, int hours, int day, int month, int year);

        static int checkTime(Time time);
        static int checkDate(Date date);

        inline int getSeconds() {return seconds;};
        inline void setSeconds(int _seconds) {ASSERT_AND_SET_TIME(_seconds, seconds, Datetime::checkTime(Time{_seconds, 0, 0}), "Error! Time out of range!")};
        inline int getMinutes() {return minutes;};
        inline void setMinutes(int _minutes) {ASSERT_AND_SET_TIME(_minutes, minutes, Datetime::checkTime(Time{0, _minutes, 0}), "Error! Time out of range!")};
        inline int getHours() {return hours;};
        inline void setHours(int _hours) {ASSERT_AND_SET_TIME(_hours, hours, Datetime::checkTime(Time{0, 0, _hours}), "Error! Time out of range!")};
        inline int getDay() {return day;};
        inline void setDay(int _day) {ASSERT_AND_SET_TIME(_day, day, Datetime::checkDate(Date{_day, month, 0}), "Error! Date out of range!")};
        inline int getMonth() {return month;};
        inline void setMonth(int _month) {ASSERT_AND_SET_TIME(_month, month, Datetime::checkDate(Date{1, _month, 0}), "Error! Date out of range!")};
        inline int getYear() {return year;};
        inline void setYear(int _year) {ASSERT_AND_SET_TIME(_year, year, Datetime::checkDate(Date{1, 1, _year}), "Error! Date out of range!")};

        inline Time time() {return Time{seconds, minutes, hours};};
        void setTime(Time time);
        inline Date date() {return Date{day, month, year};};
        void setDate(Date date);

        void printDateString();
        void printDateString(const std::string format);

        void addSeconds(const int seconds);
        void addMinutes(const int minutes);
        void addHours(const int hours);
        void addDays(const int days);
        void addMonths(const int months);
        void addYears(const int years);
    };
}