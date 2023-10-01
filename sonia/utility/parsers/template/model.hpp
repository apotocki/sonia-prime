//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

namespace sonia::parsers::tmpl {

class model
{
    model(model const&) = delete;
    model& operator= (model const&) = delete;

public:
    model() = default;

    enum class state_type
    {
        TEXT = 1,
        EXPR,
        CODE
    };

    state_type state;

    template <typename RangeT>
    void push_text(RangeT const& rng)
    {
        using std::begin;
        using std::end;
        push_text(begin(rng), end(rng));
    }

    template <typename IteratorT>
    void push_text(IteratorT b, IteratorT e)
    {
        std::copy(b, e, std::back_inserter(tempstr_));
    }

    void commit_text()
    {
        tempstr_.clear();
    }

    void commit_expr()
    {
        tempstr_.clear();
    }

    void commit_code()
    {
        tempstr_.clear();
    }

protected:
    std::vector<char> tempstr_;
};

}
