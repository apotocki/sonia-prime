//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
//#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/composite_key.hpp>

#include "sonia/span.hpp"
#include "sonia/optional.hpp"
#include "sonia/concurrency.hpp"
#include "identifier_builder.hpp"
#include "qname.hpp"

namespace sonia::lang {

template <typename IdentifierT, typename MutexT = dummy_mutex_t>
class parameterized_identifier_registry
{
    using qname_t = qname<IdentifierT>;
    using parameter_vec_t = boost::container::small_vector<qname_t, 2>;
    //using element_t = std::tuple<IdentifierT, boost::container::small_vector<qname_t, 2>>;

    struct item
    {
        //IdentifierT master_id;
        IdentifierT id; // parameterized id
        parameter_vec_t parameters;
        span<const qname_t> pspan() const { return parameters; }
    };

    //using composite_hasher_t = boost::multi_index::composite_key_hash<hasher, hasher>;
    //using composite_equal_to_t = boost::multi_index::composite_key_equal_to<std::equal_to<>, range_equal>;

    /*
    using set_t = boost::multi_index::multi_index_container<
        item,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::composite_key<
                    item,
                    //boost::multi_index::member<item, IdentifierT, &item::id>,
                    boost::multi_index::member<item, const parameter_vec_t, &item::parameters>
                    //boost::multi_index::const_mem_fun<item, span<const qname_t>, &item::pspan>
                >,
                composite_hasher_t,
                composite_equal_to_t
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::member<item, IdentifierT, &item::id>
            >
        >
    >;
    */

    using set_t = boost::multi_index::multi_index_container<
        item,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::member<item, const parameter_vec_t, &item::parameters>,
                hasher
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::member<item, IdentifierT, &item::id>,
                hasher
            >
        >
    >;

public:
    explicit parameterized_identifier_registry(identifier_builder<IdentifierT>& ib) : ib_{ ib } {}
    IdentifierT resolve(/*IdentifierT id,*/ span<qname_t> params);
    //optional<std::tuple<IdentifierT, span<const qname_t>>> resolve(IdentifierT masterid) const noexcept;
    optional<span<const qname_t>> resolve(IdentifierT masterid) const noexcept;

private:
    identifier_builder<IdentifierT>& ib_;

    set_t set_;
    mutable MutexT set_mtx_;
};

template <typename IdentifierT,typename MutexT>
IdentifierT parameterized_identifier_registry<IdentifierT, MutexT>::resolve(/*IdentifierT id,*/ span<qname_t> params)
{
    lock_guard guard(set_mtx_);
    auto it = set_.find(params, hasher{}, range_equal{});
    //auto it = set_.find(std::tuple{ id, params }, composite_hasher_t{}, composite_equal_to_t{});
    if (it == set_.end()) {
        auto id = ib_();
        set_.insert(it, item{ id, parameter_vec_t{params.begin(), params.end()} });
        return id;
    }
    return it->id;
}

template <typename IdentifierT, typename MutexT>
//optional<std::tuple<IdentifierT, span<const qname<IdentifierT>>>> parameterized_identifier_registry<IdentifierT, MutexT>::resolve(IdentifierT masterid) const noexcept
optional<span<const qname<IdentifierT>>> parameterized_identifier_registry<IdentifierT, MutexT>::resolve(IdentifierT id) const noexcept
{
    auto const& slice = set_.get<1>();
    lock_guard guard(set_mtx_);
    auto it = slice.find(id);
    if (it == slice.end()) {
        return nullopt;
    }
    return span{it->parameters};
}

#if  0 
void test()
{
    std::vector<identifier> tt0, tt1, tt2;
    for (uint32_t i = 0; i < 1000; ++i) {
        qname ll1[1] = { qname{identifier{ i }} };
        qname ll0[2] = { qname{identifier{ i }}, qname{identifier{ i + 1 }} };
        auto t0 = unit_.piregistry().resolve(identifier{ 1 }, ll0);
        auto t1 = unit_.piregistry().resolve(identifier{ 2 }, ll1);
        auto t2 = unit_.piregistry().resolve(identifier{ 2 }, ll0);
        auto t3 = unit_.piregistry().resolve(identifier{ 1 }, ll0);

        BOOST_ASSERT(t0 == t3);
        BOOST_ASSERT(t0 != t1);
        BOOST_ASSERT(t0 != t2);
        BOOST_ASSERT(t2 != t1);

        tt0.push_back(t0);
        tt1.push_back(t1);
        tt2.push_back(t2);
    }

    for (uint32_t i = 0; i < 1000; ++i) {
        qname ll1[1] = { qname{identifier{ i }} };
        qname ll0[2] = { qname{identifier{ i }}, qname{identifier{ i + 1 }} };

        auto t0 = unit_.piregistry().resolve(identifier{ 1 }, ll0);
        auto t1 = unit_.piregistry().resolve(identifier{ 2 }, ll1);
        auto t2 = unit_.piregistry().resolve(identifier{ 2 }, ll0);
        auto t3 = unit_.piregistry().resolve(identifier{ 1 }, ll0);
        BOOST_ASSERT(t0 == t3);

        BOOST_ASSERT(tt0[i] == t0);
        BOOST_ASSERT(tt1[i] == t1);
        BOOST_ASSERT(tt2[i] == t2);

        optional<std::tuple<identifier, span<const qname>>> optval = unit_.piregistry().resolve(t0);
        BOOST_ASSERT(optval && std::get<0>(*optval) == identifier{ 1 } && range_equal{}(std::get<1>(*optval), span{ ll0 }));
        optval = unit_.piregistry().resolve(t1);
        BOOST_ASSERT(optval && std::get<0>(*optval) == identifier{ 2 } && range_equal{}(std::get<1>(*optval), span{ ll1 }));
        optval = unit_.piregistry().resolve(t2);
        BOOST_ASSERT(optval && std::get<0>(*optval) == identifier{ 2 } && range_equal{}(std::get<1>(*optval), span{ ll0 }));
    }
}

#endif
}
