//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <tuple>

#include "sonia/string.hpp"

#include "sonia/utility/invokation/invokation.hpp"

#include "sonia/utility/automatic_polymorphic.hpp"

#include "functor_type.hpp"

namespace sonia::xmlbuilder {

class attr_parser
{
public:
    virtual ~attr_parser() = default;

    // {explicit value, funcname, no_return} if null or empty => not takes place
    virtual std::tuple<blob_result, std::string, func_type> parse(string_view val) const;
};

class functional_attr_parser : public attr_parser
{
public:
    std::tuple<blob_result, std::string, func_type> parse(string_view value) const override;
};

template <typename T>
class particular_attr_parser : public attr_parser
{
public:
    std::tuple<blob_result, std::string, func_type> parse(string_view value) const override;
};

template <>
class particular_attr_parser<bool> : public attr_parser
{
public:
    std::tuple<blob_result, std::string, func_type> parse(string_view value) const override;
};

template <typename T, size_t SzV>
class particular_attr_parser<std::array<T, SzV>> : public attr_parser
{
    static constexpr bool is_simple_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
    using intermediate_element_t = std::conditional_t<is_simple_v, T, blob_result>;

public:
    std::tuple<blob_result, std::string, func_type> parse(string_view value) const override;
};

template <typename T>
class particular_attr_parser<std::vector<T>> : public attr_parser
{
    static constexpr bool is_simple_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
    using intermediate_element_t = std::conditional_t<is_simple_v, T, blob_result>;

public:
    std::tuple<blob_result, std::string, func_type> parse(string_view value) const override;
};

template <typename ... Ts>
class particular_attr_parser<std::tuple<Ts ...>> : public attr_parser
{
    using tuple_t = std::tuple<Ts ...>;

    template <size_t I, typename IteratorT>
    inline static void set(IteratorT& it, std::vector<blob_result>& v)
    {
        using element_t = std::tuple_element_t<I, tuple_t>;
        string_view value{ (*it).begin(), (*it).end() }; ++it;
        v.push_back(std::get<0>(particular_attr_parser<element_t>().parse(value)));
    }

    template <size_t ... Ids, typename RangeT>
    inline static void do_parse(std::index_sequence<Ids ...>, RangeT rng, std::vector<blob_result>& v)
    {
        auto it = rng.begin();
        (..., set<Ids>(it, v));
        //(..., (std::get<Ids>(v) = particular_attr_parser<std::tuple_element_t<Ids, tuple_t>>().parse(*it++)));
    }

public:
    std::tuple<blob_result, std::string, func_type> parse(string_view value) const override;
};

using automatic_attr_parser_t = automatic_polymorphic<attr_parser, sizeof(void*)>;

}
