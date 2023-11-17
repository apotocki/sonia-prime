//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <time.h>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>

#include "sonia/cstdint.hpp"

#include "datetime.hpp"

namespace sonia {

// datetime stores ticks since Jan 1 1970 00:00:00 GMT
template <typename TickStorageT = int64_t, uint64_t TicksPerSecV = 1000000>
struct basic_datetime_tag
{
    using datetime_type = datetime<TickStorageT>;

    static unsigned int month_days_[2][12];
    static unsigned int year_days_[2][12];

    static constexpr int64_t ticks_per_second = TicksPerSecV;
    
    static constexpr int64_t seconds_in_400greg_years = 12622780800l;
    static constexpr int64_t seconds_in_100greg_years = 3155673600l;
    static constexpr int64_t seconds_in_100greg_years_leap = 3155760000l;
    static constexpr int64_t seconds_in_4greg_years = 126144000l;
    static constexpr int64_t seconds_in_4greg_years_leap = 126230400l;
    static constexpr int64_t seconds_in_1greg_year = 31536000;
    static constexpr int64_t seconds_in_1greg_year_leap = 31622400;

    static constexpr int64_t seconds_1970_2000 = 946684800;
    static constexpr int64_t seconds_per_day = 86400;

    static constexpr int64_t min_year = -10010; //dt::year(datetime<>::min_value()); // TODO: FIXME!

    static int64_t to_java(datetime_type const& val) noexcept
    {
        return val.ticks() * 1000 / ticks_per_second;
    }

    static datetime_type from_java(int64_t ticks) noexcept
    {
        return datetime_type(ticks * ticks_per_second / 1000 );
    }

    datetime_type static now() noexcept
    {
        return datetime_type(static_cast<int64_t>(time(0)) * ticks_per_second);
    }

    static bool is_leapyear(int64_t year) noexcept { return (!(year % 4) && ((year % 100) || !(year % 400))); }

    static unsigned int month_day_count(int64_t year, unsigned int month)
    {
        return month_days_[is_leapyear(year) ? 1 : 0][month - 1];
    }

    static unsigned int year_day_count(int64_t year, unsigned int month)
    {
        return year_days_[is_leapyear(year) ? 1 : 0][month - 1];
    }

    static uint64_t day_ticks(datetime_type const& val) noexcept
    {
        int64_t dts = val.ticks() % (seconds_per_day * ticks_per_second);
        if (dts < 0) dts += seconds_per_day * ticks_per_second;
        return static_cast<uint64_t>(dts);
    }

    static unsigned int day_second(datetime_type const& val) noexcept
    {
        return static_cast<unsigned int>(day_ticks(val) / ticks_per_second);
    }

    static auto fraqsecond(datetime_type const& val) noexcept
    {
        return day_ticks(val) % ticks_per_second;
    }

    static unsigned int second(datetime_type const& val) noexcept
    {
        return static_cast<unsigned int>(day_second(val) % 60);
    }

    static unsigned int minute(datetime_type const& val) noexcept
    {
        return static_cast<unsigned int>((day_second(val) % 3600) / 60);
    }

    static unsigned int hour(datetime_type const& val) noexcept
    {
        return static_cast<unsigned int>(day_second(val) / 3600);
    }

    static datetime_type date_only(datetime_type const& val) noexcept
    {
        return datetime_type(val.ticks() - day_second(val) * ticks_per_second);
    }

    static datetime_type time_only(datetime_type const& val) noexcept
    {
        return datetime_type(day_second(val));
    }

    static unsigned int weekday(datetime_type const& val) noexcept
    {
        return static_cast<unsigned int>((val.ticks() / ticks_per_second) / seconds_per_day);
    }

