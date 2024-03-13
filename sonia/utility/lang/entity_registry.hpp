//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/shared_ptr.hpp"

#include "qname.hpp"
#include "entity.hpp"

namespace sonia::lang {

template <typename EntityT, typename MutexT = dummy_mutex_t>
class entity_registry
{
    using identifier_type = typename EntityT::identifier_type;
    using qname_t = qname<identifier_type>;
    using qname_view_t = qname_view<identifier_type>;

public:
    shared_ptr<EntityT> find(qname_view_t) noexcept;
    void insert(shared_ptr<EntityT>);
    //StringT const* resolve(IdentifierT) const noexcept;

    template <typename FtorT>
    void traverse(FtorT const& ftor)
    {
        lock_guard guard(set_mtx_);
        for (auto & e : set_) {
            if (!ftor(e.name(), *e.value)) break;
        }
    }

private:
    struct entity_wrapper
    {
        shared_ptr<EntityT> value;
        qname_view_t name() const { return value->name(); }
    };

    using set_t = boost::multi_index::multi_index_container<
        entity_wrapper,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
                boost::multi_index::const_mem_fun<entity_wrapper, qname_view_t, &entity_wrapper::name>
            >
        >
    >;

    set_t set_;
    mutable MutexT set_mtx_;
};

template <typename EntityT,typename MutexT>
shared_ptr<EntityT> entity_registry<EntityT, MutexT>::find(qname_view_t name) noexcept
{
    assert(name.is_absolute());
    lock_guard guard(set_mtx_);
    auto it = set_.find(name);
    if (it == set_.end()) {
        return {};
    }
    return it->value;
}

template <typename EntityT, typename MutexT>
void entity_registry<EntityT, MutexT>::insert(shared_ptr<EntityT> e)
{
    assert(e->name().is_absolute());
    lock_guard guard(set_mtx_);
    auto it = set_.find(e->name());
    if (it == set_.end()) {
        set_.insert(it, entity_wrapper{ std::move(e) });
        return;
    }
    THROW_INTERNAL_ERROR("an entity with the same name already exists");
}

/*
template <typename IdentifierT, typename StringT, typename MutexT>
StringT const* string_literal_registry<IdentifierT, StringT, MutexT>::resolve(IdentifierT id) const noexcept
{
    auto const& slice = set_.get<1>();
    lock_guard guard(set_mtx_);
    auto it = slice.find(id);
    if (it == slice.end()) {
        return nullptr;
    }
    return std::addressof(it->name);
}
*/

}
