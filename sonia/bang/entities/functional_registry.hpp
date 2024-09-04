//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
//#include <boost/multi_index/ordered_index.hpp>
//#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/global_fun.hpp>

#include "sonia/concurrency.hpp"

namespace sonia::lang {

template <typename FunctionalT, typename MutexT = dummy_mutex_t>
class functional_registry
{
    using identifier_type = typename FunctionalT::identifier_type;
    using qname_view_t = typename FunctionalT::qname_view_type;

public:
    FunctionalT* find(qname_view_t) const noexcept;
    FunctionalT& resolve(qname_view_t) noexcept;
    FunctionalT& resolve(identifier_type qid) const;

private:
    static qname_view_t get_name(shared_ptr<FunctionalT> const& pf) { return pf->name(); }

    using set_t = boost::multi_index::multi_index_container<
        shared_ptr<FunctionalT>,
        boost::multi_index::indexed_by <
            boost::multi_index::random_access<>,
            boost::multi_index::hashed_unique<
                boost::multi_index::global_fun<shared_ptr<FunctionalT> const&, qname_view_t, &get_name>
            >
        >
        //boost::multi_index::indexed_by<
        //    boost::multi_index::hashed_unique<
        //        boost::multi_index::const_mem_fun<FunctionalT, identifier_type, &FunctionalT::id>
        //    >
        //>
    >;

    set_t set_;
    mutable MutexT set_mtx_;
};

template <typename FunctionalT, typename MutexT>
FunctionalT* functional_registry<FunctionalT, MutexT>::find(qname_view_t qnv) const noexcept
{
    BOOST_ASSERT(qnv.is_absolute());
    lock_guard guard(set_mtx_);
    auto& plane = set_.template get<1>();
    auto it = plane.find(qnv);

    if (it != plane.end()) {
        return const_cast<FunctionalT*>(it->get());
    }
    return nullptr;
}

template <typename FunctionalT, typename MutexT>
FunctionalT& functional_registry<FunctionalT, MutexT>::resolve(qname_view_t qnv) noexcept
{
    BOOST_ASSERT(qnv.is_absolute());
    lock_guard guard(set_mtx_);
    auto& plane = set_.template get<1>();
    auto it = plane.find(qnv);
    if (it == plane.end()) {
        size_t r = set_.size();
        it = plane.insert(it, make_shared<FunctionalT>(identifier_type{r}, qnv));
    }
    return **it;
}

template <typename FunctionalT, typename MutexT>
FunctionalT& functional_registry<FunctionalT, MutexT>::resolve(identifier_type qid) const
{
    lock_guard guard(set_mtx_);
    return *set_.template get<0>().at(qid.raw());
}

}
