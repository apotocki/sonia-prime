//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PARAMETERS_DESCRIPTION_HPP
#define SONIA_PARAMETERS_DESCRIPTION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/shared_ptr.hpp"
#include "sonia/function.hpp"

#include "value_descriptor.hpp"

namespace sonia { namespace parameters {

class parameters_description;

class parameters_description_init {
public:
    explicit parameters_description_init(parameters_description * pd);

    parameters_description_init & operator()(const char * name, const char * descr = nullptr);
    parameters_description_init & operator()(const char * name, value_descriptor *, const char * descr = nullptr);

private:
    parameters_description * pd_;
};

class parameters_description {
    friend class parameters_description_init;

public:
    parameters_description_init add_parameters();

    void for_each(function<void(value_descriptor const&)> const& ftor) const;

private:
    std::vector<shared_ptr<value_descriptor>> vds_;
};

}}

#endif // SONIA_PARAMETERS_DESCRIPTION_HPP
