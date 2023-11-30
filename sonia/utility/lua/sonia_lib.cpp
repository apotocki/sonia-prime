//  Sonia//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia_lib.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/variant.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/datetime/iso_parser.hpp"

#include "variant_lib.hpp"
#include "bigint_lib.hpp"

extern "C" {

#include <lauxlib.h>
#include <lualib.h> 

}

#define SONIA_LIB_NAME "sonia"

namespace sonia::lua {

template <std::integral T>
int variant_array(lua_State* L)
{
    int argcount = lua_gettop(L);
    blob_result br = make_blob_result(arrayify(blob_type_for<T>()), nullptr, static_cast<int32_t>(argcount * sizeof(T)));
    blob_result_allocate(&br);
    defer{ blob_result_unpin(&br); };
    T* arr_data = mutable_data_of<T>(br);
    for (int i = 0; i < argcount; ++i) {
        auto elemval = luaL_checkinteger(L, i + 1);
        bool check = (std::numeric_limits<T>::min)() <= elemval && (std::numeric_limits<T>::max)() >= elemval;
        luaL_argcheck(L, check, i + 1, "invalid value");
        arr_data[i] = static_cast<T>(elemval);
    }
    return push_variant(L, br);
}

// iso_date -> java, winfile, unix, dosdate, dostime, //mac, oletime
int sonialib_iso_datetime(lua_State* L)
{
    size_t strsz;
    const char* cstrval = luaL_checklstring(L, 1, &strsz);
    
    while (strsz && std::isspace(cstrval[strsz - 1])) --strsz;
    const char* ecstrval = cstrval + strsz;

    size_t destsz;
    const char* cstrdest = luaL_checklstring(L, 2, &destsz);
    
    string_view dttype{ cstrdest, destsz };
    if (dttype == "java"sv) {
        using tag_t = basic_datetime_tag<int64_t, 1000>;
        tag_t::datetime_type result;
        parsers::datetime::iso_parser<true, tag_t>::do_parse(cstrval, ecstrval, result);
        lua_pushinteger(L, result.ticks());
    } else if (dttype == "winfile"sv) {
        using tag_t = basic_datetime_tag<uint64_t, 10000000, 12591158400LL>;
        tag_t::datetime_type result;
        parsers::datetime::iso_parser<true, tag_t>::do_parse(cstrval, ecstrval, result);
        push_from_blob(L, ui64_blob_result(result.ticks()));
    } else if (dttype == "unix"sv) {
        using tag_t = basic_datetime_tag<int64_t, 1>;
        tag_t::datetime_type result;
        parsers::datetime::iso_parser<true, tag_t>::do_parse(cstrval, ecstrval, result);
        lua_pushinteger(L, result.ticks());
    } else if (dttype == "dosdate"sv) {
        using tag_t = basic_datetime_tag<int64_t, 1>;
        tag_t::datetime_type result;
        if (!parsers::datetime::iso_parser<true, tag_t>::do_parse(cstrval, ecstrval, result)) {
            return luaL_error(L, "wrong date string");
        }
        auto year = tag_t::year(result);
        if (year < 1980 || year > 2107) return luaL_error(L, "year is out of bounds");
        auto resultval = tag_t::month_day(result) + (tag_t::month(result) << 5) + ((year - 1980) << 9);
        lua_pushinteger(L, resultval);
    } else if (dttype == "dostime"sv) {
        using tag_t = basic_datetime_tag<int64_t, 1>;
        unsigned int hours, minutes, seconds;
        if (!parsers::datetime::iso_parser<true, tag_t>::do_parse_time(cstrval, ecstrval, hours, minutes, seconds)) {
            return luaL_error(L, "wrong time string");
        }
        auto resultval = seconds / 2 + (minutes << 5) + (hours << 11);
        lua_pushinteger(L, resultval);
    } else {
        return luaL_error(L, "unknown datetime type: %s", cstrdest);
    }

    if (ecstrval != cstrval) {
        return luaL_error(L, "datetime parse error");
    }
    
    return 1;
}

template <typename T>
struct caster : static_visitor<T>
{
    caster() = default;
    template <typename T>
    T operator()(T const& val) const { return (T)val; }
};

template <uint64_t TicksPerSecV, int64_t EpochTo2000secondsV = 946684800LL>
struct iso_datetime_printer : static_visitor<std::string>
{
    iso_datetime_printer() = default;

    template <typename T>
    std::string operator()(T const& val) const
    {
        using tag_t = basic_datetime_tag<T, TicksPerSecV, EpochTo2000secondsV>;
        return this->operator()(typename tag_t::datetime_type{ val });
    }

    template <typename T>
    std::string operator()(datetime<T> const& val) const
    {
        using tag_t = basic_datetime_tag<T, TicksPerSecV, EpochTo2000secondsV>;
        return tag_t::iso_datetime(val);
    }
};

template <uint64_t TicksPerSecV, int64_t EpochTo2000secondsV = 946684800LL>
struct iso_date_printer : static_visitor<std::string>
{
    iso_date_printer() = default;

