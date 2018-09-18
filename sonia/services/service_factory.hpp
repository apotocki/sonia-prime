//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_FACTORY_HPP
#define SONIA_SERVICE_FACTORY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>

#include "sonia/thread.hpp"
#include "sonia/function.hpp"

#include "sonia/services.hpp"

namespace sonia { namespace services {

class basic_service_factory : public service_factory {
public:
    service_descriptor create(string_view nm) const override;

    void register_service_factory(string_view, function<service_descriptor()> const&);

private:
    typedef boost::unordered_map<std::string, function<service_descriptor()>> factories_type;
    factories_type named_factories_;

    mutable fibers::mutex named_factories_mtx_;
};

}}

#endif // SONIA_SERVICE_FACTORY_HPP
