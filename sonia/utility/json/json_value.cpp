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
#include "sonia/utility/optimized/string.hpp"
#include "sonia/functional/mover.hpp"

namespace sonia {

auto& json_value_holder_accessor::holder(json_value & v) {
    return static_cast<json_value::holder_t&>(v);
}

void json_value_collect(json_value & jv, std::vector<json_value> & accum) noexcept {
    auto & holder = json_value_holder_accessor::holder(jv);
    if (holder.is_ptr()) {
        optimized_base * ptr = holder.get_pointer();
        if (ptr->unique()) {
            if (jv.type() == json_value_type::array || jv.type() == json_value_type::object) {
                // stealing value, don't care about cookie
                accum.push_back(std::move(jv));
                holder.set_not_ptr();
            }
        }
    }
}

template <class ElementT, class HolderT>
struct optimized_collection_impl
{
    typedef std::allocator<ElementT> allocator_t;
    typedef optimized_array_base<ElementT> optimized_collection_base_t;
    typedef adjacent_buffer<ElementT, optimized_collection_base_t> optimized_collection_t;

    static void init(HolderT * self, array_view<ElementT> s) {
        if (!s.empty()) {
            optimized_collection_t * ptr = allocate_adjacent_buffer<ElementT, optimized_collection_base_t, allocator_t>(
                s.size(), s | boost::adaptors::transformed(mover())
            );
            self->set_pointer(ptr);
        }
    }

    static array_view<ElementT> get(HolderT * self) {
        return self->is_ptr() ? ptr(self)->to_array_view() : array_view<ElementT>();
    }

    static array_view<const ElementT> get(HolderT const* self) {
        return self->is_ptr() ? ptr(self)->to_array_view() : array_view<const ElementT>();
    }

    static size_t size(HolderT const* self) {
        return self->is_ptr() ? ptr(self)->size() : 0;
    }

protected:
    static optimized_collection_t * ptr(HolderT * self) {
        return static_cast<optimized_collection_t *>(self->get_pointer());
    }

    static optimized_collection_t const* ptr(HolderT const* self) {
        return static_cast<optimized_collection_t const*>(self->get_pointer());
    }
};

typedef std::pair<std::string, json_value> object_item_t;

#if 0
class optimized_object : public optimized_base {
public:
    template <typename IteratorT>
    explicit optimized_object(IteratorT elem_it) {
        for (; !elem_it.empty(); ++elem_it) {
            auto it = map_.find(elem_it->first);
            if (it == map_.end()) {
                map_.insert(it, std::move(*elem_it));
            } else {
                it->second = std::move(elem_it->second);
            }
        }
    }

    optimized_object(array_view<std::string> names, array_view<json_value> vals) {
        BOOST_ASSERT(names.size() == vals.size());
        auto name_it = names.begin();
        auto value_it = vals.begin();
        for (; name_it != names.end() && value_it != vals.end(); ++name_it, ++value_it) {
            auto it = map_.find(*name_it);
            if (it == map_.end()) {
                map_.insert(it, std::make_pair(std::move(*name_it), std::move(*value_it)));
            } else {
                it->second = std::move(*value_it);
            }
        }
    }

    json_value * get(string_view key) {
        auto it = map_.find(key, string_hasher(), string_equal_to());
        return it != map_.end() ? &it->second : (json_value*)nullptr;
    }

    json_value const* get(string_view key) const {
        auto it = map_.find(key, string_hasher(), string_equal_to());
        return it != map_.end() ? &it->second : (json_value const*)nullptr;
    }

    optimized_base * clone() const override {
        return new optimized_object(*this);
    }

    void dispose() noexcept override {
        delete this;
    }

private:
    boost::unordered_map<std::string, json_value> map_;
};

#endif


struct json_object_item_transformer
{
    typedef std::pair<std::string&&, json_value&&> result_type;

    template <typename TplT>
    result_type operator()(TplT&& x) const { return result_type(std::move(boost::get<0>(x)), std::move(boost::get<1>(x))); }
};

template <class HolderT>
struct optimized_object_impl : optimized_collection_impl<object_item_t, HolderT>
{
    typedef optimized_collection_impl<object_item_t, HolderT> base_t;
    typedef typename base_t::allocator_t allocator_t;
    typedef typename base_t::optimized_collection_base_t optimized_collection_base_t;
    typedef typename base_t::optimized_collection_t optimized_collection_t;

    static void init(HolderT * self, array_view<std::string> keys, array_view<json_value> vals) {
        BOOST_ASSERT(keys.size() == vals.size());
        if (!keys.empty()) {
            optimized_collection_t * ptr = allocate_adjacent_buffer<object_item_t, optimized_collection_base_t, allocator_t>(
                keys.size(), boost::combine(keys, vals) | boost::adaptors::transformed(json_object_item_transformer())
            );
            self->set_pointer(ptr);
            std::sort(ptr->begin(), ptr->end(), [](object_item_t const& l, object_item_t const& r) -> bool { return l.first < r.first; });
        }
    }

    using base_t::get;

