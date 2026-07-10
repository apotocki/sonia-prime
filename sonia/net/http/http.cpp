//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <utility>
#include <string>
#include <ctime>

namespace sonia::http {
    
std::string http_date(tm* tm)
{
    char buff[256];
    const char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
        "Aug", "Sep", "Oct", "Nov", "Dec" };

    snprintf(buff, sizeof(buff), "%s, %d %s %d %02d:%02d:%02d GMT",
        days[tm->tm_wday], tm->tm_mday, months[tm->tm_mon],
        tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);

    return std::string(buff);
}

}
