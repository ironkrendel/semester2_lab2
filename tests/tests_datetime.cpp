#include <datetime.hpp>
#include <colors.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockDatetime : public TetoDatetime::Datetime {
private:
    int seconds;
    int minutes;
    int hours;
    int day;
    int month;
    int year;
public:
    [[nodiscard]]
    inline auto time() const -> TetoDatetime::Time{return TetoDatetime::Time{seconds, minutes, hours};};
    [[nodiscard]]
    inline auto date() const -> TetoDatetime::Date{return TetoDatetime::Date{day, month, year};};

    static auto getDatetime(std::time_t timestamp) -> Datetime {
        std::time_t nowTime = timestamp;
        std::tm time{};
        localtime_r(&nowTime, &time);
        if (time.tm_mon == 1 && time.tm_mday == 29) {
            std::cerr << ANSI_COLOR_RED << "Ignoring leap year according to specification and setting day value to 28!" << ANSI_COLOR_RESET << std::endl;
            time.tm_mday = 28;
        }
        return Datetime(time.tm_sec, time.tm_min, time.tm_hour, time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);
    }

    void syncDatetime(std::time_t timestamp) {
        std::time_t nowTime = timestamp;
        std::tm time{};
        localtime_r(&nowTime, &time);
        if (time.tm_mon == 1 && time.tm_mday == 29) {
            std::cerr << ANSI_COLOR_RED << "Ignoring leap year according to specification and setting day value to 28!" << ANSI_COLOR_RESET << std::endl;
            time.tm_mday = 28;
        }
        seconds = time.tm_sec;
        minutes = time.tm_min;
        hours = time.tm_hour;
        day = time.tm_mday;
        month = time.tm_mon + 1;
        year = time.tm_year + 1900;
    }
};

TEST(ConstructorTests, DefaultConstructor) {
    TetoDatetime::Time time{TetoDatetime::DEFAULT_VALUE_SECONDS, TetoDatetime::DEFAULT_VALUE_MINUTES, TetoDatetime::DEFAULT_VALUE_HOURS};
    TetoDatetime::Date date{TetoDatetime::DEFAULT_VALUE_DAY, TetoDatetime::DEFAULT_VALUE_MONTH, TetoDatetime::DEFAULT_VALUE_YEAR};
    TetoDatetime::Datetime dt;

    ASSERT_EQ(time.toString(), dt.time().toString());
    ASSERT_EQ(date.toString(), dt.date().toString());
}

TEST(ConstructorTests, StructConstructor) {
    TetoDatetime::Time time{0, 0, 0};
    TetoDatetime::Date date{1, 1, 2000};
    TetoDatetime::Datetime dt(time, date);

    ASSERT_EQ(time.toString(), dt.time().toString());
    ASSERT_EQ(date.toString(), dt.date().toString());
}

TEST(ConstructorTests, IntConstructor) {
    TetoDatetime::Time time{0, 0, 0};
    TetoDatetime::Date date{1, 1, 2000};
    TetoDatetime::Datetime dt(time.seconds, time.minutes, time.hours, date.day, date.month, date.year);

    ASSERT_EQ(time.toString(), dt.time().toString());
    ASSERT_EQ(date.toString(), dt.date().toString());
}

TEST(ConstructorTests, ConstructorInvalidInput) {
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{60, 0, 0}, TetoDatetime::Date{1, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{-42, 0, 0}, TetoDatetime::Date{1, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 60, 0}, TetoDatetime::Date{1, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, -42, 0}, TetoDatetime::Date{1, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 24}, TetoDatetime::Date{1, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, -42}, TetoDatetime::Date{1, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{32, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{0, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{-42, 1, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{30, 2, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 13, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 0, 1900}));
    ASSERT_ANY_THROW(TetoDatetime::Datetime(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, -42, 1900}));
}

TEST(GetterSetterTests, GettersSetters) {
    TetoDatetime::Datetime dt;

    dt.setSeconds(5);
    ASSERT_EQ(dt.getSeconds(), 5);
    dt.setMinutes(5);
    ASSERT_EQ(dt.getMinutes(), 5);
    dt.setHours(5);
    ASSERT_EQ(dt.getHours(), 5);
    dt.setDay(5);
    ASSERT_EQ(dt.getDay(), 5);
    dt.setMonth(5);
    ASSERT_EQ(dt.getMonth(), 5);
    dt.setYear(5);
    ASSERT_EQ(dt.getYear(), 5);
    dt.setTime(TetoDatetime::Time{2, 2, 2});
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{2, 2, 2}).toString());
    dt.setDate(TetoDatetime::Date{1, 1, 1900});
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 1900}).toString());
}

