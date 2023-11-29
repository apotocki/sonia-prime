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
#if 0
    if (is_array(b).is_array || b.type == blob_type::unspecified) {
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
#endif
    switch (b.type) {
    case blob_type::nil:
        lua_pushnil(L); break;
    case blob_type::boolean:
        lua_pushboolean(L, (int)b.bp.ui8value); break;
    case blob_type::i8:
    case blob_type::ui8:
    case blob_type::i16:
    case blob_type::ui16:
    case blob_type::i32:
    case blob_type::ui32:
    case blob_type::i64:
        lua_pushinteger(L, from_blob<lua_Integer>{}(b)); break;
    case blob_type::ui64:
        if (uint64_t val = from_blob<lua_Integer>{}(b); val <= (uint64_t)(std::numeric_limits<int64_t>::max)()) {
            lua_pushinteger(L, (lua_Integer)val);
        } else {
            push_variant(L, b);
        }
        break;
    case blob_type::c8:
        lua_pushlstring(L, (const char*)b.bp.data, 1); break;
    case blob_type::flt16:
    case blob_type::flt32:
    case blob_type::flt64:
        lua_pushnumber(L, from_blob<lua_Number>{}(b)); break;
    case blob_type::function:
        lua_pushlstring(L, data_of<char>(b), array_size_of<char>(b));
        lua_pushcclosure(L, ext_invoke, 1);
        //lua_pushcfunction(L, ext_invoke); 
        break;
    case blob_type::string:
        lua_pushlstring(L, data_of<char>(b), array_size_of<char>(b)); break;
    case blob_type::error:
        throw exception(std::string(data_of<char>(b), array_size_of<char>(b)));
    default:
        if (is_array(b)) {
            push_variant(L, b);
        } else {
            lua_pushnil(L);
        }
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
    if (!is_array(*br)) { // scalar
        if (index == 1) { // push itself
            return push_variant(L, *br);
        }
        lua_pushnil(L);
    }
    blob_type_selector(*br, [L, c_index = index - 1](auto ident, blob_result b) {
        using type = typename decltype(ident)::type;
        if constexpr (std::is_void_v<type>) { lua_pushnil(L); return; }
        else {
            using fstype = std::conditional_t<std::is_same_v<type, bool>, uint8_t, type>;

            fstype const* begin_ptr = data_of<fstype>(b);
            size_t sz = array_size_of<fstype>(b);

            if (c_index < 0 || sz <= (size_t)c_index) {
                lua_pushnil(L);
            } else {
                if constexpr (is_same_v<fstype, blob_result>) {
                    push_from_blob(L, *(begin_ptr + c_index));
                } else if constexpr (is_integral_v<fstype>) {
                    fstype const* pval = begin_ptr + c_index;
                    lua_pushinteger(L, (lua_Integer)*pval);
                } else if constexpr (is_floating_point_v<fstype>) {
                    fstype const* pval = begin_ptr + c_index;
                    lua_pushnumber(L, (lua_Number)*pval);
                } else if (is_same_v<fstype, float16>) {
                    fstype const* pval = begin_ptr + c_index;
                    lua_pushnumber(L, (lua_Number)(float)*pval);
                } else {
                    throw exception("unsupported type %1%"_fmt % typeid(fstype).name());
                }
            }
        }
    });
    return 1;
}

int variant_len(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");

    if (is_array(*br)) {
        lua_Integer count = blob_type_selector(*br, [](auto ident, blob_result b) -> size_t {
            using type = typename decltype(ident)::type;
            if constexpr (std::is_void_v<type>) { return 0; }
            else {
                using fstype = std::conditional_t<std::is_same_v<type, bool>, uint8_t, type>;
                return array_size_of<fstype>(b);
            }
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
    blob_result br = make_blob_result( blob_type_for<T>() | blob_type::is_array, nullptr, static_cast<int32_t>(argcount * sizeof(T)));
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
            ival = as<uint64_t>(*pbr);
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
    if (is_array(b) && !contains_string(b)) {
        os << "[";
        blob_type_selector(b, [&os, &printer](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            if constexpr (std::is_void_v<type>) { os << "unknown"; }
            else {
                using fstype = std::conditional_t<std::is_same_v<type, bool>, uint8_t, type>;
                fstype const* begin_ptr = data_of<fstype>(b);
                for (auto* p = begin_ptr, *e = begin_ptr + array_size_of<fstype>(b); p != e; ++p) {
                    os << ((p != begin_ptr) ? "," : "");
                    fancy_print(os, particular_blob_result((type)*p), printer);
                }
            }
        });
        return os << "]";
    }
    switch (b.type)
    {
    case blob_type::nil:
        return os << "nil";
    case blob_type::boolean:
        return printer(os, b.type, !!b.bp.i8value);
    case blob_type::i8:
        return printer(os, b.type, b.bp.i8value);
    case blob_type::ui8:
        return printer(os, b.type, b.bp.ui8value);
    case blob_type::i16:
        return printer(os, b.type, b.bp.i16value);
    case blob_type::ui16:
        return printer(os, b.type, b.bp.ui16value);
    case blob_type::i32:
        return printer(os, b.type, b.bp.i32value);
    case blob_type::ui32:
        return printer(os, b.type, b.bp.ui32value);
    case blob_type::i64:
        return printer(os, b.type, b.bp.i64value);
    case blob_type::ui64:
        return printer(os, b.type, b.bp.ui64value);
    case blob_type::flt16:
        return printer(os, b.type, b.bp.f16value);
    case blob_type::flt32:
        return printer(os, b.type, b.bp.f32value);
    case blob_type::flt64:
        return printer(os, b.type, b.bp.f64value);
    case blob_type::string:
        return printer(os, b.type, sonia::string_view{ data_of<char>(b), array_size_of<char>(b) });
    case blob_type::function:
        return os << "function";
    case blob_type::object:
        return os << "object";
    case blob_type::error:
        return os << "error: " << sonia::string_view{ data_of<char>(b), array_size_of<char>(b) };
    default:
        return os << "unknown";
    }
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

// blobs in sp are supposed already pinned
template <typename T>
blob_result to_blob_array(blob_type elemtype, span<const blob_result> sp)
{
    blob_result values = make_blob_result(elemtype | blob_type::is_array, nullptr, (int32_t)(sp.size() * sizeof(T)));
    if constexpr (!is_same_v<blob_result, T>) {
        if (sp.size() == 1) {
            *reinterpret_cast<T*>(values.ui8array) = as<T>(sp[0]);
            values.inplace_size = static_cast<uint8_t>(sizeof(T));
            return values;
        }
    }

    blob_result_allocate(&values);
    T* pobj = mutable_data_of<T>(values);
    for (size_t i = 0; i < sp.size(); ++i) {
        *(pobj + i) = as<T>(sp[i]);
        // already pinned
        //if constexpr(std::is_same_v<T, blob_result>) {
        //    blob_result_pin(pobj);
        //}
    }

    return values;
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
            blob_type ktype = blob_type::nil, vtype = blob_type::nil;
            bool keys_are_array_indices = false, no_unique_v = false;
            bool is_basic_integral_v = false, has_negative_v = false, has_i64unlimit_value_v = false;

            std::vector<blob_result> vals;
            std::vector<blob_result> keys;
            while (lua_next(L, -2) != 0) {
                keys.push_back(to_blob(L, -2));
                vals.push_back(to_blob(L, -1)); //i64, f64, bool, string, tuple
                lua_pop(L, 1);
                if (ktype == blob_type::nil) {
                    ktype = keys.back().type;
                    keys_are_array_indices = ktype == blob_type::i64 && as<int64_t>(keys.back()) == 1;
                } else if (ktype != keys.back().type) {
                    keys_are_array_indices = false;
                } else if (keys_are_array_indices) {
                    keys_are_array_indices = ktype == blob_type::i64 && as<int64_t>(keys.back()) == (int64_t)keys.size();
                }
                if (vtype == blob_type::nil) {
                    vtype = vals.back().type;
                    is_basic_integral_v = is_basic_integral(vtype);
                    if (is_basic_integral_v) {
                        has_negative_v = (vtype != blob_type::ui64) && as<int64_t>(vals.back()) < 0;
                        has_i64unlimit_value_v = (vtype == blob_type::ui64) && as<uint64_t>(vals.back()) > (uint64_t)(std::numeric_limits<int64_t>::max)();
                    }
                } else if (no_unique_v || vtype != vals.back().type) {
                    no_unique_v = true;
                    vtype = vals.back().type;
                    is_basic_integral_v = is_basic_integral_v && is_basic_integral(vtype);
                    if (is_basic_integral_v) {
                        has_negative_v = has_negative_v || (vtype != blob_type::ui64) && as<int64_t>(vals.back()) < 0;
                        has_i64unlimit_value_v = has_i64unlimit_value_v || (vtype == blob_type::ui64) && as<uint64_t>(vals.back()) > (uint64_t)(std::numeric_limits<int64_t>::max)();
                    }
                }
            }
            lua_pop(L, 1);
            if (keys.empty()) return nil_blob_result();

            smart_blob blob_values;
            if (is_basic_integral_v && !has_i64unlimit_value_v) {
                blob_values = to_blob_array<int64_t>(blob_type::i64, vals);
            } else if (is_basic_integral_v && !has_negative_v) {
                blob_values = to_blob_array<uint64_t>(blob_type::ui64, vals);
            } else if (!no_unique_v && vtype == blob_type::flt64) {
                blob_values = to_blob_array<double_t>(vtype, vals);
            } else {
                blob_values = to_blob_array<blob_result>(blob_type::tuple, vals);
            }

            if (keys_are_array_indices) {
                return blob_values.detach();
            }
            
            smart_blob result = make_blob_result(blob_type::tuple, nullptr, (uint32_t)(2 * sizeof(blob_result)));
            result.allocate();

            smart_blob blob_keys = to_blob_array<blob_result>(blob_type::tuple, keys);
            
            *reinterpret_cast<blob_result*>(const_cast<void*>(result.data())) = blob_keys.detach();
            *(reinterpret_cast<blob_result*>(const_cast<void*>(result.data())) + 1) = blob_values.detach();
            return result.detach();
        }

        case LUA_TUSERDATA:
            //GLOBAL_LOG_INFO() << "to_blob from userdata";
            if (blob_result* br = luaL_test_variant_lib(L, index); br) {
                //GLOBAL_LOG_INFO() << "to_blob from userdata found variant!";
                blob_result_pin(br);
                return *br;
            }
            GLOBAL_LOG_WARN() << "to_blob from userdata, unknown userdata";
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
            blob_result br = make_blob_result(blob_type::ui8 | blob_type::is_array, sp.data(), static_cast<uint32_t>(sp.size()));
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
