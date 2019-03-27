//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_AUTOMATIC_POLYMORPHIC_HPP
#define SONIA_UTILITY_AUTOMATIC_POLYMORPHIC_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iostream>
#include <utility>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/utility/polymorphic_traits.hpp"

namespace sonia {

template <typename PolymorphicT, size_t SizeV, typename OffsetT>
class automatic_polymorphic_base
{
public:
    PolymorphicT * get_pointer() noexcept
    {
        return std::launder(reinterpret_cast<PolymorphicT*>(reinterpret_cast<char*>(&buffer_) + offset_));
    }

    PolymorphicT const* get_pointer() const noexcept
    {
        return std::launder(reinterpret_cast<PolymorphicT const*>(reinterpret_cast<char const*>(&buffer_) + offset_));
    }

protected:
    OffsetT get_offset() const { return offset_; }

    void set_offset(int offset)
    {
        BOOST_ASSERT (offset <= (std::numeric_limits<OffsetT>().max)());
        BOOST_ASSERT (offset >= (std::numeric_limits<OffsetT>().min)());
        offset_ = static_cast<OffsetT>(offset);
    }

    aligned_storage_t<SizeV, std::alignment_of_v<PolymorphicT>> buffer_;
    OffsetT offset_;
};


template <typename PolymorphicT, size_t SizeV>
class automatic_polymorphic_base<PolymorphicT, SizeV, void>
{
public:
    PolymorphicT * get_pointer() noexcept { return std::launder(reinterpret_cast<PolymorphicT*>(&buffer_)); }
    PolymorphicT const* get_pointer() const noexcept { return std::launder(reinterpret_cast<PolymorphicT const*>(&buffer_)); }

protected:
    constexpr int get_offset() const noexcept { return 0; }

    void set_offset(int offset)
    {
        BOOST_ASSERT (0 == offset);
    }

