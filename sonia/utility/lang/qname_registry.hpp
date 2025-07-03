//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/container/small_vector.hpp>

#include "sonia/concurrency.hpp"

#include "qname.hpp"

namespace sonia::lang {

template <typename QnameIdentifierT, typename MutexT = dummy_mutex_t>
class qname_registry
{
    using qname_view_t = qname_view<QnameIdentifierT>;
    using id_vec_t = boost::container::small_vector<QnameIdentifierT, 4>;

    using set_t = boost::multi_index::multi_index_container<
        id_vec_t,
        boost::multi_index::indexed_by<
            boost::multi_index::random_access<>,
            boost::multi_index::hashed_unique<boost::multi_index::identity<id_vec_t>, hasher>
        >
    >;

public:
    qname_identifier find(qname_view_t qnv)
    {
        BOOST_ASSERT(qnv.is_absolute());
        lock_guard guard(set_mtx_);
        auto& plane = set_.template get<1>();
        auto it = plane.find(qnv, hasher{}, range_equal{});
        if (it != plane.end()) {
            return qname_identifier{ (size_t)(boost::multi_index::project<0>(set_, it) - set_.begin()) };
        }
        return {};
    }

    qname_identifier resolve(qname_view_t qnv)
    {
        BOOST_ASSERT(qnv.is_absolute());
        lock_guard guard(set_mtx_);
        auto & plane = set_.template get<1>();
        auto it = plane.find(qnv, hasher{}, range_equal{});
        if (it == plane.end()) {
            size_t r = set_.size();
            plane.insert(it, id_vec_t{ qnv.begin(), qnv.end()});
            return qname_identifier{ r };
        }
        return qname_identifier{ (size_t)(boost::multi_index::project<0>(set_, it) - set_.begin()) };
    }

    qname_view_t resolve(qname_identifier qid) const
    {
        lock_guard guard(set_mtx_);
        auto sp = span<const QnameIdentifierT>{ set_.template get<0>().at(qid.raw()) };
        return qname_view{ sp };
    }

    qname_identifier concat(qname_identifier qid, QnameIdentifierT part_id)
    {
        qname rqn = resolve(qid) / part_id;
        return resolve(rqn);
    }

private:
    set_t set_;
    mutable MutexT set_mtx_;
};

}
