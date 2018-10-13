//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_BUNDLE_HPP
#define SONIA_SERVICES_BUNDLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>
#include "sonia/function.hpp"
#include "service.hpp"
#include "builder.hpp"

namespace sonia { namespace services {

class bundle 
    : public service
    , public services::builder
{
public:
    bundle();
    ~bundle() override {}

    void open() override;

    virtual string_view build_id() const noexcept = 0;
    virtual void init() = 0;

    void set_handle(void * h) { libhandle_ = h; }

    shared_ptr<service> build(json_object const&) override;

protected:
    template <typename ServiceT>
    void install(string_view nm);

private:
    void * libhandle_;
    boost::unordered_map<std::string, function<shared_ptr<service>(json_object const&)>> builders_;
};

}}

#endif // SONIA_SERVICES_BUNDLE_HPP
