//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "value.hpp"

#include <sstream>
#include <algorithm>
#include <boost/unordered_map.hpp>
#include <boost/range/combine.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/utility/optimized/decimal.hpp"
#include "sonia/utility/optimized/array.hpp"
#include "sonia/utility/functional/mover.hpp"
#include "sonia/utility/streaming/array_view.hpp"

namespace sonia {

using namespace json_detail;

void json_value_collect(json_value & jv, std::vector<json_value> & accum) noexcept
{
    auto & holder = json_value_holder_accessor::holder(jv);
    if (holder.is_ptr()) {
        auto * ptr = holder.get_pointer();
        if (ptr->unique()) {
            if (jv.type() == json_value_type::array || jv.type() == json_value_type::object) {
                // stealing value, don't care about cookie
                accum.push_back(std::move(jv));
                holder.set_not_ptr();
            }
        }
    }
}

using json_item_name_t = optimized_array<char, 16, uint32_t>;
using object_item_t = std::pair<json_item_name_t, json_value>;

struct json_object_item_transformer
{
    using result_type = std::pair<array_view<const char>, json_value>;

    template <typename TplT>
    result_type operator()(TplT&& x) const {
        return result_type(
            to_string_view(boost::get<0>(x)),
            boost::get<1>(x)
        );
    }
};

struct object_item_less
{
    inline string_view to_string_view(object_item_t const& v) const
    {
        return (string_view)((array_view<const char>)v.first);
    }

    bool operator()(object_item_t const& l, object_item_t const& r)
    {
        return to_string_view(l) < to_string_view(r);
    }

    bool operator()(object_item_t const& l, string_view r)
    {
        return to_string_view(l) < r;
    }
};

template <class HolderT>
struct optimized_object_impl : optimized_array_impl<object_item_t, HolderT>
{
    typedef optimized_array_impl<object_item_t, HolderT> base_t;
    typedef typename base_t::allocator_t allocator_t;
    typedef typename base_t::optimized_collection_base_t optimized_collection_base_t;
    typedef typename base_t::optimized_collection_t optimized_collection_t;

    static void init(HolderT * self, array_view<const std::string> keys, array_view<const json_value> vals)
    {
        BOOST_ASSERT(keys.size() == vals.size());
        if (!keys.empty()) {
            optimized_collection_t * ptr = allocate_adjacent_buffer<object_item_t, optimized_collection_base_t>(
				allocator_t{}, keys.size(), boost::combine(keys, vals) | boost::adaptors::transformed(json_object_item_transformer())
            );
            self->set_pointer(ptr);
            std::sort(ptr->begin(), ptr->end(), object_item_less());
        } else {
            self->set_uint(0); // set size 0
        }
    }

    using base_t::get;

    static json_value const* get(HolderT const* self, string_view key)
    {
        if (!self->is_ptr()) return nullptr;
        optimized_collection_t const* ptr = base_t::ptr(self);
        object_item_t const* it = std::lower_bound(ptr->begin(), ptr->end(), key, object_item_less());
        if (it == ptr->end() || (string_view)((array_view<const char>)it->first) != key) return nullptr;
        return &it->second;
    }

