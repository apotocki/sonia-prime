//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/intrusive_ptr.hpp>

#include "sonia/filesystem.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/span.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

class file_region_iterator_base;

class file_mapping_holder
{
    struct fm_cache
    {
        boost::interprocess::file_mapping fm;
        uint64_t size;
        size_t region_size;
        fs::path filepath;
        fm_cache(fs::path const&, boost::interprocess::mode_t);
    };

public:
    file_mapping_holder(fs::path const& path, boost::interprocess::mode_t mode, size_t rsz);

    boost::interprocess::file_mapping & file_mapping() const { return fmc_->fm; }
    boost::interprocess::mode_t mode() const { return file_mapping().get_mode(); }
    fs::path const& file_path() const { return fmc_->filepath; }
    uint64_t file_size() const { return fmc_->size; }
    uint64_t region_size() const { return fmc_->region_size; }
    
    void truncate(uint64_t sz);

private:
    shared_ptr<fm_cache> fmc_;
};

/**
    file_region_descriptor instancies share the same file_mapping instance
*/
class file_region_descriptor
    : private file_mapping_holder
    , public enable_shared_from_this<file_region_descriptor>
{
    friend class file_region_iterator_base;

    uint64_t fileoffset_;
    boost::interprocess::mapped_region region_;
    char * cursor_;
    boost::intrusive_ptr<file_region_descriptor> next_;

    file_region_descriptor(file_region_descriptor const&) = delete;
    file_region_descriptor& operator=(file_region_descriptor const&) = delete;

public:
    file_region_descriptor(fs::path const& path, boost::interprocess::mode_t mode, uint64_t offset, size_t region_sz);

    file_region_descriptor(boost::intrusive_ptr<file_region_descriptor> prev);

    ~file_region_descriptor();

    span<char> get() const;
    void update_region_size(size_t);
    size_t get_region_size() const;

    boost::intrusive_ptr<file_region_descriptor> get_next() const { return next_; }
    void set_next(boost::intrusive_ptr<file_region_descriptor> next) { next_ = std::move(next); }

    //boost::intrusive_ptr<file_region_descriptor> get_previous() const { return previous_.lock(); }
    //void set_previous(boost::intrusive_ptr<file_region_descriptor> const& prev) { previous_ = prev; }

    void reset_offset(uint64_t offset);
    void next_from(file_region_descriptor const& previous);
    void previous_from(file_region_descriptor const& next);

    bool empty() const;
    uint64_t offset() const { return fileoffset_; }

    bool is_cursor_at_the_end_or_null() const;
    char * get_cursor() const { return cursor_; }
    void set_cursor(char * c) { cursor_ = c; }

    void flush();

    friend void intrusive_ptr_add_ref(file_region_descriptor * p)
    {
        ++p->refs_;
    }

    friend void intrusive_ptr_release(file_region_descriptor * p)
    {
        if (0 == --p->refs_) {
            delete p;
        }
    }

private:
    unsigned int refs_{1};
    boost::interprocess::mapped_region create_region(uint64_t offset, size_t region_size);
};

class file_region_iterator_base
{
protected:
    void increment();
    void decrement();
    size_t advance_offset(std::ptrdiff_t); // returns offset in the applied region

    void flush(char*);
    void set(span<const char> data);

public:
    bool empty() const { return !region_ || region_->empty(); }
    uint64_t offset() const { return region_ ? region_->offset() : 0; }
    uint64_t max_offset() const { return region_ ? region_->file_size() : 0; }

    bool operator== (file_region_iterator_base const& rhs) const
    {
        if (empty()) return rhs.empty();
        return !rhs.empty() && region_ == rhs.region_;
    }

    file_region_iterator_base() = default;
    file_region_iterator_base(bool readonly, fs::path const& path, uint64_t offset, size_t least_region_sz);

protected:
    boost::intrusive_ptr<file_region_descriptor> region_;
};

template <typename T>
class file_region_iterator
    : public file_region_iterator_base
{
public:
    using value_type = span<T>;
    using iterator_category = std::random_access_iterator_tag;
    /*
    using iterator_category = conditional_t <
        is_const_v<T>,
        std::bidirectional_iterator_tag,
        boost::bidirectional_traversal_tag
    >;
    */
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type;

    template <class, class> friend class ptr_proxy_wrapper;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<file_region_iterator const*, value_type>>;

    static constexpr bool is_readonly = is_const_v<T>;

    span<T> get_dereference() const
    {
        return {b_, e_};
    }

    void set_dereference(span<const T> data)
    {
        BOOST_ASSERT(is_subset_of(data, span<const T>{b_, e_}));
        b_ = b_ + (data.data() - b_);
        e_ = b_ + data.size();
    }

    void init()
    {
        span<char> raw = region_->get();
        BOOST_ASSERT(0 == raw.size() % sizeof(T));
        b_ = (T*)raw.data();
        e_ = b_ + raw.size() / sizeof(T);
    }

    file_region_iterator() = default;

    template <typename CharT>
    explicit file_region_iterator(const CharT* name, uint64_t offset = 0, size_t least_region_sz = 1)
        : file_region_iterator_base(is_readonly, name, sizeof(T) * offset, sizeof(T) * least_region_sz)
    {
        init();
    }

    explicit file_region_iterator(fs::path const& fp, uint64_t offset = 0, size_t least_region_sz = 1)
        : file_region_iterator_base(is_readonly, fp.c_str(), sizeof(T) * offset, sizeof(T) * least_region_sz)
    {
        init();
    }

    file_region_iterator(file_region_iterator const&) = default;
    file_region_iterator(file_region_iterator &&) = default;

    file_region_iterator & operator=(file_region_iterator const& rhs)
    {
        if (BOOST_UNLIKELY(region_.get() != rhs.region_.get())) {
            region_ = rhs.region_;
        }
        return *this;
    }

    file_region_iterator & operator=(file_region_iterator && rhs)
    {
        if (BOOST_UNLIKELY(region_.get() != rhs.region_.get())) {
            region_ = std::move(rhs.region_);
        } else {
            rhs.region_.reset();
        }
        return *this;
    }

    proxy_t operator* () const
    {
        return iterators::make_value_proxy<value_type>(this);
    }

    file_region_iterator& operator++()
    {
        file_region_iterator_base::increment();
        init();
        return *this;
    }

    file_region_iterator& operator--()
    {
        file_region_iterator_base::decrement();
        init();
        return *this;
    }

    void advance_offset(std::ptrdiff_t d)
    {
        if (d >= 0 && e_ - b_ > d) {
            b_ += d;
            return;
        }
        
        size_t offs = file_region_iterator_base::advance_offset(d);
        init();
        b_ += offs;
    }

    // write to file from external buffer
    template <typename ST>
    void write(span<ST> data)
    {
        set(span((const char*)data.data(), data.size() * sizeof(ST)));
        array_view<char> raw = region_->get();
        BOOST_ASSERT(0 == raw.size() % sizeof(T));
        b_ = (T*)raw.begin();
        e_ = b_ + raw.size() / sizeof(T);
    }

	void flush()
	{
        if constexpr(!is_readonly) {
		    file_region_iterator_base::flush(static_cast<same_const_t<char, T>*>(b_));
        }
	}

    void close()
    {
        region_.reset();
        b_ = e_ = nullptr;
    }

private:
    mutable T* b_ = nullptr;
    mutable T* e_ = nullptr;
};

}
