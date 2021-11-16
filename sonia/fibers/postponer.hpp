//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "context.hpp"

namespace sonia::fibers {

class postponer
{
    context* postponned_ctx_{nullptr};

public:
    postponer() = default;

    explicit operator bool() const noexcept { return !!postponned_ctx_; }

    template <typename LockT>
    void postpone(LockT & lck)
    {
        postponned_ctx_ = context::active();
        postponned_ctx_->suspend(lck);
    }

    void postpone();
    void resume_or_schedule();
    void schedule();

    bool is_pinned_context() const noexcept
    {
        BOOST_ASSERT(postponned_ctx_);
        return postponned_ctx_->is_context(type::pinned_context);
    }
};

}
