//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIONAL_INHERITOR_HPP
#define SONIA_UTILITY_OPTIONAL_INHERITOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <boost/assert.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/explicit_operator_bool.hpp"
#include "sonia/utility/polymorphic_traits.hpp"

namespace sonia {

template <typename BaseT, size_t SizeV, typename OffsetT>
class optional_inheritor_holder
{
public:
    BaseT * get_pointer() noexcept { return reinterpret_cast<BaseT*>(reinterpret_cast<char*>(&buffer_) + offset_); }
    BaseT const* get_pointer() const noexcept { return reinterpret_cast<BaseT const*>(reinterpret_cast<char const*>(&buffer_) + offset_); }

protected:
    OffsetT get_offset() const { return offset_; }

    void set_offset(int offset) {
        BOOST_ASSERT (offset <= (std::numeric_limits<OffsetT>().max)());
        BOOST_ASSERT (offset >= (std::numeric_limits<OffsetT>().min)());
        offset_ = static_cast<OffsetT>(offset);
    }

    aligned_storage_t<SizeV, std::alignment_of_v<BaseT>> buffer_;
    OffsetT offset_;
};


template <typename BaseT, size_t SizeV>
class optional_inheritor_holder<BaseT, SizeV, void>
{
public:
    BaseT * get_pointer() noexcept { return reinterpret_cast<BaseT*>(&buffer_); }
    BaseT const* get_pointer() const noexcept { return reinterpret_cast<BaseT const*>(&buffer_); }

protected:
    int get_offset() const noexcept { return 0; }

    void set_offset(int offset) {
        BOOST_ASSERT (0 == offset);
    }

    aligned_storage_t<SizeV, std::alignment_of_v<BaseT>> buffer_;
};

template <class BaseT, size_t SizeV, typename OffsetT = void>
class optional_inheritor 
    : public optional_inheritor_holder<BaseT, SizeV, OffsetT>
{
    typedef optional_inheritor_holder<BaseT, SizeV, OffsetT> holder_t;

public:
    optional_inheritor() {
        do_set_empty();
    }

    template <typename T, class ... ArgsT>
    explicit optional_inheritor(identity<T>, ArgsT&& ... args) {
        construct<T>(std::forward<ArgsT>(args) ...);
    }

    template <typename T>
    explicit optional_inheritor(T const& val) {
        static_assert(is_base_of_v<BaseT, T> || is_same_v<BaseT, T>);
        // needs polymorphic construct to avoid possible type cut
        static_assert(is_polymorphic_clonable_v<BaseT>);
        clone();
    }

    optional_inheritor(optional_inheritor const& rhs) {
        static_assert(is_polymorphic_clonable_v<BaseT>);
        if (!rhs.empty()) {
            clone(*rhs.get_pointer());
        } else {
            do_set_empty();
        }
    }

    optional_inheritor(optional_inheritor && rhs) {
        static_assert(is_polymorphic_movable_v<BaseT>);
        if (!rhs.empty()) {
            move(*rhs.get_pointer());
        } else {
            do_set_empty();
        }
    }

    ~optional_inheritor() noexcept {
        if (!empty()) {
            get_pointer()->~BaseT();
        }
    }

    optional_inheritor & operator= (optional_inheritor const& rhs) {
        static_assert(is_polymorphic_clonable_v<BaseT>);
        if (this != &rhs) {
            if (!empty()) {
                get_pointer()->~BaseT();
            }
            if (rhs.empty()) {
                do_set_empty();
            } else try {
                clone(*rhs.get_pointer());
            } catch (...) {
                do_set_empty();
                throw;
            }
        }
        return *this;
    }

    optional_inheritor & operator= (optional_inheritor && rhs) {
        static_assert(is_polymorphic_movable_v<BaseT>);
        if (this != &rhs) {
            if (!empty()) {
                get_pointer()->~BaseT();
            }
            if (rhs.empty()) {
                do_set_empty();
            } else try {
                move(*rhs.get_pointer());
                rhs.reset();
            } catch (...) {
                do_set_empty();
                throw;
            }
        }
        return *this;
    }

    template <typename T, class ... ArgsT>
    void emplace(ArgsT&& ... args) {
        if (!empty()) {
            get_pointer()->~BaseT();
        }
        try {
            construct<T>(std::forward<ArgsT>(args) ...);
        } catch (...) {
            do_set_empty();
            throw;
        }
    }

    using holder_t::get_pointer;

    void reset() noexcept {
        if (empty()) {
            get_pointer()->~BaseT();
        }
        do_set_empty();
    }

    BaseT const& get() const { BOOST_ASSERT(!empty()); return *get_pointer(); }
    BaseT & get() { BOOST_ASSERT(!empty());return *get_pointer(); }

    BaseT const& operator*() const { return get(); }
    BaseT & operator*() { return get(); }

    BaseT const* operator->() const { BOOST_ASSERT(!empty()); return get_pointer(); }
    BaseT * operator->() { BOOST_ASSERT(!empty()); return get_pointer(); }

    bool operator!() const noexcept { return empty(); }

    BOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL();

    bool empty() const noexcept {
        return 0 == *reinterpret_cast<void* const*>(&this->buffer_);
    }

private:
    void do_set_empty() {
        *reinterpret_cast<void**>(&this->buffer_) = 0;
    }

    template <class T, class ... ArgsT>
    void construct(ArgsT&& ... args) {
        static_assert(is_base_of_v<BaseT, T> || is_same_v<BaseT, T>);
        static_assert(sizeof(T) <= SizeV);
        T * place = reinterpret_cast<T*>(&this->buffer_);
        new (place) T(std::forward<ArgsT>(args) ...);
        int offset = static_cast<int>(reinterpret_cast<char*>(static_cast<BaseT*>(place)) - reinterpret_cast<char*>(place));
        this->set_offset(offset);
    }

    void clone(polymorphic_clonable const& sample) {
        BaseT * p = static_cast<BaseT*>(sample.clone(&this->buffer_, SizeV));
        int offset = static_cast<int>(reinterpret_cast<char*>(p) - reinterpret_cast<char*>(&this->buffer_));
        this->set_offset(offset);
    }

    void move(polymorphic_movable & sample) {
        BaseT * p = static_cast<BaseT*>(sample.move(&this->buffer_, SizeV));
        int offset = static_cast<int>(reinterpret_cast<char*>(p) - reinterpret_cast<char*>(&this->buffer_));
        this->set_offset(offset);
    }
};

}

#endif // SONIA_UTILITY_OPTIONAL_INHERITOR_HPP
