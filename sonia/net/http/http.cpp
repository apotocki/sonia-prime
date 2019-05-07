//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http.hpp"

#include <utility>
#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>

#include "sonia/singleton.hpp"
#include "sonia/exceptions.hpp"

#define SONIA_PRINT_HTTP_STATUS_CODES_SETUPPER(r, data, i, elem) \
    map_[BOOST_PP_TUPLE_ELEM(3, 2, elem)] = std::string(BOOST_PP_TUPLE_ELEM(3, 1, elem));

#define SONIA_PRINT_HTTP_HEADERS_SETUPPER(r, data, i, elem) \
    map_[to_uc(BOOST_PP_TUPLE_ELEM(2, 1, elem))] = std::pair{std::string(BOOST_PP_TUPLE_ELEM(2, 1, elem)), header::BOOST_PP_TUPLE_ELEM(2, 0, elem)};

#define SONIA_PRINT_HTTP_METHODS_SETUPPER(r, data, i, elem) \
    (std::string(BOOST_PP_TUPLE_ELEM(2, 1, elem)), method_verb::BOOST_PP_TUPLE_ELEM(2, 0, elem))


namespace sonia::http {
    
struct status_codes_cache : singleton
{
    status_codes_cache()
    {
        map_[0] = std::string("UNKNOWN");
        BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_HTTP_STATUS_CODES_SETUPPER, _, SONIA_HTTP_STATUS_CODES_SEQ)
    }

    std::string const& to_message(status c) const
    {
        auto it = map_.find((int)c);
        if (BOOST_LIKELY(it != map_.end())) return it->second;
        return map_.find(0)->second;
    }

    boost::unordered_map<int, std::string> map_;
};

template <typename EnumT>
struct back_way_cache
{
    using back_way_cache_t = back_way_cache;

    std::string const& to_string(EnumT c) const
    {
        int idx = (int)c;
        if (idx <= 0 || idx >= (int)list_.size()) {
            THROW_INTERNAL_ERROR("unexceptable %1% value: %2%"_fmt % typeid(EnumT).name() % idx);
        }
        BOOST_ASSERT (list_[idx]);
        return *list_[idx];
    }

protected:
    std::vector<std::string const*> list_;
};

struct methods_cache 
    : back_way_cache<method_verb>
    , singleton
{
    methods_cache()
    {
        map_ = boost::assign::map_list_of
            BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_HTTP_METHODS_SETUPPER, _, SONIA_HTTP_METHODS_SEQ)
        ;

        list_.resize((size_t)method_verb::UPPER_BOUND_VALUE, nullptr);
        for (auto const& [k, v] : map_) {
            BOOST_ASSERT ((size_t)v < list_.size());
            list_[(size_t)v] = &k;
        }
    }

    method_verb to_method(string_view str) const
    {
        auto it = map_.find(str, hasher(), string_equal_to());
        if (BOOST_LIKELY(it != map_.end())) return it->second;
        return method_verb::UNKNOWN;
    }

    boost::unordered_map<std::string, method_verb, hasher> map_;
};

struct headers_cache
    : back_way_cache<header>
    , singleton
{
    headers_cache()
    {
        BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_HTTP_HEADERS_SETUPPER, _, SONIA_HTTP_HEADERS_SEQ)

        list_.resize((size_t)header::UPPER_BOUND_VALUE, nullptr);
        
        for (auto const& [k, v] : map_) {
            BOOST_ASSERT ((size_t)v.second < list_.size());
            list_[(size_t)v.second] = &v.first;
        }
    }

    header to_header(string_view str) const
    {
        char buff[64];
        std::string tmp;
        if (sizeof(buff) > str.size()) {
            str = string_view(buff, to_uc(str, buff));
        } else {
            tmp = to_uc(str);
            str = to_string_view(tmp);
        }
        auto it = map_.find(str, hasher(), string_equal_to());
        if (BOOST_LIKELY(it != map_.cend())) {
            return it->second.second;
        }
        return header::UNKNOWN;
    }
    
    template <class OutputIteratorT>
    static OutputIteratorT to_uc(string_view sv, OutputIteratorT result)
    {
        for (char c : sv) {
            if (c >= 'a' && c <= 'z') c -= ('a' - 'A');
            *result++ = c;
        }
        return result;
    }

    static std::string to_uc(string_view sv)
    {
        std::string result;
        result.reserve(sv.size());
        to_uc(sv, std::back_inserter(result));
        return result;
    }

    boost::unordered_map<std::string, std::pair<std::string, header>, hasher> map_;
};

std::string const& to_string(status c)
{
    return as_singleton<status_codes_cache>()->to_message(c);
}

std::string const& to_string(header c)
{
    return as_singleton<headers_cache>()->to_string(c);
}

header to_header(string_view str)
{
    return as_singleton<headers_cache>()->to_header(str);
}

std::string const& to_string(method_verb m)
{
    return as_singleton<methods_cache>()->to_string(m);
}

method_verb to_method(string_view str)
{
    return as_singleton<methods_cache>()->to_method(str);
}

}