TEST(GetterSetterTests, GettersSettersInvalidInput) {
    TetoDatetime::Datetime dt;
    dt.setMonth(1);

    ASSERT_ANY_THROW(dt.setSeconds(60));
    ASSERT_ANY_THROW(dt.setSeconds(-42));
    ASSERT_ANY_THROW(dt.setMinutes(60));
    ASSERT_ANY_THROW(dt.setMinutes(-42));
    ASSERT_ANY_THROW(dt.setHours(24));
    ASSERT_ANY_THROW(dt.setHours(-42));
    ASSERT_ANY_THROW(dt.setDay(32));
    ASSERT_ANY_THROW(dt.setDay(0));
    ASSERT_ANY_THROW(dt.setDay(-42));
    ASSERT_ANY_THROW(dt.setMonth(13));
    ASSERT_ANY_THROW(dt.setMonth(0));
    ASSERT_ANY_THROW(dt.setMonth(-42));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{32, 1, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{29, 2, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{31, 4, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{0, 1, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{-5, 1, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{1, 13, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{1, 0, 1900}));
    ASSERT_ANY_THROW(dt.setDate(TetoDatetime::Date{1, -5, 1900}));
}

TEST(CopyMoveOperatorsTests, Copy) {
    TetoDatetime::Datetime dt1;
    TetoDatetime::Datetime dt2(dt1);

    ASSERT_EQ(dt1.getTimestamp(), dt2.getTimestamp());
}

TEST(CopyMoveOperatorsTests, Move) {
    TetoDatetime::Datetime dt1;
    TetoDatetime::Datetime dt2(std::move(dt1));

    ASSERT_EQ(dt1.getTimestamp(), dt2.getTimestamp());
}

TEST(CopyMoveOperatorsTests, OperatorCopy) {
    TetoDatetime::Datetime dt1;
    TetoDatetime::Datetime dt2 = dt1;

    ASSERT_EQ(dt1.getTimestamp(), dt2.getTimestamp());
}

TEST(CopyMoveOperatorsTests, OperatorMove) {
    TetoDatetime::Datetime dt1;
    TetoDatetime::Datetime dt2 = dt1;

    ASSERT_EQ(dt1.getTimestamp(), dt2.getTimestamp());
}

TEST(TimeDateValidatorsTests, TimeValidator) {
    // seconds
    TetoDatetime::Time time{0, 0, 0};
    for (int i = 0;i < 60;i++) {
        time.seconds = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_SUCCESS);
    }
    for (int i = -10;i < 0;i++) {
        time.seconds = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_ERROR_TIME_OUT_OF_RANGE);
    }
    for (int i = 60;i < 70;i++) {
        time.seconds = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_ERROR_TIME_OUT_OF_RANGE);
    }
    // minutes
    time = {0, 0, 0};
    for (int i = 0;i < 60;i++) {
        time.minutes = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_SUCCESS);
    }
    for (int i = -10;i < 0;i++) {
        time.minutes = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_ERROR_TIME_OUT_OF_RANGE);
    }
    for (int i = 60;i < 70;i++) {
        time.minutes = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_ERROR_TIME_OUT_OF_RANGE);
    }
    // hours
    time = {0, 0, 0};
    for (int i = 0;i < 24;i++) {
        time.hours = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_SUCCESS);
    }
    for (int i = -10;i < 0;i++) {
        time.hours = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_ERROR_TIME_OUT_OF_RANGE);
    }
    for (int i = 24;i < 30;i++) {
        time.hours = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkTime(time), TETO_ERROR_TIME_OUT_OF_RANGE);
    }
}

