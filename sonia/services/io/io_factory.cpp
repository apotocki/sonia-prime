//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "factory.hpp"

#include "sonia/sal.hpp"

namespace sonia { namespace io {

void factory::open(uint32_t thr_cnt)
{
    initialize_impl(thr_cnt);
    threads_.reserve(thr_cnt);

    for (size_t i = 0; i < thr_cnt; ++i) {
        threads_.push_back(thread([this] { thread_proc(); }));
        sonia::sal::set_thread_name(threads_.back().get_id(), ("%1% factory thread #%2%"_fmt % name() % i).str());
    }
}

void factory::close()
{
    unique_lock lk(close_mtx_);
    //LOG_INFO(logger()) << "CLOSE: " << qsz.load();
    if (qsz.load() >= 0) {
        if (0 == qsz.fetch_add(qsz_min_value)) {
            impl_data_->park_threads();
        }
    }
    close_cond_.wait(lk, [this] { return threads_.empty(); });
}

size_t factory::thread_count() const
{
    return threads_.size();
}

void factory::on_add_callback()
{
    long qszval = qsz.fetch_add(1);
    if (qszval < 0) {
        if (qsz_min_value != qszval) {
            on_release_callback();
        } else {
            // if qsz_min_value == qszval then initiate_thread_parking was already called, 
            // so we just keep it actual
            --qsz;
        }
        throw closed_exception("io::factory");
    }
}

void factory::on_release_callback() noexcept
{
    if ((qsz_min_value + 1) == qsz.fetch_sub(1)) {
        impl_data_->park_threads();
    }
}

void factory::join_threads()
{
    for (thread & t : threads_) {
        t.join();
    }
    unique_lock lk(close_mtx_);
    threads_.clear();
    close_cond_.notify_one();
}

}}
