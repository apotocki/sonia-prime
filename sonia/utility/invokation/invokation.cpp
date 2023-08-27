//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "invokation.hpp"
#include "sonia/singleton.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/logger/logger.hpp"
#include <boost/unordered_map.hpp>

namespace sonia::invokation {

class blob_manager : public singleton
{
public:
    void pin(shared_ptr<uint8_t> pval)
    {
        lock_guard guard(blobs_mtx_);
        auto it = blobs_.find(pval.get());
        if (it != blobs_.end()) {
            ++it->second.second;
            GLOBAL_LOG_TRACE() << "addref for the blob at: " << std::hex << (uint64_t)pval.get();
        } else {
            auto address = pval.get();
            blobs_.insert(it, std::pair{address, std::pair{std::move(pval), 1}});
            GLOBAL_LOG_TRACE() << "a new blob was pinned at: " << std::hex << (uint64_t)address;
        }
    }

    void allocate(blob_result * b)
    {
        if (b->need_unpin == 0 && b->size > 0) {
            shared_ptr<uint8_t> data(new uint8_t[b->size], [](uint8_t* p) { delete[] p; });
            if (b->data) {
                std::memcpy(data.get(), b->data, b->size);
            }
            b->data = data.get();
            b->need_unpin = 1;
            pin(std::move(data));
        }
    }

    void pin(blob_result * b)
    {
        if (b->type == blob_type::blob || (b->is_array && b->type == blob_type::string)) {
            blob_result* rdata = const_cast<blob_result*>(reinterpret_cast<blob_result const*>(b->data));
            blob_result const* endrdata = reinterpret_cast<blob_result const*>(b->data) + b->size / sizeof(blob_result);
            for (; rdata != endrdata; ++rdata) {
                pin(rdata);
            }
        }
        if (b->need_unpin) {
            lock_guard guard(blobs_mtx_);
            auto it = blobs_.find(b->data);
            if (it != blobs_.end()) {
                ++it->second.second;
            } else {
                GLOBAL_LOG_DEBUG() << "a blob was not found for pinning at: " << std::hex << (uint64_t)b->data;
            }
        }
    }

    void unpin(blob_result * b) noexcept
    {
        if (b->type == blob_type::blob || (b->is_array && b->type == blob_type::string)) {
            blob_result* rdata = const_cast<blob_result*>(reinterpret_cast<blob_result const*>(b->data));
            blob_result const* endrdata = reinterpret_cast<blob_result const*>(b->data) + b->size / sizeof(blob_result);
            for (; rdata != endrdata; ++rdata) {
                unpin(rdata);
            }
        }
        
        if (b->need_unpin) {
            //GLOBAL_LOG_DEBUG() << "a blob to unpin: " << b;
            b->need_unpin = 0;
            lock_guard guard(blobs_mtx_);
            auto it = blobs_.find(b->data);
            if (it != blobs_.end()) {
                if (--it->second.second == 0) {
                    blobs_.erase(it);
                    GLOBAL_LOG_TRACE() << "a blob was unpinned at: " << std::hex << (uint64_t)b->data;
                }
            } else {
                GLOBAL_LOG_ERROR() << "a blob was not found for unpinning at: " << std::hex << (uint64_t)b->data;
                //THROW_INTERNAL_ERROR("a blob was not found for unpinning %1%"_fmt % b);
            }
        }
    }

private:
    mutable fibers::mutex blobs_mtx_;
    boost::unordered_map<const void*, std::pair<shared_ptr<uint8_t>, uint32_t>> blobs_; // start of blob -> {blob, pin count}
};

}

void blob_result_allocate(blob_result * b)
{
    sonia::as_singleton<sonia::invokation::blob_manager>()->allocate(b);
}

void blob_result_pin(blob_result * b)
{
    sonia::as_singleton<sonia::invokation::blob_manager>()->pin(b);
}

void blob_result_unpin(blob_result * b)
{
    sonia::as_singleton<sonia::invokation::blob_manager>()->unpin(b);
}