    static json_value * get(HolderT * self, string_view key)
    {
        if (!self->is_ptr()) return nullptr;
        optimized_collection_t * ptr = base_t::ptr(self);
        object_item_t * it = std::lower_bound(ptr->begin(), ptr->end(), key, object_item_less());
        if (it == ptr->end() || (string_view)((array_view<const char>)it->first) != key) return nullptr;
        return &it->second;
    }
};

std::pair<string_view, json_value&> json_object_item_iterator_dereference(json_object & obj, size_t pos)
{
    typedef optimized_object_impl<holder_t> object_t;
    auto& item = object_t::get(&json_value_holder_accessor::holder(obj))[pos];
    return {string_view(item.first.begin(), item.first.size()), item.second};
}

bool operator==(json_object const& lhs, json_object const& rhs)
{
    typedef optimized_object_impl<holder_t> object_t;
    auto litems = object_t::get(&lhs);
    auto ritems = object_t::get(&rhs);
    return litems == ritems;
}

bool operator<(json_object const& lhs, json_object const& rhs)
{
    typedef optimized_object_impl<holder_t> object_t;
    auto litems = object_t::get(&lhs);
    auto ritems = object_t::get(&rhs);
    return litems < ritems;
}

bool json_value::get_bool() const
{
    if (json_value_type::boolean == type()) {
        return !!this->get_uint();
    }
    throw exception("json_value (%1%) is not a boolean"_fmt % to_string(*this));
}

int json_value::get_int() const
{
    if (json_value_type::number == type()) {
        typedef optimized_decimal_impl<holder_t> number_t;
        return number_t::get<int>(this);
    }
    throw exception("json_value (%1%) is not a number"_fmt % to_string(*this));
}

int64_t json_value::get_int64() const
{
    if (json_value_type::number == type()) {
        typedef optimized_decimal_impl<holder_t> number_t;
        return number_t::get<int64_t>(this);
    }
    throw exception("json_value (%1%) is not a number"_fmt % to_string(*this));
}

decimal json_value::get_number() const
{
    if (json_value_type::number == type()) {
        typedef optimized_decimal_impl<holder_t> number_t;
        return number_t::get(this);
    }
    throw exception("json_value (%1%) is not a number"_fmt % to_string(*this));
}

string_view json_value::get_string() const
{
    if (json_value_type::string == type()) {
        typedef optimized_array_impl<char, holder_t> string_t;
        return string_t::get(this);
    }
    throw exception("json_value (%1%) is not a string"_fmt % to_string(*this));
}

array_view<const json_value> json_value::get_array() const
{
    if (json_value_type::array == type()) {
        typedef optimized_array_impl<json_value, holder_t> array_t;
        return array_t::get(this);
    }
    throw exception("json_value (%1%) is not an array"_fmt % to_string(*this));
}

array_view<json_value> json_value::get_array()
{
    if (json_value_type::array == type()) {
        typedef optimized_array_impl<json_value, holder_t> array_t;
        return array_t::get(this);
    }
    throw exception("json_value (%1%) is not an array"_fmt % to_string(*this));
}

json_object json_value::get_object() const
{
    if (json_value_type::object == type()) {
        return json_object(*this);
    }
    throw exception("json_value (%1%) is not an object"_fmt % to_string(*this));
}

json_object::json_object()
{
    using object_t = optimized_object_impl<holder_t>;
    object_t::init(this, array_view<std::string>(), array_view<json_value>());
    set_service_cookie((size_t)json_value_type::object);
}

json_object::json_object(holder_t const& hval)
    : holder_t(cref(hval))
{}

json_object::json_object(json_object const& rhs)
    : holder_t(cref((holder_t const&)rhs))
{}

json_object& json_object::operator=(json_object const& rhs)
{
    holder_t::operator= (cref((holder_t const&)rhs));
    return *this;
}

json_value const* json_object::operator[](string_view key) const noexcept
{
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::get(this, key);
}

json_value * json_object::operator[](string_view key) noexcept
{
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::get(this, key);
}

json_object::const_item_range_t json_object::items() const noexcept
{
    typedef json_object_item_iterator<true> const_iterator;
    return const_item_range_t(const_iterator(*this), const_iterator(*this, size()));
}

json_object::item_range_t json_object::items() noexcept
{
    typedef json_object_item_iterator<false> iterator;
    return item_range_t(iterator(*this), iterator(*this, size()));
}

size_t json_object::size() const noexcept
{
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::size(this);
}

json_object::operator json_value() const noexcept
{
    return json_value((holder_t const&)*this);
}

json_value::json_value(holder_t const& hval)
    : holder_t(cref(hval))
{}

// null 
json_value::json_value()
{
    *data() = 1;
    set_service_cookie((size_t)json_value_type::null);
}

// bool
json_value::json_value(bool val)
{
    *data() = 1;
    set_service_cookie((size_t)json_value_type::boolean);
    set_uint(val ? 1 : 0);
}

json_value::json_value(int val)
{
    using number_t = optimized_decimal_impl<holder_t>;
    number_t::set(this, decimal{val});
    set_service_cookie((size_t)json_value_type::number);
}

json_value::json_value(decimal val)
{
    using number_t = optimized_decimal_impl<holder_t>;
    number_t::set(this, val);
    set_service_cookie((size_t)json_value_type::number);
}

json_value::json_value(string_view val)
{
    using string_t = optimized_array_impl<char, holder_t>;
    string_t::init(this, val);
    set_service_cookie((size_t)json_value_type::string);
}

json_value::json_value(array_view<json_value> val)
{
    using array_t = optimized_array_impl<json_value, holder_t>;
    array_t::init(this, val | boost::adaptors::transformed(mover()), val.size());
    set_service_cookie((size_t)json_value_type::array);
}

json_value::json_value(std::initializer_list<json_value> l)
{
    using array_t = optimized_array_impl<json_value, holder_t>;
    array_t::init(this, l | boost::adaptors::transformed(mover()), l.size());
    set_service_cookie((size_t)json_value_type::array);
}

json_value::json_value(array_view<const std::string> keys, array_view<const json_value> vals)
{
    using object_t = optimized_object_impl<holder_t>;
    object_t::init(this, keys, vals);
    set_service_cookie((size_t)json_value_type::object);
}

template <class OpT>
bool compare(json_value const& lhs, json_value const& rhs, OpT const& op)
{
    switch (lhs.type())
    {
    case json_value_type::null:
        return true;
    case json_value_type::boolean:
        return op(lhs.get_bool(), rhs.get_bool());
    case json_value_type::number:
        return op(lhs.get_number(), rhs.get_number());
    case json_value_type::string:
        return op(lhs.get_string(), rhs.get_string());
    case json_value_type::array:
        return op(lhs.get_array(), rhs.get_array());
    case json_value_type::object:
        return op(lhs.get_object(), rhs.get_object());
    default:
        THROW_INTERNAL_ERROR("unexpected json_value type (%1%)"_fmt % (int)lhs.type());
    }
}

bool operator==(json_value const& lhs, json_value const& rhs)
{
    if (lhs.type() != rhs.type()) return false;
    return compare(lhs, rhs, [](auto const& l, auto const& r)->bool { return l == r; });
}

bool operator<(json_value const& lhs, json_value const& rhs)
{
    if (lhs.type() != rhs.type()) return (int)lhs.type() < (int)rhs.type();
    return compare(lhs, rhs, [](auto const& l, auto const& r)->bool { return l < r; });
}

json_value::~json_value()
{
    if (is_ptr()) {
        auto * ptr = get_pointer();
        if (ptr->unique()) {
            if (type() == json_value_type::array || type() == json_value_type::object) {
                std::vector<json_value> accum;
                accum.push_back(std::move(*this));
                while (!accum.empty()) {
                    json_value jv(std::move(accum.back()));
                    accum.pop_back();
                    auto & h = json_value_holder_accessor::holder(jv);
                    if (jv.type() == json_value_type::array) {
                        accum.reserve(accum.size() + jv.get_array().size());
                        for (json_value & jv : jv.get_array()) {
                            json_value_collect(jv, accum);
                        }
                    } else if (jv.type() == json_value_type::object) {
                        for (auto const & item : jv.get_object().items()) {
                            json_value_collect(item.second, accum);
                        }
                    }
                    h.reset();
                }
            }
        }
    }
}

std::ostream & operator<< (std::ostream& os, json_value const& val)
{
    switch (val.type())
    {
    case json_value_type::null:
        return os << "null";
    case json_value_type::boolean:
        return os << (val.get_bool() ? "true" : "false");
    case json_value_type::number:
        return os << val.get_number();
    case json_value_type::string:
        return os << '\"' << val.get_string() << '\"';
    case json_value_type::array:
        return os << val.get_array();
    case json_value_type::object: {
        os << '{';
        bool first = true;
        for (auto const& item : val.get_object().items()) {
            if (!first) os << ", "; else first = false;
            os << '"' << item.first << "\": " << item.second;
        }
        return os << '}';
    }
    default:
        THROW_INTERNAL_ERROR("unexpected json_value type (%1%)"_fmt % (int)val.type());
    }
}

std::string to_string(json_value const& val)
{
    std::ostringstream ss;
    ss << val;
    return std::move(ss.str());
}

}
