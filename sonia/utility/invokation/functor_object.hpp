//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "invokation.hpp"
#include "invokable.hpp"

namespace sonia::invokation {

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

class invokable_functor_object : public functor_object
{
    shared_ptr<invokable> invokable_;
    small_string name_;

public:
    invokable_functor_object(shared_ptr<invokable> inv, small_string name)
        : invokable_{ std::move(inv) }
        , name_ { std::move(name) }
    {}

    smart_blob operator()(std::span<const blob_result> args) override
    {
        return invokable_->invoke(name_, args);
    }
};

class weak_invokable_functor_object : public functor_object
{
    weak_ptr<invokable> invokable_;
    small_string name_;

public:
    weak_invokable_functor_object(shared_ptr<invokable> inv, small_string name)
        : invokable_{ std::move(inv) }
        , name_{ std::move(name) }
    {}

    smart_blob operator()(std::span<const blob_result> args) override
    {
        if (auto obj = invokable_.lock(); obj) {
            return obj->invoke(name_, args);
        }
        return {};
    }
};

}
