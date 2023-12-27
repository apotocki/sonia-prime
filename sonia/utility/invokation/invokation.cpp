//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "invokation.hpp"
#include "sonia/singleton.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/variant.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/logger/logger.hpp"
#include <boost/unordered_map.hpp>

namespace sonia::invokation {

class blob_manager : public singleton
{
    using blob_storage = variant<shared_ptr<uint8_t>, shared_ptr<sonia::invokation::invokable>>;

    struct address_retriever_visitor : static_visitor<const void*>
    {
        address_retriever_visitor() = default;
        template <typename T>
        inline const void* operator()(T const& obj) const noexcept { return obj.get(); }
    };

public:
    ~blob_manager()
    {
        if (!blobs_.empty()) {
            GLOBAL_LOG_ERROR() << "BLOB LEAKS: " << blobs_.size();
        }
    }

    void pin(blob_storage pval)
    {
        const void* ptr = apply_visitor(address_retriever_visitor{}, pval);
        lock_guard guard(blobs_mtx_);
        auto it = blobs_.find(ptr);
        if (it != blobs_.end()) {
            ++it->second.second;
            GLOBAL_LOG_TRACE() << "addref for the blob at: " << std::hex << (uint64_t)ptr;
        } else {
            blobs_.insert(it, std::pair{ ptr, std::pair{std::move(pval), 1}});
            GLOBAL_LOG_TRACE() << "a new blob was pinned at: " << std::hex << (uint64_t)ptr;
        }
    }

    void addref(blob_result* b)
    {
        lock_guard guard(blobs_mtx_);
        auto it = blobs_.find(b->bp.data);
        if (it != blobs_.end()) {
            ++it->second.second;
        } else {
            GLOBAL_LOG_DEBUG() << "a blob was not found for pinning at: " << std::hex << (uint64_t)b->bp.data;
        }
    }

    optional<blob_storage> releaseref(blob_result* b) noexcept
    {
        lock_guard guard(blobs_mtx_);
        auto it = blobs_.find(b->bp.data);
        if (it != blobs_.end()) {
            if (--it->second.second == 0) {
                blob_storage st = std::move(it->second.first);
                blobs_.erase(it);
                GLOBAL_LOG_TRACE() << "a blob was unpinned at: " << std::hex << (uint64_t)b->bp.data;
                return std::move(st);
            }
        } else {
            GLOBAL_LOG_ERROR() << "a blob '" << *b << "' was not found for unpinning at: " << std::hex << (uint64_t)b->bp.data;
            //THROW_INTERNAL_ERROR("a blob was not found for unpinning %1%"_fmt % *b);
        }
        return nullopt;
    }

private:
    mutable fibers::mutex blobs_mtx_;
    boost::unordered_map<const void*, std::pair<blob_storage, uint32_t>> blobs_; // start of blob -> {blob, pin count}
};

}

// allocates only top level array!!!
void blob_result_allocate(blob_result * b)
{
    if (b->need_unpin == 0 && !b->inplace_size && !!b->bp.size) {
        void const* ptr = b->bp.data;
        uint32_t sz = b->bp.size;
        static_assert(sizeof(b->ui8array) == 14);
        if (sz <= sizeof(b->ui8array)) {
            b->inplace_size = static_cast<uint8_t>(sz);
            if (ptr) {
                std::memcpy(b->ui8array, ptr, sz);
            }
        } else {
            sonia::shared_ptr<uint8_t> data(new uint8_t[sz], [](uint8_t* p) { delete[] p; });
            if (b->bp.data) {
                std::memcpy(data.get(), ptr, sz);
            }
            b->bp.data = data.get();
            b->need_unpin = 1;
            sonia::as_singleton<sonia::invokation::blob_manager>()->pin(std::move(data));
        }
        // elements should be pinned by callee
        /*
        if (b->type == blob_type::tuple) {
            blob_result* pblob = mutable_data_of<blob_result>(*b), *epblob = pblob + array_size_of<blob_result>(*b);
            for (; pblob != epblob; blob_result_pin(pblob++));
        }
        */
    }
}

void blob_result_pin(blob_result * b)
{
    if (b->need_unpin) {
        sonia::as_singleton<sonia::invokation::blob_manager>()->addref(b);
    }
}

void blob_result_unpin(blob_result * b)
{
    if (b->need_unpin) {
        auto optst = sonia::as_singleton<sonia::invokation::blob_manager>()->releaseref(b);
        b->need_unpin = 0;
        if (optst && b->type == blob_type::tuple) {
            blob_result* pblob = mutable_data_of<blob_result>(*b), * epblob = pblob + array_size_of<blob_result>(*b);
            for (; pblob != epblob; blob_result_unpin(pblob++));
        }
    }
}
