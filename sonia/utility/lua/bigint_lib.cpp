//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bigint_lib.hpp"

#include "sonia/string.hpp"

extern "C" {

#include <lauxlib.h>
#include <lualib.h> 

}

#define BIGINT_METATABLE_NAME "sonia.bigint"
#define BIGINT_LIB_NAME "bigint"

#include <boost/multiprecision/cpp_int.hpp>

namespace sonia::lua {

using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;

bigint_header* luaL_check_bigint_lib(lua_State* L, int index)
{
    return (bigint_header*)luaL_checkudata(L, index, BIGINT_METATABLE_NAME);
}

bigint_header* luaL_test_bigint_lib(lua_State* L, int index)
{
    return (bigint_header*)luaL_testudata(L, index, BIGINT_METATABLE_NAME);
}

int bigint_index(lua_State* L)
{
    bigint_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");

    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    return 1;
}

int push_bigint(lua_State* L, mp::basic_integer_view<const limb_type> value)
{
    auto limbs = (span<const limb_type>)value;
    size_t datasz = limbs.size() * sizeof(limb_type);
    bigint_header* br = (bigint_header*)lua_newuserdata(L, sizeof(bigint_header) + datasz);
    br->size = limbs.size();
    br->sign = value.sign() > 0 ? 0 : 1;
    memcpy(br + 1, limbs.data(), datasz);

    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

//bigint_header* push_bigint(lua_State* L, size_t limb_count)
//{
//    size_t datasz = limb_count * sizeof(limb_type);
//    bigint_header* br = (bigint_header*)lua_newuserdata(L, sizeof(bigint_header) + datasz);
//    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
//    lua_setmetatable(L, -2);
//}

template <std::integral LimbT>
struct lua_limbs_allocator
{
    lua_State* L_;
    //bigint_header* br;

    using value_type = LimbT;

    explicit lua_limbs_allocator(lua_State* L) : L_{L} {} //, br{nullptr} {}

    value_type* allocate(size_t cnt)
    {
        size_t datasz = cnt * sizeof(limb_type);
        bigint_header* br = (bigint_header*)lua_newuserdata(L_, sizeof(bigint_header) + datasz);
        return reinterpret_cast<value_type*>(br + 1);
    }

    void deallocate(value_type*, size_t) { /* do nothing, lua will take care */ }
};

int push_bigint(lua_State* L, uint64_t value)
{
    limb_type buf[sizeof(uint64_t) / sizeof(limb_type)];
    auto [sz, sign] = mp::to_limbs<limb_type>(value, std::span{buf});
    return push_bigint(L, mp::basic_integer_view{ std::span{buf, sz}, sign });
}

//int push_bigint(lua_State* L, lua_Integer value)
//{
//    lua_limbs_allocator<limb_type> limbs_allocator{ L };
//    auto [limbs, sz, allsz, sign] = mp::to_limbs<limb_type>(value, limbs_allocator);
//    limbs_allocator.br->sign = sign;
//    limbs_allocator.br->size = sz;
//
//    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
//    lua_setmetatable(L, -2);
//    return 1;
//}

int bigint_create(lua_State* L)
{
    try {
        lua_limbs_allocator<limb_type> limbs_allocator{ L };
        if (lua_isinteger(L, 1)) {
            lua_Integer value = lua_tointeger(L, 1);
            limb_type buf[sizeof(uint64_t) / sizeof(limb_type)];
            auto [sz, sign] = mp::to_limbs<limb_type>(value, std::span{ buf });
            return push_bigint(L, mp::basic_integer_view{ std::span{buf, sz}, sign });
        } else if (lua_isstring(L, 1)) {
            size_t strsz;
            char const* strval = lua_tolstring(L, 1, &strsz);
            auto [limbs, sz, allocsz, sign] = mp::to_limbs<limb_type>(string_view{ strval, strsz }, 0, limbs_allocator);
            bigint_header* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
            bh->sign = sign > 0 ? 0 : 1;
            bh->size = sz;
            luaL_getmetatable(L, BIGINT_METATABLE_NAME);
            lua_setmetatable(L, -2);
            return 1;
        } else if (lua_isnil(L, 1)) {
            lua_pushnil(L);
            return 1;
        } else {
            return luaL_error(L, "bigint.create: invalid argument, type: %d", lua_type(L, 1));
        }
    } catch (std::exception const& e) {
        return luaL_argerror(L, 1, e.what());
    }
}

mp::basic_integer_view<limb_type> restore_bigint(bigint_header * bh)
{
    return mp::basic_integer_view<limb_type>{ std::span{ reinterpret_cast<limb_type*>(bh + 1), bh->size }, bh->sign ? -1: 1 };
    //ival.backend().resize(bh->size, bh->size);
    //memcpy(ival.backend().limbs(), bh + 1, bh->size * sizeof(limb_type));
    //if (bh->sign != ival.backend().sign()) ival.backend().negate();
}

int bigint_tostring(lua_State* L)
{
    bigint_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    auto ival = restore_bigint(bh);
    
    std::vector<char> s;
    if (ival.sign() < 0) s.push_back('-');
    bool reversed;
    mp::to_string_converter((std::span<const limb_type>)ival, std::back_inserter(s), reversed);
    if (reversed) {
        std::reverse(s.begin() + (ival.sign() < 0), s.end());
    }
    lua_pushlstring(L, s.data(), s.size());
    return 1;
}

int bigint_fancy_string(lua_State* L)
{
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

    std::ostringstream ss;
    std::string val;

    limb_type buf[sizeof(lua_Integer) / sizeof(limb_type)];
    mp::basic_integer_view<limb_type> ival;

    if (lua_isinteger(L, 1)) {
        auto [sz, sign] = mp::to_limbs(lua_tointeger(L, 1), std::span{buf});
        ival = mp::basic_integer_view<limb_type>{std::span{buf, sz}, sign};
    } else if (lua_isnumber(L, 1)) {
        return luaL_error(L, "bigint.to_fancy_string: not implemented for numbers, type: %d;", lua_type(L, 1));
    } else if (bigint_header* bh = luaL_test_bigint_lib(L, 1); bh) {
        ival = restore_bigint(bh);
    } else if (lua_isnil(L, 1)) {
        lua_pushnil(L);
        return 1;
    } else {
        return luaL_error(L, "bigint.to_fancy_string: invalid argument, type: %d; `bigint' or integer expected", lua_type(L, 1));
    }

    if (ival.sign() < 0) ss << '-';
    if (radix == 16) {
        ss << "0x"sv;
    }
    bool reversed;
    mp::to_string_converter((std::span<const limb_type>)ival, std::back_inserter(val), reversed, (int)radix, sonia::mp::detail::default_alphabet_big);
    if (reversed) {
        std::reverse(val.begin(), val.end());
    }

    // formatting
    if (!groupSz) {
        ss << val;
    } else {
        for (size_t i = 0, sz = val.size(); i < sz; ++i) {
            ss << val[i];
            if (size_t pos = sz - i - 1; pos && 0 == pos % groupSz) {
                ss << delim;
            }
        }
    }

    val = ss.str();
    lua_pushlstring(L, val.data(), val.size());
    return 1;
}

int bigint_to_integer(lua_State* L)
{
    bigint_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    auto val = restore_bigint(bh);
    luaL_argcheck(L, val >= (std::numeric_limits<lua_Integer>::min)() &&
        val <= (std::numeric_limits<lua_Integer>::max)(), 1, "out of bounds");
    lua_pushinteger(L, (lua_Integer)val);
    return 1;
}

void bigint_binary_operator(lua_State* L, mp::basic_integer_view<limb_type>& l_val, mp::basic_integer_view<limb_type>& r_val)
{
    bigint_header* blh = luaL_test_bigint_lib(L, 1);
    if (blh) { 
        l_val = restore_bigint(blh);
    } else if (lua_isinteger(L, 1)) {
        auto [sz, sign] = mp::to_limbs(lua_tointeger(L, 1), l_val.limbs());
        l_val = mp::basic_integer_view<limb_type>{ l_val.limbs().subspan(0, sz), sign };
    } else {
        luaL_argerror(L, 1, "`bigint' or integer expected");
    }

    bigint_header* brh = luaL_test_bigint_lib(L, 2);
    if (brh) {
        r_val = restore_bigint(brh);
    } else if (lua_isinteger(L, 2)) {
        auto [sz, sign] = mp::to_limbs(lua_tointeger(L, 2), r_val.limbs());
        r_val = mp::basic_integer_view<limb_type>{ r_val.limbs().subspan(0, sz), sign };
    } else {
        luaL_argerror(L, 2, "`bigint' or integer expected");
    }
}

int bigint_unary_minus(lua_State* L)
{
    bigint_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    auto val = restore_bigint(bh);
    return push_bigint(L, -val);
}

int bigint_equal(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };
    
    bigint_binary_operator(L, l_val, r_val);

    lua_pushboolean(L, l_val == r_val);

    return 1;
}

int bigint_lt(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_pushboolean(L, l_val < r_val);

    return 1;
}

int bigint_le(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_pushboolean(L, l_val <= r_val);

    return 1;
}

int bigint_add(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::add(l_val, r_val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_sub(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::sub(l_val, r_val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_mul(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::mul(l_val, r_val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_div(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::div(l_val, r_val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_mod(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::mod(l_val, r_val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1 ;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_pow(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    luaL_argcheck(L, r_val >= (std::numeric_limits<unsigned int>::min)() &&
        r_val <= (std::numeric_limits<unsigned int>::max)(), 2, "out of bounds");

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::pow(l_val, (unsigned int)r_val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_band(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::binand(l_val, r_val, limbs_allocator);
    if (!limbs) {
        return push_bigint(L, 0);
    }
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_bor(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::binor(l_val, r_val, limbs_allocator);
    if (!limbs) {
        return push_bigint(L, 0);
    }
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_bxor(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::binxor(l_val, r_val, limbs_allocator);
    if (!limbs) {
        return push_bigint(L, 0);
    }
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_unary_not(lua_State* L)
{
    bigint_header* bharg = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bharg, 1, "`bigint' is expected");
    auto val = restore_bigint(bharg);

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::binnot(val, limbs_allocator);
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_shl(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    luaL_argcheck(L, r_val >= (std::numeric_limits<unsigned int>::min)() &&
        r_val <= (std::numeric_limits<unsigned int>::max)(), 2, "out of bounds");

    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::shift_left(l_val, (unsigned int)r_val, limbs_allocator);
    if (!limbs) {
        return push_bigint(L, 0);
    }
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

int bigint_shr(lua_State* L)
{
    limb_type llimbs[limbs_per_lua_integer];
    limb_type rlimbs[limbs_per_lua_integer];
    mp::basic_integer_view<limb_type> l_val{ llimbs };
    mp::basic_integer_view<limb_type> r_val{ rlimbs };

    bigint_binary_operator(L, l_val, r_val);

    luaL_argcheck(L, r_val >= 0 &&
        r_val <= (std::numeric_limits<unsigned int>::max)(), 2, "out of bounds");

    unsigned int shift = r_val <= (std::numeric_limits<unsigned int>::max)() ?
        (unsigned int)r_val : (std::numeric_limits<unsigned int>::max)();
    
    lua_limbs_allocator<limb_type> limbs_allocator{ L };
    auto [limbs, sz, rsz, sign] = mp::shift_right(l_val, shift, limbs_allocator);
    if (!limbs) {
        return push_bigint(L, 0);
    }
    auto* bh = reinterpret_cast<bigint_header*>(limbs) - 1;
    bh->size = sz;
    bh->sign = sign < 0 ? 1 : 0;
    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);
    return 1;
}

const struct luaL_Reg bigintlib[] = {
    {"create", bigint_create},
    {"to_fancy_string", bigint_fancy_string},
    {NULL, NULL}
};

const struct luaL_Reg bigintlib_m[] = {
    {"__tostring", bigint_tostring},
    {"__index", bigint_index},
    {"__eq", bigint_equal},
    {"__lt", bigint_lt},
    {"__le", bigint_le},
    {"__add", bigint_add},
    {"__sub", bigint_sub},
    {"__mul", bigint_mul},
    {"__div", bigint_div},
    {"__mod", bigint_mod},
    {"__pow", bigint_pow},
    {"__unm", bigint_unary_minus},
    {"__band", bigint_band},
    {"__bor", bigint_bor},
    {"__bxor", bigint_bxor},
    {"__bnot", bigint_unary_not},
    {"__shl", bigint_shl},
    {"__shr", bigint_shr},
    {NULL, NULL}
};

const struct luaL_Reg bigintlib_f[] = {
    {"to_fancy_string", bigint_fancy_string},
    {"to_integer", bigint_to_integer},
    {NULL, NULL}
};

int luaopen_bigintlib(lua_State* L)
{
    luaL_newmetatable(L, BIGINT_METATABLE_NAME);
    luaL_setfuncs(L, bigintlib_m, 0);
    luaL_setfuncs(L, bigintlib_f, 0);
    lua_pop(L, 1);

    lua_newtable(L);

    luaL_setfuncs(L, bigintlib, 0);
    lua_setglobal(L, BIGINT_LIB_NAME);

    return 1;
}

}
