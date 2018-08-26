//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_VALUE_HPP
#define SONIA_UTILITY_JSON_VALUE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "sonia/array_view.hpp"

#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/number/decimal.hpp"
//#include "sonia/utility/iterators/wrapper_iterator.hpp"

#define SONIA_JSON_VALUE_SZ 8 // at least pointer size for 64bit platforms

namespace sonia {

/*
class json_value_visitor {
public:
    virtual ~json_value_visitor() {}

    virtual void operator()() = 0; // null
    virtual void operator()(bool) = 0;
    virtual void operator()(decimal const&) = 0;
    virtual void operator()(std::string const&) = 0;
    virtual void operator()(array_view<json_value>) = 0;
};
*/
enum class json_value_type {
    null = 0,
    boolean,
    number,
    string,
    array,
    object
};

class json_object : optimized_holder<SONIA_JSON_VALUE_SZ, 3>
{
    friend class json_value;
    typedef optimized_holder<SONIA_JSON_VALUE_SZ, 3> holder_t;

    json_object(holder_t const&);

public:
    size_t size() const noexcept;

    json_value const* operator[](string_view) const noexcept;
    json_value * operator[](string_view) noexcept;

    array_view<const std::pair<std::string, json_value>> items() const noexcept;
    array_view<std::pair<const std::string, json_value>> items() noexcept;
};

class json_value : optimized_holder<SONIA_JSON_VALUE_SZ, 3>
{
    friend class json_value_holder_accessor;

    typedef optimized_holder<SONIA_JSON_VALUE_SZ, 3> holder_t;

 public:
    json_value();

    explicit json_value(bool);
    explicit json_value(decimal);
    explicit json_value(string_view);
    explicit json_value(array_view<json_value>);
    json_value(array_view<std::string>, array_view<json_value>);

    ~json_value();

    json_value(json_value const&) = default;
    json_value(json_value &&) = default;
    json_value& operator=(json_value const&) = default;
    json_value& operator=(json_value &&) = default;

    json_value_type type() const {
        return (json_value_type)holder_t::get_service_cookie();
    }

    bool get_bool() const;
    int get_int() const;
    int64_t get_int64() const;
    decimal get_number() const;
    string_view get_string() const;
    array_view<const json_value> get_array() const;
    array_view<json_value> get_array();
    json_object get_object() const;
};

class json_value_holder_accessor {
public:
    static auto& holder(json_value &);
};

std::string to_string(json_value const&);

std::ostream & operator<< (std::ostream &, json_value const&);

}

#endif // SONIA_UTILITY_JSON_VALUE_HPP
