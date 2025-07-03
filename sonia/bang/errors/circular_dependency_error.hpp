//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/errors.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

class circular_dependency_error : public error
{
public:
    std::vector<error_storage> circle_items;
    void visit(error_visitor& vis) const override { vis(*this); }

    explicit circular_dependency_error(std::vector<error_storage> errs)
        : circle_items{ std::move(errs) }
    {}

    explicit circular_dependency_error(error_storage err)
        : circle_items{ std::move(err) }
    {}
};

}