    static int64_t year(datetime_type const& val, unsigned int & yday) noexcept
    {
        int64_t result_year = 2000;
        int64_t secs = (val.ticks() / ticks_per_second) - seconds_1970_2000;

        result_year += secs / seconds_in_400greg_years * 400;
        secs = secs % seconds_in_400greg_years;

        bool first100_from_400 = secs < seconds_in_100greg_years_leap;
        if (!first100_from_400) {
            secs -= seconds_in_100greg_years_leap;
            result_year += 100;
            result_year += secs / seconds_in_100greg_years * 100;
            secs = secs % seconds_in_100greg_years;
        } else if (secs < -3 * seconds_in_100greg_years) {
            result_year -= 300;
            secs += 3 * seconds_in_100greg_years;
        } else {
            result_year += secs / seconds_in_100greg_years * 100;
            secs = secs % seconds_in_100greg_years;
        }

        bool first4_from_100 = secs < seconds_in_4greg_years;
        if (!first4_from_100 && !first100_from_400) {
            secs -= seconds_in_4greg_years;
            result_year += 4;
        }
        result_year += secs / seconds_in_4greg_years_leap * 4;
        secs = secs % seconds_in_4greg_years_leap;

        if (secs >= seconds_in_1greg_year_leap && (first100_from_400 || !first4_from_100)) {
            secs -= seconds_in_1greg_year_leap;
            ++result_year;
            result_year += secs / seconds_in_1greg_year;
            secs = secs % seconds_in_1greg_year;
        } else if (secs < -3 * seconds_in_1greg_year) {
            result_year -= 3;
            secs += 3 * seconds_in_1greg_year;
        } else {
            result_year += secs / seconds_in_1greg_year;
            secs = secs % seconds_in_1greg_year;
        }

        if (secs < 0) {
            --result_year;
            yday = static_cast<unsigned int>((is_leapyear(result_year) ? 366 : 365) + ((secs + 1) / seconds_per_day));
        } else {
            yday = 1 + static_cast<unsigned int>(secs / seconds_per_day);
        }

        return result_year;
    }

    static int64_t year(datetime_type const& val) noexcept
    {
        unsigned int yday;
        return year(val, yday);
    }

    static unsigned int year_day(datetime_type const& val) noexcept
    {
        unsigned int yday;
        year(val, yday);
        return yday;
    }

    static unsigned int month(datetime_type const& val, int64_t& yr, unsigned int & mday) noexcept
    {
        unsigned int m = 0;
        yr = year(val, mday);
        bool il = is_leapyear(yr);
    
        while (mday > month_days_[il ? 1 : 0][m]) {
            mday -= month_days_[il ? 1 : 0][m];
            m++;
        }
        return m + 1;
    }

    static unsigned int month(datetime_type const& val) noexcept
    {
        int64_t yr;
        unsigned int mday;
        return month(val, yr, mday);
    }

    static unsigned int month_day(datetime_type const& val) noexcept
    {
        int64_t yr;
        unsigned int mday;
        month(val, yr, mday);
        return mday;
    }

    template <typename SecondT = unsigned int>
    static datetime_type construct(int64_t year, unsigned int month = 1, unsigned int day = 1, unsigned int hours = 0, unsigned int minutes = 0, SecondT seconds = 0) noexcept
    {
        TickStorageT secs = 0; // since 2000 year

        int64_t dyr = year - 2000;
        secs += (dyr / 400) * seconds_in_400greg_years;
        dyr = dyr % 400;

        bool first100_from_400 = dyr < 100;
        if (!first100_from_400) {
            dyr -= 100; secs += seconds_in_100greg_years_leap;
        }
        secs += (dyr / 100) * seconds_in_100greg_years;
        dyr = dyr % 100;
    
        bool first4_from_100 = dyr < 4;
        if (!first4_from_100 && !first100_from_400) {
            dyr -= 4; secs += seconds_in_4greg_years;
        }
        secs += (dyr / 4) * seconds_in_4greg_years_leap;
        dyr = dyr % 4;

        if (dyr > 0 && (first100_from_400 || !first4_from_100)) {
            --dyr; secs += seconds_in_1greg_year_leap;
        }
        secs += dyr * seconds_in_1greg_year;

        // apply month/day/hours/minutes/seconds
        secs += (year_day_count(year, month) + day - 1) * seconds_per_day + 3600 * hours + 60 * minutes;

        // fix base year
        TickStorageT result_secs = secs + seconds_1970_2000;

        return datetime_type(result_secs * ticks_per_second + static_cast<int64_t>(round(seconds * ticks_per_second)));
    }

    static datetime_type construct_month(unsigned int month)
    {
        return construct(min_year + 2, month, 1, 0, 0, 0);
    }

    static datetime_type construct_monthday(unsigned int month, unsigned int day)
    {
        return construct(min_year + 2, month, day, 0, 0, 0);
    }

    static datetime_type construct_day(unsigned int day)
    {
        return construct(min_year + 1, 1, day, 0, 0, 0);
    }

    template <typename SecondT>
    static datetime_type construct_time(unsigned int hours, unsigned int minutes, SecondT seconds)
    {
        return construct(min_year, 1, 1, hours, minutes, seconds);
    }

    static bool check_date(int64_t year, unsigned int month, unsigned int day)
    {
        return month > 0 && month <= 12 && day > 0 && month_days_[is_leapyear(year) ? 1 : 0][month - 1] >= day;
    }