    aligned_storage_t<SizeV, std::alignment_of_v<PolymorphicT>> buffer_;
};

template <class PolymorphicT, size_t SizeV, typename OffsetT = void>
class automatic_polymorphic 
    : public automatic_polymorphic_base<PolymorphicT, SizeV, OffsetT>
{
    using base_t = automatic_polymorphic_base<PolymorphicT, SizeV, OffsetT>;

public:
    using value_type = PolymorphicT;

    automatic_polymorphic()
    {
        do_reset();
    }

    template <class T, class ... ArgsT>
    explicit automatic_polymorphic(in_place_type_t<T>, ArgsT&& ... args)
    {
        construct<T>(std::forward<ArgsT>(args) ...);
    }

    template <class T>
    automatic_polymorphic(T && arg, enable_if_t<
        (is_same_v<remove_cvref_t<T>, automatic_polymorphic> && is_polymorphic_clonable_v<PolymorphicT>)
        || (is_base_of_v<PolymorphicT, remove_cvref_t<T>> &&
                (
                    (is_reference_v<T&&> && is_polymorphic_clonable_v<remove_cvref_t<T>>) ||
                    (is_rvalue_reference_v<T&&> && is_polymorphic_movable_v<remove_cvref_t<T>>)
                )
            )
    > * enabler = nullptr)
    {
        if constexpr (is_same_v<remove_cvref_t<T>, automatic_polymorphic>) {
            clone(arg);
        } else if constexpr (is_rvalue_reference_v<T&&>) {
            move(std::move(arg));
        } else if constexpr (is_reference_v<T&&>) {
            clone(arg);
        } else {
            BOOST_THROW_EXCEPTION(internal_error("automatic_polymorphic"));
        }
    }

    automatic_polymorphic(automatic_polymorphic const& rhs)
    {
        static_assert(is_polymorphic_clonable_v<PolymorphicT>);
        clone(rhs);
    }

    automatic_polymorphic(automatic_polymorphic && rhs)
    {
        static_assert(is_polymorphic_movable_v<PolymorphicT>);
        move(std::move(rhs));
    }

    ~automatic_polymorphic() noexcept
    {
        if (*this) {
            std::destroy_at(get_pointer());
        }
    }

    automatic_polymorphic & operator= (automatic_polymorphic const& rhs)
    {
        static_assert(is_polymorphic_clonable_v<PolymorphicT>);
        if (this != &rhs) {
            if (*this) {
                std::destroy_at(get_pointer());
            }
            if (!rhs) {
                do_reset();
            } else try {
                clone(*rhs.get_pointer());
            } catch (...) {
                do_reset();
                throw;
            }
        }
        return *this;
    }

    automatic_polymorphic & operator= (automatic_polymorphic && rhs)
    {
        static_assert(is_polymorphic_movable_v<PolymorphicT>);
        if (this != &rhs) {
            if (*this) {
                std::destroy_at(get_pointer());
            }
            if (!rhs) {
                do_reset();
            } else try {
                move(*rhs.get_pointer());
                rhs.reset();
            } catch (...) {
                do_reset();
                throw;
            }
        }
        return *this;
    }

    template <typename T, class ... ArgsT>
    void emplace(ArgsT&& ... args)
    {
        if (!*this) {
            std::destroy_at(get_pointer());
        }
        try {
            construct<T>(std::forward<ArgsT>(args) ...);
        } catch (...) {
            do_reset();
            throw;
        }
    }

    using base_t::get_pointer;

    void reset() noexcept
    {
        if (!*this) {
            std::destroy_at(get_pointer());
        }
        do_reset();
    }

    PolymorphicT const& get() const { BOOST_ASSERT(*this); return *get_pointer(); }
    PolymorphicT & get() { BOOST_ASSERT(*this); return *get_pointer(); }

    PolymorphicT const& operator*() const { return get(); }
    PolymorphicT & operator*() { return get(); }

    PolymorphicT const* operator->() const { BOOST_ASSERT(*this); return get_pointer(); }
    PolymorphicT * operator->() { BOOST_ASSERT(*this); return get_pointer(); }

    explicit operator bool() const noexcept { return nullptr != *reinterpret_cast<void* const*>(&this->buffer_); }

private:
    void do_reset() { *reinterpret_cast<void**>(&this->buffer_) = nullptr; }

    template <class T, class ... ArgsT>
    void construct(ArgsT&& ... args)
    {
        static_assert(is_base_of_v<PolymorphicT, T>);
        BOOST_MPL_ASSERT_RELATION( sizeof(T), <=, SizeV );
        new (&this->buffer_) T(std::forward<ArgsT>(args) ...);
        T * place = std::launder(reinterpret_cast<T*>(&this->buffer_));
        int offset = static_cast<int>(reinterpret_cast<char*>(static_cast<PolymorphicT*>(place)) - reinterpret_cast<char*>(place));
        this->set_offset(offset);
    }

    void clone(automatic_polymorphic const& rhs)
    {
        if (rhs) {
            clone(*rhs.get_pointer());
        } else {
            do_reset();
        }
    }

    void clone(polymorphic_clonable const& sample)
    {
        PolymorphicT * p = static_cast<PolymorphicT*>(sample.clone(&this->buffer_, SizeV));
        int offset = static_cast<int>(reinterpret_cast<char*>(p) - std::launder(reinterpret_cast<char*>(&this->buffer_)));
        this->set_offset(offset);
    }

    void move(automatic_polymorphic && rhs)
    {
        if (rhs) {
            move(std::move(*rhs.get_pointer()));
        } else {
            do_reset();
        }
    }

    void move(polymorphic_movable && sample)
    {
        PolymorphicT * p = static_cast<PolymorphicT*>(sample.move(&this->buffer_, SizeV));
        int offset = static_cast<int>(reinterpret_cast<char*>(p) - std::launder(reinterpret_cast<char*>(&this->buffer_)));
        this->set_offset(offset);
    }
};

template <typename T, size_t SizeV, typename OffsetT> T * get_pointer(automatic_polymorphic<T, SizeV, OffsetT> & p) { return p.get_pointer(); }
template <typename T, size_t SizeV, typename OffsetT> T const* get_pointer(automatic_polymorphic<T, SizeV, OffsetT> const& p) { return p.get_pointer(); }

}

#endif // SONIA_UTILITY_AUTOMATIC_POLYMORPHIC_HPP
