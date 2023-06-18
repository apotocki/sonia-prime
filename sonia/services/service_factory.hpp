//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/function.hpp"

#include "sonia/services.hpp"

namespace sonia::services {

class basic_service_factory : public service_factory
{
public:
    shared_ptr<service> create(std::string_view nm) const override;

    void register_service_factory(std::string_view, function<shared_ptr<service>()> const&);

private:
    using factories_type = boost::unordered_map<std::string, function<shared_ptr<service>()>, hasher>;
    factories_type named_factories_;

    mutable sonia::fibers::mutex named_factories_mtx_;
};

}
