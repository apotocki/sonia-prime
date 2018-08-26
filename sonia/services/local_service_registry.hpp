//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP
#define SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <mutex>
#include <tuple>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/global_fun.hpp>

#include "service.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/utility/persister.hpp"

namespace sonia {

class local_service_registry : public service_registry
{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int = 0) {
        ar & boost::serialization::make_nvp("counter", counter_)
           & boost::serialization::make_nvp("catalog", registry_);
    }

public:
    explicit local_service_registry(shared_ptr<persister> sp);

    service::id get_id(string_view) override;
    string_view get_name(service::id) const override;

private:
    void backup() const;
    void restore();

    mutable std::mutex mtx_;

    typedef std::tuple<std::string, service::id> reg_item_t;

    typedef boost::multi_index::multi_index_container<
        reg_item_t,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::global_fun<reg_item_t const&, std::string const&, &std::get<0>>,
                string_hasher
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::global_fun<reg_item_t const&, service::id const&, &std::get<1>>
            >
        >
    > registry_t;

    registry_t registry_;
    service::id counter_;
    shared_ptr<persister> state_persister_;
};

}

#endif // SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP
