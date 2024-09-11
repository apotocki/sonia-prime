//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/utility/functional/hash.hpp"

namespace sonia::lang {

template <typename EntityT, typename MutexT = dummy_mutex_t>
class entity_registry
{
    using entity_identifier_type = typename EntityT::identifier_type;

public:
    EntityT const& get(entity_identifier_type) const;
    
    template <typename FactoryT>
    EntityT & find_or_create(EntityT const&, FactoryT const&);

    void insert(shared_ptr<EntityT>);
    //StringT const* resolve(IdentifierT) const noexcept;

    template <typename FtorT>
    void traverse(FtorT const& ftor)
    {
        //THROW_NOT_IMPLEMENTED_ERROR("entity_registry traverse");
        lock_guard guard(set_mtx_);
        for (auto & e : set_) {
            if (!ftor(*e.value)) break;
        }
    }

private:
    struct entity_wrapper
    {
        shared_ptr<EntityT> value;
        //qname_identifier_type name() const { return value->name(); }
        //inline entity_identifier_type id() const noexcept { return value->id(); }
        inline EntityT const& self() const noexcept { return *value; }
    };

    using set_t = boost::multi_index::multi_index_container<
        entity_wrapper,
        boost::multi_index::indexed_by<
            boost::multi_index::random_access<>,
            //boost::multi_index::ordered_unique<
            //    boost::multi_index::const_mem_fun<entity_wrapper, entity_identifier_type, &entity_wrapper::id>
            //>,
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<entity_wrapper, EntityT const&, &entity_wrapper::self>,
                hasher
            >
        >
    >;

    set_t set_;
    mutable MutexT set_mtx_;
};

template <typename EntityT, typename MutexT>
template <typename FactoryT>
EntityT & entity_registry<EntityT, MutexT>::find_or_create(EntityT const& sample, FactoryT const& factory)
{
    lock_guard guard(set_mtx_);
    auto& index = set_.get<1>();
    auto it = index.find(sample);
    if (it == index.end()) {
        it = index.insert(it, entity_wrapper{ factory() });
        it->value->set_id(entity_identifier_type{ set_.size() });
    }
    return *it->value;
}

template <typename EntityT,typename MutexT>
EntityT const& entity_registry<EntityT, MutexT>::get(entity_identifier_type eid) const
{
    lock_guard guard(set_mtx_);
    if (eid.raw() > set_.size()) [[unlikely]] {
        THROW_INTERNAL_ERROR("no entity with id: %1%"_fmt % eid);
    }
    return *set_.template get<0>().at(eid.raw() - 1).value;
}

template <typename EntityT, typename MutexT>
void entity_registry<EntityT, MutexT>::insert(shared_ptr<EntityT> e)
{
    lock_guard guard(set_mtx_);
    auto& index = set_.get<1>();
    auto it = index.find(*e);
    if (it == index.end()) {
        it = index.insert(it, entity_wrapper{ std::move(e) });
        it->value->set_id(entity_identifier_type{ set_.size() });
        return;
    }
    THROW_INTERNAL_ERROR("an equivalent entity has bean already registered");
}

}
