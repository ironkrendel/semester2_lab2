#pragma once

#include <iostream>
#include <string>
#include <regex>

typedef enum TetoResult {
    TETO_SUCCESS,
    TETO_ERROR_TIME_OUT_OF_RANGE,
    TETO_ERROR_DATE_OUT_OF_RANGE,
} TetoResult;

namespace TetoDatetime {
    struct Time {
        int seconds = 0;
        int minutes = 0;
        int hours = 0;
    };

    struct Date {
        int day = 1;
        int month = 1;
        int year = 0;

        static inline int getDayCount(int month);
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

        static inline int checkTime(Time time);
        static inline int checkDate(Date date);

        inline Time time();
        inline void setTime(Time time);
        inline Date date();
        inline void setDate(Date date);

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