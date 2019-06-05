//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_VALUE_HPP
#define SONIA_UTILITY_JSON_VALUE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range_core.hpp>

#include "sonia/string.hpp"
#include "sonia/array_view.hpp"
#include "sonia/type_traits.hpp"

#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/number/decimal.hpp"

#define SONIA_JSON_VALUE_SZ 8 // at least pointer size for 64bit platforms

namespace sonia {

namespace json_detail {
    typedef optimized_holder<SONIA_JSON_VALUE_SZ, 3, uint32_t> holder_t;
}

enum class json_value_type
{
    null = 0,
    boolean,
    number,
    string,
    array,
    object
};

class json_value;
class json_object;
template <bool IsConstV> class json_object_item_iterator;


class json_object : json_detail::holder_t
{
    friend class json_value;
    friend class json_value_holder_accessor;
    template <bool> friend class json_object_item_iterator;

    json_object(json_detail::holder_t const&);

    using item_range_t = boost::iterator_range<json_object_item_iterator<false>>;
    using const_item_range_t = boost::iterator_range<json_object_item_iterator<true>>;

public:
    json_object(); // equivalent of {}

    json_object(json_object const&);
    json_object(json_object &&) = default;
    json_object& operator=(json_object const&);
    json_object& operator=(json_object &&) = default;

    friend bool operator==(json_object const&, json_object const&);
    friend bool operator<(json_object const&, json_object const&);

    operator json_value() const noexcept;

    size_t size() const noexcept;

    json_value const* operator[](string_view) const noexcept;
    json_value * operator[](string_view) noexcept;

    const_item_range_t items() const noexcept;
    item_range_t items() noexcept;
};

std::pair<string_view, json_value&> json_object_item_iterator_dereference(json_object &, size_t pos);

template <bool IsConstV>
class json_object_item_iterator
    : public boost::iterator_facade<
        json_object_item_iterator<IsConstV>,
        std::pair<string_view, conditional_t<IsConstV, json_value const&, json_value&>>,
        boost::random_access_traversal_tag,
        std::pair<string_view, conditional_t<IsConstV, json_value const&, json_value&>>
    >
{
    friend class boost::iterator_core_access;
    friend class json_object;

    typedef std::pair<string_view, conditional_t<IsConstV, json_value const&, json_value&>> value_t;
    mutable json_object obj_;
    size_t pos_;

    explicit json_object_item_iterator(json_object const& p, size_t pos = 0) : obj_(p), pos_(pos) {}

    bool equal(json_object_item_iterator const& rhs) const {
        return pos_ == rhs.pos_ && obj_ == rhs.obj_;
    }

    void increment() { ++pos_; }
    void decrement() { --pos_; }

    value_t dereference() const
    {
        auto pair = json_object_item_iterator_dereference(obj_, pos_);
        return {pair.first, pair.second};
    }
};

class json_value : json_detail::holder_t
{
    friend class json_value_holder_accessor;
    friend class json_object;

    explicit json_value(json_detail::holder_t const&); // reference

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

    json_value_type type() const
    {
        return (json_value_type)json_detail::holder_t::get_service_cookie();
    }

    explicit operator bool() const noexcept { return type() == json_value_type::null; }

    bool get_bool() const;
    int get_int() const;
    int64_t get_int64() const;
    decimal get_number() const;
    string_view get_string() const;
    array_view<const json_value> get_array() const;
    array_view<json_value> get_array();
    json_object get_object() const;
};

class json_value_holder_accessor
{
public:
    template <class JsonT>
    static add_const_if_t<is_const_v<JsonT>, json_detail::holder_t>& holder(JsonT & v)
    {
        return static_cast<add_const_if_t<is_const_v<JsonT>, json_detail::holder_t>&>(v);
    }
};

bool operator==(json_value const&, json_value const&);

bool operator<(json_value const&, json_value const&);

inline bool operator!=(json_value const& l, json_value const& r) { return !(l == r); }

inline bool operator>(json_value const& l, json_value const& r) { return r < l; }

inline bool operator<=(json_value const& l, json_value const& r) { return !(r < l); }

inline bool operator>=(json_value const& l, json_value const& r) { return !(l < r); }

std::string to_string(json_value const&);

std::ostream & operator<< (std::ostream &, json_value const&);

}

#endif // SONIA_UTILITY_JSON_VALUE_HPP