    static bool check_monthday(unsigned int month, unsigned int day)
    {
        return month > 0 && month <= 12 && day > 0 && month_days_[1][month - 1] >= day;
    }

    static std::string iso_date(datetime_type const& val)
    {
        // Preallocating buffer
        std::string buf;
        buf.reserve(30);
        std::ostringstream ss(std::move(buf));
    
        int64_t yr;
        unsigned int mday;
        const auto m = month(val, yr, mday);
    
        if (yr < 0) {
            ss << '-';
        }

        ss << std::setfill('0');
        if (yr < 1000 && yr > -1000) {
            ss << std::setw(4);
        }

        ss << (std::abs)(yr) << "-" << std::setw(2) << m << "-" << std::setw(2) << mday << "T" << std::setw(2) << hour(val) << ":" << std::setw(2) << minute(val) << ":" << std::setw(2) << second(val);
        if (auto fs = fraqsecond(val); fs) {
            int fraqwidth = (int)log10(TicksPerSecV);
            for (; !(fs % 10); fs /= 10, --fraqwidth);
            ss << "." << std::setw(fraqwidth) << fs;
        }
        ss << "Z";
        return ss.str();
    }
};

template <typename StorageT, uint64_t TicksPerSecV>
unsigned int basic_datetime_tag<StorageT, TicksPerSecV>::month_days_[2][12] = 
    {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

template <typename StorageT, uint64_t TicksPerSecV>
unsigned int basic_datetime_tag<StorageT, TicksPerSecV>::year_days_[2][12] = 
    {{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}, {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}};
}

/*
BOOST_AUTO_TEST_CASE( gregorian_calendar_test )
{
    using tag = basic_datetime_tag<>;
    auto now = tag::now();
    std::cout << tag::iso_date(now);

    unsigned int hour = 0, minute = 0, second = 0;
    unsigned int hourh = 23, minuteh = 59, secondh = 59;
    for (int64_t baseyear = 1970, dyear = 0; dyear < 100000; ++dyear)
    {
        for (unsigned int month = 1; month < 12; ++month)
        {
            int64_t year = baseyear + dyear;
            for (unsigned int day = 1; day < tag::month_day_count(year, month); ++day)
            {
                auto dt = tag::construct(year, month, day, hour, minute, second);
                if (tag::year(dt) != year || tag::month(dt) != month || tag::month_day(dt) != day || tag::hour(dt) != hour || tag::minute(dt) != minute || tag::second(dt) != second)
                {
                    std::cout << tag::iso_date(dt) << ", original: " << year << "-" << month << "-" << day << "T" << hour << ":" << minute << ":" << second << "\n";
                    tag::construct(year, month, day, hour, minute, second);
                }
                dt = tag::construct(year, month, day, hourh, minuteh, secondh);
                if (tag::year(dt) != year || tag::month(dt) != month || tag::month_day(dt) != day || tag::hour(dt) != hourh || tag::minute(dt) != minuteh || tag::second(dt) != secondh)
                {
                    std::cout << tag::iso_date(dt) << ", original: " << year << "-" << month << "-" << day << "T" << hourh << ":" << minuteh << ":" << secondh << "\n";
                    tag::construct(year, month, day, hour, minute, second);
                }
            }
            year = baseyear - dyear;
            for (unsigned int day = 1; day < tag::month_day_count(year, month); ++day)
            {
                auto dt = tag::construct(year, month, day, hour, minute, second);
                if (tag::year(dt) != year || tag::month(dt) != month || tag::month_day(dt) != day || tag::hour(dt) != hour || tag::minute(dt) != minute || tag::second(dt) != second)
                {
                    std::cout << tag::iso_date(dt) << ", original: " << year << "-" << month << "-" << day << "T" << hour << ":" << minute << ":" << second << "\n";
                    tag::construct(year, month, day, hour, minute, second);
                }
                dt = tag::construct(year, month, day, hourh, minuteh, secondh);
                if (tag::year(dt) != year || tag::month(dt) != month || tag::month_day(dt) != day || tag::hour(dt) != hourh || tag::minute(dt) != minuteh || tag::second(dt) != secondh)
                {
                    std::cout << tag::iso_date(dt) << ", original: " << year << "-" << month << "-" << day << "T" << hourh << ":" << minuteh << ":" << secondh << "\n";
                    tag::construct(year, month, day, hour, minute, second);
                }
            }
        }
        if (dyear % 100 == 0) {
            std::cout << "done: " << (baseyear - dyear) << " - " << (baseyear + dyear) << "\n";
        }
    }
}
*/