TEST(TimeDateValidatorsTests, DateValidator) {
    // day
    TetoDatetime::Date date{1, 1, 1900};
    for (int i = 1;i < 32;i++) {
        date.day = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_SUCCESS);
    }
    for (int i = -10;i < 1;i++) {
        date.day = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_ERROR_DATE_OUT_OF_RANGE);
    }
    for (int i = 32;i < 40;i++) {
        date.day = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_ERROR_DATE_OUT_OF_RANGE);
    }
    // month
    date = {1, 1, 1900};
    for (int i = 1;i < 13;i++) {
        date.month = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_SUCCESS);
    }
    for (int i = -10;i < 1;i++) {
        date.month = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_ERROR_DATE_OUT_OF_RANGE);
    }
    for (int i = 13;i < 20;i++) {
        date.month = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_ERROR_DATE_OUT_OF_RANGE);
    }
    // wrong day, month combination
    date = {1, 1, 1900};
    for (int i = 1;i < 13;i++) {
        date.day = TetoDatetime::Date::getDayCount(i) + 1;
        date.month = i;
        ASSERT_EQ(TetoDatetime::Datetime::checkDate(date), TETO_ERROR_DATE_OUT_OF_RANGE);
    }
}

TEST(ClockSyncTests, SystemTimeConversionTest) {
    // jan 01 2024 gmt+3
    TetoDatetime::Datetime dt = MockDatetime::getDatetime(1704067200);
    // jan 01 2024 gmt+3
    TetoDatetime::Datetime dt2(TetoDatetime::Time{0, 0, 3}, TetoDatetime::Date{1, 1, 2024});
    
    ASSERT_EQ(dt.date().toString(), dt2.date().toString());
}

TEST(ClockSyncTests, SystemTimeConversionFeb29Test) {
    // feb 29 2024 gmt+3
    TetoDatetime::Datetime dt = MockDatetime::getDatetime(1709164800);
    // feb 28 2024 gmt+3
    TetoDatetime::Datetime dt2 = MockDatetime::getDatetime(1709078400);
    
    ASSERT_EQ(dt.date().toString(), dt2.date().toString());
}

TEST(ClockSyncTests, SystemTimeSyncTest) {
    // jan 01 2024 gmt+3
    MockDatetime dt;
    dt.syncDatetime(1704067200);
    // jan 01 2024 gmt+3
    TetoDatetime::Datetime dt2(TetoDatetime::Time{0, 0, 3}, TetoDatetime::Date{1, 1, 2024});
    
    ASSERT_EQ(dt.date().toString(), dt2.date().toString());
}

TEST(ClockSyncTests, SystemTimeSyncFeb29Test) {
    // feb 29 2024 gmt+3
    MockDatetime dt;
    dt.syncDatetime(1709164800);
    // feb 28 2024 gmt+3
    MockDatetime dt2;
    dt2.syncDatetime(1709078400);
    
    ASSERT_EQ(dt.date().toString(), dt2.date().toString());
}

TEST(TimeDiffTests, SecsToTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});
    TetoDatetime::Datetime dt2(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{2, 1, 2024});

    ASSERT_EQ(dt.secsTo(dt2), 60 * 60 * 24);
    dt2.setDay(31);
    ASSERT_EQ(dt.secsTo(dt2), 60 * 60 * 24 * 30);
    dt2.setMonth(2);
    dt2.setDay(5);
    ASSERT_EQ(dt.secsTo(dt2), 60 * 60 * 24 * 35);
    dt2.setDay(1);
    dt2.setMonth(1);
    dt2.setTime(TetoDatetime::Time{30, 0, 0});
    ASSERT_EQ(dt.secsTo(dt2), 30);
    dt2.setTime(TetoDatetime::Time{0, 30, 0});
    ASSERT_EQ(dt.secsTo(dt2), 60 * 30);
    dt2.setTime(TetoDatetime::Time{0, 0, 5});
    ASSERT_EQ(dt.secsTo(dt2), 60 * 60 * 5);
}

TEST(TimeDiffTests, DaysToTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});
    TetoDatetime::Datetime dt2(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{2, 1, 2024});

    ASSERT_EQ(dt.daysTo(dt2), 1);
    dt2.setDay(31);
    ASSERT_EQ(dt.daysTo(dt2), 30);
    dt2.setMonth(2);
    dt2.setDay(5);
    ASSERT_EQ(dt.daysTo(dt2), 35);
}

TEST(AddTimeTests, AddSecondsTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});

    dt.addSeconds(30);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{30, 0, 0}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 0});
    dt.addSeconds(60);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 1, 0}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 0});
    dt.addSeconds(90);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{30, 1, 0}).toString());
    dt.setTime(TetoDatetime::Time{30, 0, 0});
    dt.addSeconds(-30);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
    dt.setTime(TetoDatetime::Time{30, 1, 0});
    dt.addSeconds(-90);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
}

