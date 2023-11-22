//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "variant_lib.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/invokation/value_encoder.hpp"
#include "sonia/utility/invokation/value_decoder.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/datetime/iso_parser.hpp"

#include <sstream>

#include <boost/multiprecision/cpp_int.hpp>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h> 

}

#define VARIANT_METATABLE_NAME "sonia.variant"
#define VARIANT_LIB_NAME "variant"

namespace sonia::lua {

static const char context_key = '_';

int ext_index(lua_State* L)
{
    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->resolve_global();
}

int ext_newindex(lua_State* L)
{
    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->set_global();
}

void setup_ext(lua_State* L, language* inv)
{
    lua_pushlightuserdata(L, (void*)&context_key);
    lua_pushlightuserdata(L, inv);
    lua_settable(L, LUA_REGISTRYINDEX);

    luaL_Reg const metamethods[] = {
        { "__index", ext_index },
        { "__newindex", ext_newindex },
        { NULL, NULL }
    };

    lua_pushglobaltable(L);
    luaL_newlib(L, metamethods);
    lua_setmetatable(L, -2);
}

int ext_invoke(lua_State* L)
{
    auto fname_index = lua_upvalueindex(1);
    char const* strval = lua_tostring(L, fname_index);
    size_t sz = lua_rawlen(L, fname_index);
    string_view fname{ strval, sz };

    int argcount = lua_gettop(L);
    std::vector<blob_result> args;
    for (int i = 0; i < argcount; ++i) {
        args.push_back(to_blob(L, i + 1));
    }
    SCOPE_EXIT([&args] {
        for (auto arg : args) blob_result_unpin(&arg);
    });

    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->invoke_global(fname, args);
}

blob_result* luaL_check_variant_lib(lua_State* L, int index)
{
    return (blob_result*)luaL_checkudata(L, index, VARIANT_METATABLE_NAME);
}

blob_result* luaL_test_variant_lib(lua_State* L, int index)
{
    return (blob_result*)luaL_testudata(L, index, VARIANT_METATABLE_NAME);
}

int variant_gc(lua_State* L)
{
    blob_result* br = (blob_result*)lua_touserdata(L, 1);
    if (br) {
        blob_result_unpin(br);
    }
    return 0;
}

int push_variant(lua_State* L, blob_result const& value)
{
    blob_result* br = (blob_result*)lua_newuserdata(L, sizeof(blob_result));
    *br = value;
    blob_result_pin(br);

    luaL_getmetatable(L, VARIANT_METATABLE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

void push_from_blob(lua_State* L, blob_result const& b)
{
    if (b.type == blob_type::unspecified && !b.data) {
        lua_pushnil(L);
        return;
    }
    if (b.is_array || b.type == blob_type::unspecified) {
        push_variant(L, b);
        /*
        lua_newtable(L);
        blob_result_type_selector(b, [L, this](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            for (int32_t i = 0; i < b.size / sizeof(fstype); ++i) {
                lua_pushinteger(L, (lua_Integer)(i+1));
                if constexpr (is_same_v<fstype, blob_result>) {
                    push_from_blob(*(reinterpret_cast<blob_result const*>(b.data) + i));
                } else if constexpr (is_integral_v<fstype>) {
                    fstype const* pval = reinterpret_cast<fstype const*>(b.data) + i;
                    lua_pushinteger(L, (lua_Integer)*pval);
                } else {
                    fstype const* pval = reinterpret_cast<fstype const*>(b.data) + i;
                    lua_pushnumber(L, (lua_Number)*pval);
                }
                lua_settable(L, -3);
            }
        });
        */
        return;
    }
    switch (b.type) {
    case blob_type::boolean:
        lua_pushboolean(L, (int)b.ui8value);
        return;

    case blob_type::unspecified:
    case blob_type::i8:
        lua_pushinteger(L, (lua_Integer)b.i8value); return;
    case blob_type::ui8:
        lua_pushinteger(L, (lua_Integer)b.ui8value); return;
    case blob_type::i16:
        lua_pushinteger(L, (lua_Integer)b.i16value); return;
    case blob_type::ui16:
        lua_pushinteger(L, (lua_Integer)b.ui16value); return;
    case blob_type::i32:
        lua_pushinteger(L, (lua_Integer)b.i32value); return;
    case blob_type::ui32:
        lua_pushinteger(L, (lua_Integer)b.ui32value); return;
    case blob_type::i64:
        lua_pushinteger(L, (lua_Integer)b.i64value); return;
    case blob_type::ui64:
        if (b.ui64value > (uint64_t)(std::numeric_limits<int64_t>::max)()) {
            push_variant(L, b);
        } else {
            lua_pushinteger(L, (lua_Integer)b.ui64value);
        }
        return;
    case blob_type::flt16:
        lua_pushnumber(L, (lua_Number)(float_t)b.f16value); return;
    case blob_type::flt32:
        lua_pushnumber(L, (lua_Number)b.f32value); return;
    case blob_type::flt64:
        lua_pushnumber(L, (lua_Number)b.f64value); return;
    case blob_type::string:
        lua_pushlstring(L, (const char*)b.data, b.size); return;
        //case blob_type::blob:
    case blob_type::function:
        lua_pushlstring(L, (const char*)b.data, b.size);
        lua_pushcclosure(L, ext_invoke, 1);
        //lua_pushcfunction(L, ext_invoke); 
        return;
    case blob_type::error:
        throw exception(std::string((const char*)b.data, b.size));
    default:
        lua_pushnil(L);
    }
}

int variant_index(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");

    luaL_getmetatable(L, VARIANT_METATABLE_NAME);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    if (!lua_isnil(L, -1)) return 1;

    auto index = luaL_checkinteger(L, 2);
    if (!br->is_array) { // scalar
        if (index == 1) { // push itself
            return push_variant(L, *br);
        }
        lua_pushnil(L);
    }
    blob_result_type_selector(*br, [L, c_index = index - 1](auto ident, blob_result b) {
        using type = typename decltype(ident)::type;
        using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
        using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;

        if (c_index < 0 || b.size / sizeof(fstype) <= (size_t)c_index) {
            lua_pushnil(L);
        } else {
            if constexpr (is_same_v<fstype, blob_result>) {
                push_from_blob(L, *(reinterpret_cast<blob_result const*>(b.data) + c_index));
            } else if constexpr (is_integral_v<fstype>) {
                fstype const* pval = reinterpret_cast<fstype const*>(b.data) + c_index;
                lua_pushinteger(L, (lua_Integer)*pval);
            } else if constexpr (is_floating_point_v<fstype>) {
                fstype const* pval = reinterpret_cast<fstype const*>(b.data) + c_index;
                lua_pushnumber(L, (lua_Number)*pval);
            } else if (is_same_v<fstype, float16>) {
                fstype const* pval = reinterpret_cast<fstype const*>(b.data) + c_index;
                lua_pushnumber(L, (lua_Number)(float)*pval);
            } else {
                throw exception("unsupported type %1%"_fmt % typeid(fstype).name());
            }
        }
    });
    return 1;
}

int variant_len(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");

    if (br->is_array) {
        lua_Integer count = blob_result_type_selector(*br, [](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            return b.size / sizeof(fstype);
        });
        lua_pushinteger(L, count);
    } else {
        return luaL_error(L, "is not an array");
    }
    return 1;
}

int variant_equal(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    blob_result* br2 = luaL_check_variant_lib(L, 2);
    lua_pushboolean(L, br && br2 && *br == *br2);
    return 1;
}

int variant_tostring(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' is expected");
    std::ostringstream s;
    s << *br;
    std::string result = s.str();
    lua_pushfstring(L, "variant(%s)", result.c_str());
    return 1;
}

template <std::integral T>
int variant_array(lua_State* L)
{
    int argcount = lua_gettop(L);
    blob_result br{ nullptr, static_cast<int32_t>(argcount * sizeof(T)), 0, 1, blob_type_for<T>() };
    blob_result_allocate(&br);
    defer{ blob_result_unpin(&br); };
    T* arr_data = reinterpret_cast<T*>(const_cast<void*>(br.data));
    for (int i = 0; i < argcount; ++i) {
        auto elemval = luaL_checkinteger(L, i + 1);
        bool check = (std::numeric_limits<T>::min)() <= elemval && (std::numeric_limits<T>::max)() >= elemval;
        luaL_argcheck(L, check, i + 1, "invalid value");
        arr_data[i] = static_cast<T>(elemval);
    }
    return push_variant(L, br);
}

int variant_int(lua_State* L)
{
    if (lua_isinteger(L, 1)) {
        lua_Integer i = lua_tointeger(L, 1);
        lua_pushinteger(L, i); // we use native lua integer if equivalent
        //return push_variant(L, i64_blob_result(i));
    } else if (lua_isstring(L, 1)) {
        char const* strval = lua_tostring(L, 1);
        size_t sz = lua_rawlen(L, 1);
        using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;
        integer_type ival(string_view{ strval, sz });
        if (ival <= (std::numeric_limits<int64_t>::max)() && ival >= (std::numeric_limits<int64_t>::min)()) {
            lua_pushinteger(L, ival.convert_to<lua_Integer>()); // we use native lua integer if equivalent
            //return push_variant(L, i64_blob_result(ival.convert_to<int64_t>()));
        } else if (ival >= 0 && ival <= (std::numeric_limits<uint64_t>::max)()) {
            return push_variant(L, ui64_blob_result(ival.convert_to<uint64_t>()));
        } else {
            return luaL_error(L, "invalid argument");
        }
    } else {
        return luaL_error(L, "invalid argument");
    }
    return 1;
}

int variant_type(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");
    std::ostringstream s;
    print_type(s, *br);
    std::string result = s.str();
    lua_pushfstring(L, "%s", result.c_str());
    return 1;
}

int variant_parse_float(lua_State* L)
{
    size_t sz;
    const char * cstrval = luaL_checklstring(L, 1, &sz);
    while (sz && std::isspace(cstrval[sz - 1])) --sz;
    char * pend;
    double dval = strtod (cstrval, &pend);
    if (pend != cstrval + sz) {
        return luaL_error(L, "float parse error");
    }
    lua_pushnumber(L, dval);
    return 1;
}

// iso_date -> java, winfile, unix, dosdate, dostime, //mac, oletime
int variant_iso_date(lua_State* L)
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

//(datetime type, val)->string
int variant_datetime_string(lua_State* L)
{
    size_t strsz;
    const char* cstrval = luaL_checklstring(L, 1, &strsz);
    
    string_view dttype{ cstrval, strsz };
    std::string result;

    if (dttype == "java"sv) { // milliseconds since 1970-01-01T00:00:00Z
        auto ival = luaL_checkinteger(L, 2);
        using tag_t = basic_datetime_tag<int64_t, 1000>;
        tag_t::datetime_type dt{ ival };
        result = tag_t::iso_datetime(dt);
    } else if(dttype == "winfile"sv) { // 100 nanoseconds intervals since 1601-01-01T00:00:00Z
        uint64_t ival;
        if (lua_isinteger(L, 2)) {
            auto argival = luaL_checkinteger(L, 2);
            luaL_argcheck(L, argival >= 0, 1, "must be not negative");
            ival = static_cast<uint64_t>(argival);
        } else {
            blob_result* pbr = luaL_check_variant_lib(L, 2);
            luaL_argcheck(L, !!pbr && pbr->type == blob_type::ui64, 1, "`variant.ui64' or integer expected");
            ival = pbr->ui64value;
        }
        using tag_t = basic_datetime_tag<uint64_t, 10000000, 12591158400LL>;
        tag_t::datetime_type dt{ ival };
        result = tag_t::iso_datetime(dt);
    } else if (dttype == "unix"sv) { // seconds since 1970-01-01T00:00:00Z
        auto ival = luaL_checkinteger(L, 2);
        using tag_t = basic_datetime_tag<int64_t, 1>;
        tag_t::datetime_type dt{ ival };
        result = tag_t::iso_datetime(dt);
    } else if (dttype == "dosdate"sv) { // seconds since 1970-01-01T00:00:00Z
        auto ival = luaL_checkinteger(L, 2);
        luaL_argcheck(L, ival >= 0 && ival < 65536, 2, "wrong value");
        uint32_t day = ival & 31;
        uint32_t month = (ival >> 5) & 15;
        uint64_t year = 1980 + (ival >> 9);

        //using tag_t = basic_datetime_tag<int64_t, 1>;
        //tag_t::datetime_type dt = tag_t::construct(year, month, day);
        //result = tag_t::iso_datetime(dt, false);
        
        std::ostringstream s;
        s << std::setfill('0') << year << "-" << std::setw(2) << month << "-" << std::setw(2) << day;
        result = s.str();

    } else if (dttype == "dostime"sv) { // seconds since 1970-01-01T00:00:00Z
        auto ival = luaL_checkinteger(L, 2);
        luaL_argcheck(L, ival >= 0 && ival < 65536, 2, "wrong value");
        uint32_t seconds = (ival & 31) * 2;
        uint32_t minutes = (ival >> 5) & 63;
        uint32_t hours = (ival >> 11) & 31;

        std::ostringstream s;
        s << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
        result = s.str();
        //using tag_t = basic_datetime_tag<int64_t, 1>;
        //tag_t::datetime_type dt = tag_t::construct_time(hours, minutes, seconds);
        //result = tag_t::iso_time(dt);
    } else {
        return luaL_error(L, "unknown datetime type: %s", cstrval);
    }
    lua_pushlstring(L, result.c_str(), result.size());
    return 1;
}

template <typename PrinterT>
std::ostream& fancy_print(std::ostream& os, blob_result const& b, PrinterT const& printer)
{
    if (b.type == blob_type::unspecified && !b.data) {
        return os << "nil";
    }
    if (b.is_array) {
        os << "[";
        blob_result_type_selector(b, [&os, &printer](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            for (int32_t i = 0; i < b.size / sizeof(fstype); ++i) {
                os << (i ? "," : "");
                fancy_print(os, particular_blob_result(*(reinterpret_cast<fstype const*>(b.data) + i)), printer);
            }
        });
        return os << "]";
    }
    switch (b.type)
    {
    case blob_type::unspecified:
        if (!b.data) return os << "nil";
        return os << "unspecified";
    case blob_type::boolean:
        return printer(os, b.type, !!b.i8value);
    case blob_type::i8:
        return printer(os, b.type, b.i8value);
    case blob_type::ui8:
        return printer(os, b.type, b.ui8value);
    case blob_type::i16:
        return printer(os, b.type, b.i16value);
    case blob_type::ui16:
        return printer(os, b.type, b.ui16value);
    case blob_type::i32:
        return printer(os, b.type, b.i32value);
    case blob_type::ui32:
        return printer(os, b.type, b.ui32value);
    case blob_type::i64:
        return printer(os, b.type, b.i64value);
    case blob_type::ui64:
        return printer(os, b.type, b.ui64value);
    case blob_type::flt16:
        return printer(os, b.type, b.f16value);
    case blob_type::flt32:
        return printer(os, b.type, b.f32value);
    case blob_type::flt64:
        return printer(os, b.type, b.f64value);
    case blob_type::string:
        return printer(os, b.type, sonia::string_view(reinterpret_cast<const char*>(b.data), b.size));
    case blob_type::blob:
        return os << "blob";
    case blob_type::function:
        return os << "function";
    case blob_type::object:
        return os << "object";
    case blob_type::error:
        return os << "error: " << sonia::string_view(reinterpret_cast<const char*>(b.data), b.size);
    }
    return os;
}

int variant_fancy_string(lua_State* L)
{
    blob_result br = nil_blob_result();
    if (lua_isinteger(L, 1)) {
        br = i64_blob_result(luaL_checkinteger(L, 1));
    } else {
        blob_result* pbr = luaL_check_variant_lib(L, 1);
        luaL_argcheck(L, !!pbr, 1, "`variant' or integer is expected");
        br = *pbr;
    }

    lua_Integer radix = 10;
    lua_Integer groupSz = 0;
    string_view delim = " "sv;
    int argcount = lua_gettop(L);
    if (argcount > 1) {
        radix = luaL_checkinteger(L, 2);
        if (argcount > 2) {
            groupSz = luaL_checkinteger(L, 3);
            if (argcount > 3) {
                size_t sz;
                char const* strval = luaL_checklstring(L, 4, &sz);
                delim = string_view{ strval, sz };
            }
        }
    }
    
    // formatting
    std::ostringstream ss;

    fancy_print(ss, br, [radix, groupSz, delim](std::ostream& s, blob_type bt, auto value) -> std::ostream& {
        using type = decltype(value);
        if constexpr (std::is_same_v<type, bool>) {
            s << (value ? "true"sv : "false"sv);
        } else if constexpr (std::is_integral_v<type>) {
            std::ostringstream tempss;
            if (radix == 16) {
                if (value < 0) s << '-';
                s << "0x"sv;
                
                tempss << std::hex << std::uppercase;
                tempss << (value >= 0 ? uint64_t(value) : uint64_t(1-(value + 1)));
            } else {
                tempss << value;
            }

            std::string grouped_val = [groupSz, delim](std::string && val) -> std::string {
                if (!groupSz) return std::move(val);
                std::string result;
                for (size_t i = 0, sz = val.size(); i < sz; ++i) {
                    result.push_back(val[i]);
                    if (size_t pos = sz - i - 1; pos && 0 == pos % groupSz) {
                        std::ranges::copy(delim, std::back_inserter(result));
                        //std::copy(delim.begin(), delim.end(), std::back_inserter(result));
                    }
                }
                return result;
            }(tempss.str());

            s << grouped_val;
        } else {
            s << value; // ?string
        }
        return s;
    });
    std::string result = ss.str();
    lua_pushlstring(L, result.c_str(), result.size());
    return 1;
}

blob_result to_blob(lua_State* L, int index)
{
    int luavaltype = lua_type(L, index);
    switch (luavaltype) {
        case LUA_TNUMBER:
            if (lua_isinteger(L, index)) {
                lua_Integer i = lua_tointeger(L, index);
                return i64_blob_result(i);
            } else {
                lua_Number i = lua_tonumber(L, index);
                return f64_blob_result(i);
            }

        case LUA_TBOOLEAN:
        {
            int bval = lua_toboolean(L, index);
            return bool_blob_result(!!bval);
        }

        case LUA_TSTRING:
        {
            char const* strval = lua_tostring(L, index);
            size_t sz = lua_rawlen(L, index);
            auto result = string_blob_result(string_view(strval, sz));
            blob_result_allocate(&result);
            return result;
        }

        case LUA_TTABLE:
        {
            lua_pushvalue(L, index);
            lua_pushnil(L);
            blob_type ktype = blob_type::unspecified, vtype = blob_type::unspecified;
            bool no_unique_k = false, no_unique_v = false;
            std::vector<blob_result> vals;
            std::vector<blob_result> keys;
            while (lua_next(L, -2) != 0) {
                keys.push_back(to_blob(L, -2));
                vals.push_back(to_blob(L, -1));
                lua_pop(L, 1);
                if (ktype == blob_type::unspecified) {
                    ktype = keys.back().type;
                } else if (ktype != keys.back().type) {
                    no_unique_k = true;
                }
                if (vtype == blob_type::unspecified) {
                    vtype = vals.back().type;
                } else if (vtype != vals.back().type) {
                    no_unique_v = true;
                }
            }
            lua_pop(L, 1);
            if (keys.empty()) return nil_blob_result();

            if (no_unique_v) vtype = blob_type::blob;
            smart_blob blob_values;
            if (vtype == blob_type::flt64) {
                blob_values = blob_result{ nullptr, (int32_t)(vals.size() * sizeof(double_t)), 0, 1, vtype };
                blob_values.allocate();
                for (size_t i = 0; i < vals.size(); ++i) {
                    *(reinterpret_cast<double_t*>(const_cast<void*>(blob_values->data)) + i) = vals[i].f64value;
                }
            } else if (vtype == blob_type::i64) {
                blob_values = blob_result{ nullptr, (int32_t)(vals.size() * sizeof(int64_t)), 0, 1, vtype };
                blob_values.allocate();
                for (size_t i = 0; i < vals.size(); ++i) {
                    *(reinterpret_cast<int64_t*>(const_cast<void*>(blob_values->data)) + i) = vals[i].i64value;
                }
            } else if (vtype == blob_type::string || vtype == blob_type::blob) {
                blob_values = blob_result{ nullptr, (int32_t)(vals.size() * sizeof(blob_result)), 0, 1, vtype };
                blob_values.allocate();
                for (size_t i = 0; i < vals.size(); ++i) {
                    *(reinterpret_cast<blob_result*>(const_cast<void*>(blob_values->data)) + i) = vals[i];
                }
            } else {
                // not implemented for now
                for (blob_result v : vals) blob_result_unpin(&v);
            }

            // check if array
            if (!no_unique_k && ktype == blob_type::i64) {
                bool is_array = true;
                for (int i = 0; i < keys.size(); ++i) {
                    if (auto kval = keys[i].i64value; kval != (i+1)) {
                        //if (float fval = keys[i].i64value.floatvalue; fmod(fval, 1.0) > 0.00001 || ((int)fval) != (i+1)) {
                        is_array = false;
                        break;
                    }
                }
                if (is_array) {
                    return blob_values.detach();
                }
            }

            smart_blob result = blob_result{nullptr, (int32_t)(2 * sizeof(blob_result)), 0, 1, blob_type::blob};
            result.allocate();
            smart_blob blob_keys = blob_result{nullptr, (int32_t)(keys.size() * sizeof(blob_result)), 0, 1, blob_type::blob};
            blob_keys.allocate();

            for (size_t i = 0; i < keys.size(); ++i) {
                *(reinterpret_cast<blob_result*>(const_cast<void*>(blob_keys->data)) + i) = keys[i];
            }
            *reinterpret_cast<blob_result*>(const_cast<void*>(result->data)) = blob_keys.detach();
            *(reinterpret_cast<blob_result*>(const_cast<void*>(result->data)) + 1) = blob_values.detach();
            return result.detach();
        }

        case LUA_TUSERDATA:
            if (blob_result* br = luaL_test_variant_lib(L, index); br) {
                blob_result_pin(br);
                return *br;
            }

        case LUA_TNIL:
        default:
            return nil_blob_result();
    }
}

// arguments: bytearray: variant, type: string, endianness: string
// types: i8, ui8, i16, ui16, i32, ui32, i64, ui64
// endianness: le, be
int variant_decode(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");

    span<const uint8_t> sp;
    try {
        sp = as<span<const uint8_t>>(*br);
    } catch (std::exception const&) {
        return luaL_argerror(L, 1, "byte array expected");
    }
    
    size_t typestrsz;
    const char* typestr = luaL_checklstring(L, 2, &typestrsz);
    
    string_view endianness;
    int argcount = lua_gettop(L);
    if (argcount > 2) {
        size_t estrsz;
        const char* estr = luaL_checklstring(L, 3, &estrsz);
        endianness = string_view{ estr, estrsz };
    } else {
        endianness = std::endian::native == std::endian::little ? "le"sv : "be"sv;
    }

    blob_result res;
    try {
        res = as_singleton<invokation::value_decoder>()->decode(sp, { typestr, typestrsz }, endianness);
    } catch (std::exception const& e) {
        return luaL_error(L, e.what());
    }

    push_from_blob(L, res);
    return 1;
}

int variant_encode(lua_State* L)
{
    blob_result input;
    if (lua_isinteger(L, 1)) {
        input = i64_blob_result(lua_tointeger(L, 1));
    } else {
        blob_result *br = luaL_check_variant_lib(L, 1);
        luaL_argcheck(L, !!br, 1, "`variant' or integer expected");
        input = *br;
    }

    size_t typestrsz;
    const char* typestr = luaL_checklstring(L, 2, &typestrsz);
    
    string_view endianness;
    int argcount = lua_gettop(L);
    if (argcount > 2) {
        size_t estrsz;
        const char* estr = luaL_checklstring(L, 3, &estrsz);
        endianness = string_view{ estr, estrsz };
    } else {
        endianness = std::endian::native == std::endian::little ? "le"sv : "be"sv;
    }

    try {
        as_singleton<invokation::value_encoder>()->encode(input, { typestr, typestrsz }, endianness, [L](span<const uint8_t> sp) {
            blob_result br{ sp.data(), static_cast<int32_t>(sp.size()), 0, 1, blob_type::ui8 };
            blob_result_allocate(&br);
            defer{ blob_result_unpin(&br); };
            push_variant(L, br);
        });
    } catch (std::exception const& e) {
        return luaL_error(L, e.what());
    }

    return 1;
}

const struct luaL_Reg variantlib[] = {
    {"ui8array", variant_array<uint8_t>},
    {"i8array", variant_array<int8_t>},
    {"int", variant_int},
    {"f64", variant_parse_float},
    {"iso_date", variant_iso_date},
    {"to_fancy_string", variant_fancy_string},
    {"to_datetime_string", variant_datetime_string},
    {"encode", variant_encode},
    {NULL, NULL}
};

const struct luaL_Reg variantlib_m[] = {
    {"__tostring", variant_tostring},
    {"__index", variant_index},
    {"__len", variant_len},
    {"__eq", variant_equal},
    {"__gc", variant_gc},
    {NULL, NULL}
};

const struct luaL_Reg variantlib_f[] = {
    {"type", variant_type},
    {"to_fancy_string", variant_fancy_string},
    {"decode", variant_decode},
    {NULL, NULL}
};

int luaopen_variantlib(lua_State* L)
{
#ifdef MERGE_VARIANT_LIB // hasn't tested
    lua_getglobal(L, VARIANT_LIB_NAME);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        luaL_newmetatable(L, VARIANT_METATABLE_NAME);
        lua_newtable(L);
    }
#else
    luaL_newmetatable(L, VARIANT_METATABLE_NAME);
    luaL_setfuncs(L, variantlib_m, 0);
    luaL_setfuncs(L, variantlib_f, 0);
    lua_pop(L, 1);

    lua_newtable(L);
#endif
    luaL_setfuncs(L, variantlib, 0);
    lua_setglobal(L, VARIANT_LIB_NAME);

    return 1;
}

}
