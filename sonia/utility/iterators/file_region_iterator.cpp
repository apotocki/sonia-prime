//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "file_region_iterator.hpp"

#include <cstring>

namespace boost { namespace interprocess { namespace detail {} namespace ipcdetail {} } }

namespace sonia {

namespace ipc = boost::interprocess;

file_mapping_holder::fm_cache::fm_cache(boost::filesystem::path const& path, boost::interprocess::mode_t m)
{
    fm = boost::interprocess::file_mapping(path.string().c_str(), m);
}

file_mapping_holder::file_mapping_holder(boost::filesystem::path const& path, boost::interprocess::mode_t mode, size_t rsz)
{
    using namespace ipc::detail;
    using namespace ipc::ipcdetail;

    try {
        fmc_ = make_shared<fm_cache>(path, mode);
        ipc::offset_t result = 0u;
        get_file_size(file_handle_from_mapping_handle(fmc_->fm.get_mapping_handle()), result);
        fmc_->size = static_cast<uint64_t>(result);

        size_t pagesz = ipc::mapped_region::get_page_size();
        fmc_->region_size = pagesz * ((rsz + pagesz - 1) / pagesz);
    } catch (std::exception const& e) {
        throw exception("can't create a file mapping for '%1$S', mode: %2%, error: %3%"_fmt % path.string() % (int)mode % e.what());
    }
}

void file_mapping_holder::truncate(uint64_t sz)
{
    using namespace ipc::detail;
    using namespace ipc::ipcdetail;

    truncate_file(file_handle_from_mapping_handle(file_mapping().get_mapping_handle()), sz);
    fmc_->size = sz;
}

file_region_descriptor::file_region_descriptor(boost::filesystem::path const& path, boost::interprocess::mode_t mode, uint64_t offset, size_t region_sz)
    : file_mapping_holder(path, mode, region_sz), fileoffset_(offset)
{
    region_ = create_region(offset, region_size());
    cursor_ = nullptr; // not initialized
}

file_region_descriptor::file_region_descriptor(boost::intrusive_ptr<file_region_descriptor> prev)
    : file_mapping_holder(static_cast<file_mapping_holder const&>(*prev)), fileoffset_(prev->fileoffset_)
{
    cursor_ = nullptr;
}

file_region_descriptor::~file_region_descriptor()
{
    // to avoid stack overflow for long chains
    while (next_) {
        boost::intrusive_ptr<file_region_descriptor> tmp = std::move(next_->next_);
        next_ = std::move(tmp);
    }
    flush();
}

bool file_region_descriptor::is_cursor_at_the_end_or_null() const
{
    return !cursor_ || cursor_ == reinterpret_cast<char*>(region_.get_address()) + region_.get_size();
}

void file_region_descriptor::update_region_size(size_t sz)
{
	if (sz != region_size()) {
		region_ = ipc::mapped_region();
		truncate(fileoffset_ + sz);
		region_ = ipc::mapped_region(file_mapping(), mode(), fileoffset_, sz);
	}
}

void file_region_descriptor::next_from(file_region_descriptor const& previous)
{
    uint64_t nextoffset = previous.get_region_size() + previous.fileoffset_;
	region_ = ipc::mapped_region();
    region_ = create_region(nextoffset, region_size());
    cursor_ = nullptr;
    fileoffset_ = nextoffset;
}

void file_region_descriptor::previous_from(file_region_descriptor const& next)
{
    uint64_t prevoffset = next.fileoffset_ > region_size() ? next.fileoffset_ - region_size() : 0;
    size_t prev_reg_size = (size_t)(next.fileoffset_ - prevoffset);
    region_ = prev_reg_size ? create_region(prevoffset, prev_reg_size) : ipc::mapped_region();
    cursor_ = nullptr;
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
        } else {
            region_size = fsz - offset;
            if (region_size == 0) return ipc::mapped_region();
        }
    }

    return ipc::mapped_region(file_mapping(), mode(), offset, region_size);
}

size_t file_region_descriptor::get_region_size() const
{
    return region_.get_size();
}

array_view<char> file_region_descriptor::get() const
{
    if (cursor_) {
        return {cursor_, (size_t)(reinterpret_cast<char*>(region_.get_address()) + region_.get_size() - cursor_)};
    } else {
        return {reinterpret_cast<char*>(region_.get_address()), region_.get_size()};
    }
}

bool file_region_descriptor::empty() const
{
    return 0 == region_.get_size();
}

void file_region_descriptor::flush()
{
    if (cursor_) {
        update_region_size(cursor_ - reinterpret_cast<char*>(region_.get_address()));
    }
}

file_region_iterator_base::file_region_iterator_base(bool readonly, boost::filesystem::path const& path, uint64_t offset, size_t least_region_sz)
{
    namespace ipc = boost::interprocess;

    region_.reset(new file_region_descriptor(
        path,
        readonly ? ipc::read_only : ipc::read_write,
        offset, least_region_sz
    ), false);
}

void file_region_iterator_base::increment()
{
    if (!region_->is_cursor_at_the_end_or_null()) return;

    boost::intrusive_ptr<file_region_descriptor> next = region_->get_next();
    if (next) {
        region_ = next;
    } else if (1 == region_->refs_) {
        region_->next_from(*region_);
    } else {
        auto nextreg = boost::intrusive_ptr(new file_region_descriptor(region_), false);
        nextreg->next_from(*region_);
        region_->set_next(nextreg);
        region_ = std::move(nextreg);
    }
}

void file_region_iterator_base::decrement()
{
    if (region_->get_cursor()) {
        region_->set_cursor(nullptr);
        return;
    }
    //boost::intrusive_ptr<file_region_descriptor> previous = region_->get_previous();
    //if (previous) {
    //    region_ = previous;
    if (1 == region_->refs_) {
        region_->previous_from(*region_);
    } else {
        auto prevreg = boost::intrusive_ptr(new file_region_descriptor(region_), false);
        prevreg->previous_from(*region_);
        //region_->set_previous(prevreg);
        region_ = std::move(prevreg);
    }
}

void file_region_iterator_base::flush(char * writ)
{
    if (region_) {
        region_->set_cursor(writ);
    }
}

// post condition: region_->cursor_ != nullptr; it's important for the correctness of increment()
void file_region_iterator_base::set(array_view<const char> data)
{
    array_view<char> dest_raw = region_->get();
    size_t bytes_to_write = data.size();
    BOOST_ASSERT (data.begin() != dest_raw.begin());
    //if (data.begin() == dest_raw.begin()) {
    //    BOOST_ASSERT(bytes_to_write <= dest_raw.size());
    //    region_->set_cursor(dest_raw.begin() + bytes_to_write);
    //    return;
    //}

    char * writ = nullptr;
    for (;;) {
        size_t cnt = (std::min)(bytes_to_write, dest_raw.size());
        writ = (char*)std::memmove(dest_raw.begin(), data.begin(), cnt) + cnt;
        region_->set_cursor(writ);
        bytes_to_write -= cnt;
        if (!bytes_to_write) break;
        data.advance_front(cnt);
        increment();
        dest_raw = region_->get();
    }
    //region_->update_region_size((size_t)(writ - dest_raw.begin()));
}

}