    static json_value const* get(HolderT const* self, string_view key) {
        if (!self->is_ptr()) return nullptr;
        optimized_collection_t const* ptr = base_t::ptr(self);
        object_item_t const* it = std::lower_bound(ptr->begin(), ptr->end(), key, [](object_item_t const& l, string_view r) -> bool { return l.first < r; });
        if (it == ptr->end() || it->first != key) return nullptr;
        return &it->second;
    }

    static json_value * get(HolderT * self, string_view key) {
        if (!self->is_ptr()) return nullptr;
        optimized_collection_t * ptr = base_t::ptr(self);
        object_item_t * it = std::lower_bound(ptr->begin(), ptr->end(), key, [](object_item_t const& l, string_view r) -> bool { return l.first < r; });
        if (it == ptr->end() || it->first != key) return nullptr;
        return &it->second;
    }
};

bool json_value::get_bool() const {
    if (json_value_type::boolean == type()) {
        return !!this->get_uint();
    }
    throw exception("json_value (%1%) is not a boolean"_fmt % to_string(*this));
}

int json_value::get_int() const {
    if (json_value_type::number == type()) {
        typedef optimized_decimal_impl<holder_t> number_t;
        return number_t::get<int>(this);
    }
    throw exception("json_value (%1%) is not a number"_fmt % to_string(*this));
}

int64_t json_value::get_int64() const {
    if (json_value_type::number == type()) {
        typedef optimized_decimal_impl<holder_t> number_t;
        return number_t::get<int64_t>(this);
    }
    throw exception("json_value (%1%) is not a number"_fmt % to_string(*this));
}

decimal json_value::get_number() const {
    if (json_value_type::number == type()) {
        typedef optimized_decimal_impl<holder_t> number_t;
        return number_t::get(this);
    }
    throw exception("json_value (%1%) is not a number"_fmt % to_string(*this));
}

string_view json_value::get_string() const {
    if (json_value_type::string == type()) {
        typedef optimized_string_impl<holder_t> string_t;
        return string_t::get(this);
    }
    throw exception("json_value (%1%) is not a string"_fmt % to_string(*this));
}

array_view<const json_value> json_value::get_array() const {
    if (json_value_type::array == type()) {
        typedef optimized_collection_impl<json_value, holder_t> array_t;
        return array_t::get(this);
    }
    throw exception("json_value (%1%) is not an array"_fmt % to_string(*this));
}

array_view<json_value> json_value::get_array() {
    if (json_value_type::array == type()) {
        typedef optimized_collection_impl<json_value, holder_t> array_t;
        return array_t::get(this);
    }
    throw exception("json_value (%1%) is not an array"_fmt % to_string(*this));
}

json_object json_value::get_object() const {
    if (json_value_type::object == type()) {
        return json_object(*this);
    }
    throw exception("json_value (%1%) is not an object"_fmt % to_string(*this));
}

json_object::json_object(holder_t const& hval)
    : holder_t(cref(hval))
{}

json_value const* json_object::operator[](string_view key) const noexcept {
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::get(this, key);
}

json_value * json_object::operator[](string_view key) noexcept {
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::get(this, key);
}

array_view<const std::pair<std::string, json_value>> json_object::items() const noexcept {
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::get(this);
}

size_t json_object::size() const noexcept {
    typedef optimized_object_impl<holder_t> object_t;
    return object_t::size(this);
}

array_view<std::pair<const std::string, json_value>> json_object::items() noexcept {
    typedef optimized_object_impl<holder_t> object_t;
    typedef std::pair<const std::string, json_value> safe_item_t;
    auto r = object_t::get(this);
    return array_view<safe_item_t>(reinterpret_cast<safe_item_t*>(r.begin()), r.size());
}

// null 
json_value::json_value() {
    *data() = 1;
    set_service_cookie((size_t)json_value_type::null);
}

// bool
json_value::json_value(bool val) {
    *data() = 1;
    set_service_cookie((size_t)json_value_type::boolean);
    set_uint(val ? 1 : 0);
}

json_value::json_value(decimal val) {
    typedef optimized_decimal_impl<holder_t> number_t;
    number_t::set(this, val);
    set_service_cookie((size_t)json_value_type::number);
}

json_value::json_value(string_view val) {
    typedef optimized_string_impl<holder_t> string_t;
    string_t::init(this, val);
    set_service_cookie((size_t)json_value_type::string);
}

json_value::json_value(array_view<json_value> val) {
    typedef optimized_collection_impl<json_value, holder_t> array_t;
    array_t::init(this, val);
    set_service_cookie((size_t)json_value_type::array);
}

json_value::json_value(array_view<std::string> keys, array_view<json_value> vals) {
    typedef optimized_object_impl<holder_t> object_t;
    object_t::init(this, keys, vals);
    set_service_cookie((size_t)json_value_type::object);
}

json_value::~json_value() {
    if (is_ptr()) {
        optimized_base * ptr = get_pointer();
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
                        for (auto & item : jv.get_object().items()) {
                            json_value_collect(item.second, accum);
                        }
                    }
                    h.reset();
                }
            }
        }
    }
}

std::ostream & operator<< (std::ostream& os, json_value const& val) {
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
        throw internal_error("unexpected json_value type (%1%)"_fmt % (int)val.type());
    }
}

std::string to_string(json_value const& val) {
    std::ostringstream ss;
    ss << val;
    return std::move(ss.str());
}

}