TEST(AddTimeTests, AddMinutesTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});

    dt.addMinutes(30);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 30, 0}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 0});
    dt.addMinutes(60);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 1}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 0});
    dt.addMinutes(90);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 30, 1}).toString());
    dt.setTime(TetoDatetime::Time{0, 30, 0});
    dt.addMinutes(-30);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
    dt.setTime(TetoDatetime::Time{0, 30, 1});
    dt.addMinutes(-90);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
}

TEST(AddTimeTests, AddHoursTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});

    dt.addHours(6);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 6}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 0});
    dt.addHours(24);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{2, 1, 2024}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 0});
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addHours(30);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 6}).toString());
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{2, 1, 2024}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 5});
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addHours(-5);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
    dt.setTime(TetoDatetime::Time{0, 0, 6});
    dt.setDate(TetoDatetime::Date{2, 1, 2024});
    dt.addHours(-30);
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{0, 0, 0}).toString());
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 2024}).toString());
}

TEST(AddTimeTests, AddDaysTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});

    dt.addDays(5);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{6, 1, 2024}).toString());
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addDays(31);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 2, 2024}).toString());
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addDays(31 + 28);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 3, 2024}).toString());
    dt.setDate(TetoDatetime::Date{2, 1, 2024});
    dt.addDays(-1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 2024}).toString());
    dt.setDate(TetoDatetime::Date{2, 2, 2024});
    dt.addDays(-32);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 2024}).toString());
}

TEST(AddTimeTests, AddMonthsTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});

    dt.addMonths(1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 2, 2024}).toString());
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addMonths(12);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 2025}).toString());
    dt.setDate(TetoDatetime::Date{1, 2, 2024});
    dt.addMonths(-1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 2024}).toString());
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addMonths(-6);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 7, 2023}).toString());
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addMonths(-12);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{1, 1, 2023}).toString());
#ifdef DISCARD_DAYS
    dt.setDate(TetoDatetime::Date{31, 1, 2024});
    dt.addMonths(1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{28, 2, 2024}).toString());
    dt.setDate(TetoDatetime::Date{31, 3, 2024});
    dt.addMonths(-1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{28, 2, 2024}).toString());
#else
    dt.setDate(TetoDatetime::Date{31, 1, 2024});
    dt.addMonths(1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{3, 3, 2024}).toString());
    dt.setDate(TetoDatetime::Date{31, 3, 2024});
    dt.addMonths(-1);
    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{25, 2, 2024}).toString());
#endif
}

TEST(AddTimeTests, AddYearsTest) {
    TetoDatetime::Datetime dt(TetoDatetime::Time{0, 0, 0}, TetoDatetime::Date{1, 1, 2024});

    dt.addYears(1);
    ASSERT_EQ(dt.getYear(), 2025);
    dt.setDate(TetoDatetime::Date{1, 1, 2024});
    dt.addYears(-1);
    ASSERT_EQ(dt.getYear(), 2023);
}

TEST(StringToDatetimeTests, StringToDatetimeTest) {
    TetoDatetime::Datetime dt = TetoDatetime::Datetime::stringToDatetime("%h:%m:%s-%D/%M/%Y", "5:31:24-31/5/2024");

    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{31, 5, 2024}).toString());
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{24, 31, 5}).toString());

    dt = TetoDatetime::Datetime::stringToDatetime("%h %m %s %D %M %Y", "5 31 24 31 5 2024");

    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{31, 5, 2024}).toString());
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{24, 31, 5}).toString());

    dt = TetoDatetime::Datetime::stringToDatetime("%hTE0104TO%m %s %D %M %Y", "5TE0104TO31 24 31 5 2024");

    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{31, 5, 2024}).toString());
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{24, 31, 5}).toString());

    ASSERT_ANY_THROW(TetoDatetime::Datetime::stringToDatetime("%h %m %s %D %M %Y", "5 31 24 31 2 2024"));

    ASSERT_ANY_THROW(TetoDatetime::Datetime::stringToDatetime("%h %m %s %D %M %Y", "-5 31 24 31 1 2024"));

    dt = TetoDatetime::Datetime::stringToDatetime("%hTE0104TO%m %s %D %M %Y", "5TE0104TO31 24 31 5 -2024");

    ASSERT_EQ(dt.date().toString(), (TetoDatetime::Date{31, 5, -2024}).toString());
    ASSERT_EQ(dt.time().toString(), (TetoDatetime::Time{24, 31, 5}).toString());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    // testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}