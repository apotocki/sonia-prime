//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include "sonia/config.hpp"
#include "sonia/fibers/context_initializer.hpp"
#include "sonia/fibers/scheduler.hpp"

namespace sonia::fibers {

BOOST_NOINLINE SONIA_PRIME_API context_initializer::context_descriptor* context_initializer::get_descriptor() noexcept {
    return &context_descriptor_;
}

context_initializer::~context_initializer()
{
    context_descriptor* pd = get_descriptor();
    if (context* main_ctx = pd->main_; main_ctx) {
        BOOST_ASSERT(main_ctx->is_context(type::main_context));
        scheduler* sched = main_ctx->get_scheduler();
        delete sched;
        delete main_ctx;
        pd->main_ = nullptr;
    }
}

// zero-initialization
thread_local context_initializer::context_descriptor context_initializer::context_descriptor_{ nullptr, nullptr };
//thread_local context * context_initializer::active_{ nullptr };
//thread_local std::size_t context_initializer::counter_{ 0 };

// to free resources
thread_local static context_initializer ctx_initializer;


//void clean()
//{
//    auto* pd = context_initializer::get_descriptor();
//    ctx_initializer.~context_initializer();
//}

}