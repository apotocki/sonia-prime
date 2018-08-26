//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "parameters_description.hpp"

#include <algorithm>

namespace sonia { namespace parameters {

parameters_description_init::parameters_description_init(parameters_description * pd)
    : pd_(pd)
{

}

parameters_description_init & parameters_description_init::operator()(const char * name, const char * descr)
{
    return this->operator()(name, new value_descriptor, descr);
}

parameters_description_init & parameters_description_init::operator()(const char * name, value_descriptor *, const char * descr)
{
    shared_ptr<value_descriptor> vd(new value_descriptor);
    vd->set_name(name);
    if (descr) vd->set_description(descr);
    pd_->vds_.push_back(std::move(vd));
    return *this;
}

parameters_description_init parameters_description::add_parameters() {
    return parameters_description_init(this);
}

void parameters_description::for_each(function<void(value_descriptor const&)> const& ftor) const
{
    std::for_each(vds_.cbegin(), vds_.cend(), [&ftor](auto v) { ftor(*v); });
}

}}