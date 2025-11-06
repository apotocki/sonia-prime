//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

class functor_object : public object
{
public:
    virtual smart_blob operator()(std::span<const blob_result> args) = 0;
    smart_blob operator()(std::initializer_list<const blob_result> args)
    {
        return this->operator()(std::span{ args });
    }
};

class blob_functor_object : public functor_object
{
    smart_blob blob_;

public:
    explicit blob_functor_object(blob_result const& br)
        : blob_{ br }
    {}

    smart_blob operator()(std::span<const blob_result> args) override
    {
        return blob_.as<functor_object>()(args);
    }
};

class invocable_functor_object : public functor_object
{
    shared_ptr<invocable> invocable_;
    small_string name_;

public:
    invocable_functor_object(shared_ptr<invocable> inv, small_string name)
        : invocable_{ std::move(inv) }
        , name_ { std::move(name) }
    {}

    smart_blob operator()(std::span<const blob_result> args) override
    {
        return invocable_->invoke(name_, args);
    }
};

class weak_invocable_functor_object : public functor_object
{
    weak_ptr<invocable> invocable_;
    small_string name_;

public:
    weak_invocable_functor_object(shared_ptr<invocable> inv, small_string name)
        : invocable_{ std::move(inv) }
        , name_{ std::move(name) }
    {}

    smart_blob operator()(std::span<const blob_result> args) override
    {
        if (auto obj = invocable_.lock(); obj) {
            return obj->invoke(name_, args);
        }
        return {};
    }
};

}