    template <typename T>
    std::string operator()(T const& val) const
    {
        using tag_t = basic_datetime_tag<T, TicksPerSecV, EpochTo2000secondsV>;
        return this->operator()(typename tag_t::datetime_type{ val });
    }

    template <typename T>
    std::string operator()(datetime<T> const& val) const
    {
        using tag_t = basic_datetime_tag<T, TicksPerSecV, EpochTo2000secondsV>;
        std::ostringstream s;
        s << std::setfill('0') << tag_t::year(val) << "-" << std::setw(2) << tag_t::month(val) << "-" << std::setw(2) << tag_t::month_day(val);
        return s.str();
    }
};

template <uint64_t TicksPerSecV, int64_t EpochTo2000secondsV = 946684800LL>
struct iso_time_printer : static_visitor<std::string>
{
    iso_time_printer() = default;

    template <typename T>
    std::string operator()(T const& val) const
    {
        using tag_t = basic_datetime_tag<T, TicksPerSecV, EpochTo2000secondsV>;
        return this->operator()(typename tag_t::datetime_type{ val });
    }

    template <typename T>
    std::string operator()(datetime<T> const& val) const
    {
        using tag_t = basic_datetime_tag<T, TicksPerSecV, EpochTo2000secondsV>;
        std::ostringstream s;
        s << std::setfill('0') << std::setw(2) << tag_t::hour(val) << ":" << std::setw(2) << tag_t::minute(val) << ":" << std::setw(2) << tag_t::second(val);
        return s.str();
   }
};

//(val, datetime type)->string
template <template <auto...> class printer>
int sonialib_iso_datetime_string(lua_State* L)
{
    size_t strsz;
    const char* cstrval = luaL_checklstring(L, 2, &strsz);
    
    string_view dttype{ cstrval, strsz };
    std::string result;

    variant<lua_Integer, integer_type> arg;
    if (lua_isinteger(L, 1)) {
        arg = luaL_checkinteger(L, 1);
    } else if (bigint_header* pbh = luaL_test_bigint_lib(L, 1); pbh) {
        integer_type ival;
        restore_bigint(pbh, ival);
        arg = std::move(ival);
    } else {
        return luaL_argerror(L, 1, "integer or `bigint' expected");
    }
    if (dttype == "java"sv) { // milliseconds since 1970-01-01T00:00:00Z
        result = apply_visitor(printer<1000>{}, arg);
    } else if(dttype == "winfile"sv) { // 100 nanoseconds intervals since 1601-01-01T00:00:00Z
        result = apply_visitor(printer<10000000, 12591158400LL>{}, arg);
    } else if (dttype == "unix"sv) { // seconds since 1970-01-01T00:00:00Z
        result = apply_visitor(printer<1>{}, arg);
    } else if (dttype == "dosdate"sv) { // seconds since 1970-01-01T00:00:00Z
        integer_type ival = apply_visitor(caster<integer_type>{}, arg);
        unsigned int day = (unsigned int)(ival & 31);
        unsigned int month = (unsigned int)((ival >> 5) & 15);
        int64_t year = (int64_t)(1980 + (ival >> 9));
        result = printer<1>{}(basic_datetime_tag<uint64_t, 1>::construct(year, month, day));
    } else if (dttype == "dostime"sv) { // seconds since 1970-01-01T00:00:00Z
        integer_type ival = apply_visitor(caster<integer_type>{}, arg);
        luaL_argcheck(L, ival >= 0 && ival < 65536, 1, "wrong value");
        unsigned int seconds = (unsigned int)((ival & 31) * 2);
        unsigned int minutes = (unsigned int)((ival >> 5) & 63);
        uint32_t hours = (uint32_t)((ival >> 11) & 31);

        //result = printer<1>{}(basic_datetime_tag<uint64_t, 1>::construct(1970, 1, 1, hours, minutes, seconds));
        std::ostringstream s;
        s << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
        result = s.str();
        
    } else {
        return luaL_error(L, "unknown datetime type: %s", cstrval);
    }
    lua_pushlstring(L, result.c_str(), result.size());
    return 1;
}

const struct luaL_Reg sonialib[] = {
    {"ui8array", variant_array<uint8_t>},
    {"i8array", variant_array<int8_t>},
    {"iso_datetime", sonialib_iso_datetime},
    {"to_iso_datetime_string", sonialib_iso_datetime_string<iso_datetime_printer>},
    {"to_iso_date_string", sonialib_iso_datetime_string<iso_date_printer>},
    {"to_time_string", sonialib_iso_datetime_string<iso_time_printer>},
    /*
    {"create", bigint_create},
    {"to_datetime_string", variant_datetime_string},
    {"encode", variant_encode},
    */
    {NULL, NULL}
};

int luaopen_sonialib(lua_State* L)
{
    lua_newtable(L);

    luaL_setfuncs(L, sonialib, 0);
    lua_setglobal(L, SONIA_LIB_NAME);

    return 1;
}

}
