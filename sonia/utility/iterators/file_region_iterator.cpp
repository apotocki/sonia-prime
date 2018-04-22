//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/iterators/file_region_iterator.hpp"

#include <cstring>

namespace boost { namespace interprocess { namespace detail {} namespace ipcdetail {} } }

namespace sonia {

namespace ipc = boost::interprocess;

file_mapping_holder::file_mapping_holder(const char* filename, boost::interprocess::mode_t mode, size_t rsz)
{
    using namespace ipc::detail;
    using namespace ipc::ipcdetail;

    fmc_ = make_shared<fm_cache>();
    fmc_->fm = boost::interprocess::file_mapping(filename, mode);
    fmc_->mode = mode;

    ipc::offset_t result = 0u;
    get_file_size(file_handle_from_mapping_handle(fmc_->fm.get_mapping_handle()), result);
    fmc_->size = static_cast<uint64_t>(result);

    size_t pagesz = ipc::mapped_region::get_page_size();
    fmc_->region_size = pagesz * ((rsz + pagesz - 1) / pagesz);
}

void file_mapping_holder::truncate(uint64_t sz)
{
    using namespace ipc::detail;
    using namespace ipc::ipcdetail;

    truncate_file(file_handle_from_mapping_handle(file_mapping().get_mapping_handle()), sz);
    fmc_->size = sz;
}

file_region_descriptor::file_region_descriptor(const char* filename, ipc::mode_t mode, uint64_t offset, size_t region_sz)
    : file_mapping_holder(filename, mode, region_sz), fileoffset_(offset)
{
    region_ = create_region(offset, region_size());
}

file_region_descriptor::file_region_descriptor(shared_ptr<file_region_descriptor> prev)
    : file_mapping_holder(static_cast<file_mapping_holder const&>(*prev)), fileoffset_(prev->fileoffset_), previous_(prev)
{

}

file_region_descriptor::~file_region_descriptor()
{
    // to avoid stack overflow for long chains
    while (next_) {
        shared_ptr<file_region_descriptor> tmp = std::move(next_->next_);
        next_ = std::move(tmp);
    }
}

void file_region_descriptor::update_region_size(size_t sz)
{
    region_ = ipc::mapped_region();
    truncate(fileoffset_ + sz);
    region_ = ipc::mapped_region(file_mapping(), mode(), fileoffset_, sz);
}

void file_region_descriptor::next_from(file_region_descriptor const& previous)
{
    uint64_t nextoffset = previous.get().size() + previous.fileoffset_;
    region_ = create_region(nextoffset, region_size());
    fileoffset_ = nextoffset;
}

void file_region_descriptor::previous_from(file_region_descriptor const& next)
{
    uint64_t prevoffset = next.fileoffset_ > region_size() ? next.fileoffset_ - region_size() : 0;
    size_t prev_reg_size = (size_t)(next.fileoffset_ - prevoffset);
    region_ = prev_reg_size ? create_region(prevoffset, prev_reg_size) : ipc::mapped_region();
    fileoffset_ = prevoffset;
}

ipc::mapped_region file_region_descriptor::create_region(uint64_t offset, size_t region_size)
{
    using namespace ipc::detail;
    using namespace ipc::ipcdetail;

    uint64_t fsz = file_size();
    if (offset + region_size > fsz) {
        if (mode() == ipc::read_write) {
            truncate(offset + region_size);
            fsz = offset + region_size;
        } else {
            region_size = fsz - offset;
            if (region_size == 0) return ipc::mapped_region();
        }
    }

    return ipc::mapped_region(file_mapping(), mode(), offset, region_size);
}

array_view<char> file_region_descriptor::get() const {
    return array_view<char>(reinterpret_cast<char*>(region_.get_address()), region_.get_size());
}

bool file_region_descriptor::empty() const
{
    return 0 == region_.get_size();
}

void file_region_iterator_base::increment()
{
    shared_ptr<file_region_descriptor> next = region_->get_next();
    if (next) {
        region_ = next;
    } else if (1 == region_.use_count()) {
        region_->next_from(*region_);
    } else {
        auto nextreg = make_shared<file_region_descriptor>(region_);
        nextreg->next_from(*region_);
        region_->set_next(nextreg);
        region_ = std::move(nextreg);
    }
}

void file_region_iterator_base::decrement() {
    shared_ptr<file_region_descriptor> previous = region_->get_previous();
    if (previous) {
        region_ = previous;
    } else if (1 == region_.use_count()) {
        region_->previous_from(*region_);
    } else {
        auto prevreg = make_shared<file_region_descriptor>(region_);
        prevreg->previous_from(*region_);
        region_->set_previous(prevreg);
        region_ = std::move(prevreg);
    }
}

void file_region_iterator_base::set(array_view<const char> src_raw)
{
    array_view<char> dest_raw = region_->get();
    if (src_raw.begin() != dest_raw.begin()) {
        size_t bytes_to_write = src_raw.size();
        char * writ = nullptr;
        for (;;) {
            size_t cnt = (std::min)(src_raw.size(), dest_raw.size());
            writ = (char*)std::memcpy(dest_raw.begin(), src_raw.begin(), cnt) + cnt;
            bytes_to_write -= cnt;
            if (!bytes_to_write) break;
            increment();
            dest_raw = region_->get();
        }
        region_->update_region_size((size_t)(writ - dest_raw.begin()));
    } else {
        region_->update_region_size(src_raw.size());
    }
}

}
