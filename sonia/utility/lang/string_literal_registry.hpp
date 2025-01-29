//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "sonia/concurrency.hpp"

#include "identifier_builder.hpp"

namespace sonia::lang {

template <typename IdentifierT, typename StringT, typename MutexT = dummy_mutex_t>
class string_literal_registry
{
    using string_view_t = sonia::basic_string_view<typename StringT::value_type>;

public:
    explicit string_literal_registry(identifier_builder<IdentifierT> & ib) : ib_{ib} {}

    IdentifierT resolve(string_view_t);
    StringT const* resolve(IdentifierT) const noexcept;

private:
    struct name_part_item
    {
        StringT name;
        IdentifierT id;
    };

    using set_t = boost::multi_index::multi_index_container<
        name_part_item,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::member<name_part_item, StringT, &name_part_item::name>
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::member<name_part_item, IdentifierT, &name_part_item::id>
            >
        >
    >;

    set_t set_;
    mutable MutexT set_mtx_;
    identifier_builder<IdentifierT> & ib_;
};

template <typename IdentifierT, typename StringT, typename MutexT>
IdentifierT string_literal_registry<IdentifierT, StringT, MutexT>::resolve(string_view_t name)
{
    lock_guard guard(set_mtx_);
    auto it = set_.find(name, hasher(), sonia::range_equal());
    if (it == set_.end()) {
        auto id = ib_();
#ifndef SONIA_LANG_DEBUG
        set_.insert(it, name_part_item{ StringT{name}, id });
        return id;
#else
        it = set_.insert(it, name_part_item{ StringT{name}, id });
        const_cast<IdentifierT&>(it->id).debug_name = it->name;
#endif
    }
    return it->id;
}

template <typename IdentifierT, typename StringT, typename MutexT>
StringT const* string_literal_registry<IdentifierT, StringT, MutexT>::resolve(IdentifierT id) const noexcept
{
    auto const& slice = set_.template get<1>();
    lock_guard guard(set_mtx_);
    auto it = slice.find(id);
    if (it == slice.end()) {
        return nullptr;
    }
    return std::addressof(it->name);
}

}
