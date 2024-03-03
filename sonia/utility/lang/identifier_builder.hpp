//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <atomic>

namespace sonia::lang {

template <typename IdentifierT>
class identifier_builder
{
public:
    explicit identifier_builder(typename IdentifierT::value_type init_value = 1)
        : id_counter_ { init_value }
    {}

    IdentifierT operator()()
    {
        return IdentifierT{ id_counter_.fetch_add(1) };
    }

private:
    std::atomic<typename IdentifierT::value_type> id_counter_;
};

}
