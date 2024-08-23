//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
//#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "sonia/concurrency.hpp"

namespace sonia::lang {

template <typename FunctionalT, typename MutexT = dummy_mutex_t>
class functional_registry
{
    using identifier_type = typename FunctionalT::identifier_type;

public:
    FunctionalT* find(identifier_type) const noexcept;

    FunctionalT& resolve(identifier_type) noexcept;

private:
    using set_t = boost::multi_index::multi_index_container<
        FunctionalT,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::const_mem_fun<FunctionalT, identifier_type, &FunctionalT::id>
            >
        >
    >;

    set_t set_;
    mutable MutexT set_mtx_;
};

template <typename FunctionalT, typename MutexT>
FunctionalT* functional_registry<FunctionalT, MutexT>::find(identifier_type id) const noexcept
{
    lock_guard guard(set_mtx_);
    auto it = set_.find(id);
    if (it != set_.end()) {
        return std::addressof(const_cast<FunctionalT&>(*it));
    }
    return nullptr;
}

template <typename FunctionalT, typename MutexT>
FunctionalT& functional_registry<FunctionalT, MutexT>::resolve(identifier_type id) noexcept
{
    lock_guard guard(set_mtx_);
    auto it = set_.find(id);
    if (it == set_.end()) {
        return const_cast<FunctionalT&>(*set_.insert(it, FunctionalT{ std::move(id) }));
    }
    return const_cast<FunctionalT&>(*it);
}

}
