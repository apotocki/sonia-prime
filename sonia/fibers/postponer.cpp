//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/fibers/postponer.hpp"
#include "sonia/fibers/scheduler.hpp"

#include <boost/assert.hpp>

namespace sonia::fibers {

void postponer::postpone()
{
    postponned_ctx_ = context::active();
    postponned_ctx_->suspend();
}

void postponer::resume_or_schedule()
{
    //context* cur_ctx = context::active();
    //if (postponned_ctx_->is_context(type::pinned_context) || (postponned_ctx_->get_scheduler()->get_group()
    //    && postponned_ctx_->get_scheduler()->get_group() == cur_ctx->get_scheduler()->get_group()))
    //{
    //    cur_ctx->schedule(postponned_ctx_);
    //} else {
    //    postponned_ctx_->detach();
    //    cur_ctx->attach(postponned_ctx_);
    //    postponned_ctx_->resume(cur_ctx);
    //}

    context* cur_ctx = context::active();
    if (!postponned_ctx_->is_context(type::pinned_context)) {
        scheduler* psched = postponned_ctx_->get_scheduler();
        if (scheduler_group* pscg = psched->get_group(); pscg && pscg == cur_ctx->get_scheduler()->get_group()) {
            postponned_ctx_->detach();
            cur_ctx->attach(postponned_ctx_);
            postponned_ctx_->resume(cur_ctx);
            return;
        }
    }
    cur_ctx->schedule(postponned_ctx_);
}

void postponer::schedule()
{
    context::active()->schedule(postponned_ctx_);
}

}
