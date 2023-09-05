//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/polymorphic_traits.hpp"

namespace sonia {

template <typename PolymorphicT, size_t SizeV, size_t AlignmentV, typename OffsetT>
class automatic_polymorphic_base
{
public:
    PolymorphicT * get_pointer() noexcept
    {
        return std::launder(reinterpret_cast<PolymorphicT*>(buffer_ + offset_));
    }

    PolymorphicT const* get_pointer() const noexcept
    {
        return std::launder(reinterpret_cast<PolymorphicT const*>(buffer_ + offset_));
    }

protected:
    OffsetT get_offset() const { return offset_; }

    void set_offset(int offset)
    {
        BOOST_ASSERT (offset <= (std::numeric_limits<OffsetT>().max)());
        BOOST_ASSERT (offset >= (std::numeric_limits<OffsetT>().min)());
        offset_ = static_cast<OffsetT>(offset);
    }

    alignas(AlignmentV) char buffer_[SizeV];
    OffsetT offset_;
};


template <typename PolymorphicT, size_t SizeV, size_t AlignmentV>
class automatic_polymorphic_base<PolymorphicT, SizeV, AlignmentV, void>
{
public:
    PolymorphicT * get_pointer() noexcept { return std::launder(reinterpret_cast<PolymorphicT*>(buffer_)); }
    PolymorphicT const* get_pointer() const noexcept { return std::launder(reinterpret_cast<PolymorphicT const*>(buffer_)); }

protected:
    constexpr int get_offset() const noexcept { return 0; }

    void set_offset(int offset)
    {
        BOOST_ASSERT (0 == offset);
    }

    alignas(AlignmentV) char buffer_[SizeV];
};

template <class PolymorphicT, size_t SizeV, size_t AlignmentV = std::alignment_of_v<PolymorphicT>, typename OffsetT = void>
class automatic_polymorphic 
    : public automatic_polymorphic_base<PolymorphicT, SizeV, AlignmentV, OffsetT>
{
    using base_t = automatic_polymorphic_base<PolymorphicT, SizeV, AlignmentV, OffsetT>;

public:
    using value_type = PolymorphicT;
    constexpr static bool clonable_v = is_polymorphic_clonable_v<PolymorphicT>;
    constexpr static bool movable_v = is_polymorphic_movable_v<PolymorphicT>;

    automatic_polymorphic()
    {
        do_reset();
    }

    template <std::derived_from<PolymorphicT> T, class ... ArgsT>
    explicit automatic_polymorphic(in_place_type_t<T>, ArgsT&& ... args)
    {
        construct<T>(std::forward<ArgsT>(args) ...);
    }

    template <typename T>
    requires(movable_v && is_same_v<T, automatic_polymorphic>)
    automatic_polymorphic(T && val)
    {
        move(std::move(val));
    }

    template <typename T>
    requires(clonable_v && is_same_v<T, automatic_polymorphic>)
    automatic_polymorphic(T const& val)
    {
        clone(val);
    }
    
    template <std::derived_from<PolymorphicT> T>
    requires(is_rvalue_reference_v<T&&> && is_polymorphic_movable_v<remove_cvref_t<T>>)
    automatic_polymorphic(T&& val)
    {
        move(std::move(val));
    }
    
    template <std::derived_from<PolymorphicT> T>
    requires(is_polymorphic_clonable_v<remove_cvref_t<T>>)
    automatic_polymorphic(T const& val)
    {
        clone(val);
    }
    /*
    {
        if constexpr (is_rvalue_reference_v<T&&>) {
            move(std::move(arg));
        } else if constexpr (is_reference_v<T&&>) {
            clone(arg);
        } else {
            THROW_INTERNAL_ERROR("automatic_polymorphic");
        }
    }

    
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
            THROW_INTERNAL_ERROR("automatic_polymorphic");
        }
    }
    */


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
                move(std::move(*rhs.get_pointer()));
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
        if (*this) {
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
        if (*this) {
            std::destroy_at(get_pointer());
        }
        do_reset();
    }

    operator PolymorphicT&() { BOOST_ASSERT(*this); return *get_pointer(); }
    operator const PolymorphicT&() const { BOOST_ASSERT(*this); return *get_pointer(); }

    PolymorphicT const& get() const { BOOST_ASSERT(*this); return *get_pointer(); }
    PolymorphicT & get() { BOOST_ASSERT(*this); return *get_pointer(); }

    PolymorphicT const& operator*() const { return get(); }
    PolymorphicT & operator*() { return get(); }

    PolymorphicT const* operator->() const { BOOST_ASSERT(*this); return get_pointer(); }
    PolymorphicT * operator->() { BOOST_ASSERT(*this); return get_pointer(); }

    explicit operator bool() const noexcept { return nullptr != *reinterpret_cast<void* const*>(this->buffer_); }

private:
    void do_reset() { *reinterpret_cast<void**>(this->buffer_) = nullptr; }

    template <std::derived_from<PolymorphicT> T, class ... ArgsT>
    void construct(ArgsT&& ... args)
    {
        BOOST_MPL_ASSERT_RELATION( sizeof(T), <=, SizeV );
        new (this->buffer_) T(std::forward<ArgsT>(args) ...);
        T * place = std::launder(reinterpret_cast<T*>(this->buffer_));
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
        do_clone(sample);
    }

    void clone(polymorphic_clonable_and_movable const& sample)
    {
        do_clone(sample);
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
        do_move(sample);
    }
    
    void move(polymorphic_clonable_and_movable && sample)
    {
        do_move(sample);
    }

    template <class PolymorphicClonableT>
    void do_clone(PolymorphicClonableT const& sample)
    {
        PolymorphicT * p = static_cast<PolymorphicT*>(sample.clone(this->buffer_, SizeV));
        int offset = static_cast<int>(reinterpret_cast<char*>(p) - std::launder(reinterpret_cast<char*>(this->buffer_)));
        this->set_offset(offset);
    }

    template <class PolymorphicMovableT>
    void do_move(PolymorphicMovableT & sample)
    {
        PolymorphicT * p = static_cast<PolymorphicT*>(sample.move(this->buffer_, SizeV));
        int offset = static_cast<int>(reinterpret_cast<char*>(p) - std::launder(reinterpret_cast<char*>(this->buffer_)));
        this->set_offset(offset);
    }
};

template <typename T, size_t SizeV, size_t AlignmentV, typename OffsetT> T * get_pointer(automatic_polymorphic<T, SizeV, AlignmentV, OffsetT> & p) { return p.get_pointer(); }
template <typename T, size_t SizeV, size_t AlignmentV, typename OffsetT> T const* get_pointer(automatic_polymorphic<T, SizeV, AlignmentV, OffsetT> const& p) { return p.get_pointer(); }

template <class PolymorphicT, size_t SizeV, size_t AlignmentV, typename OffsetT>
inline size_t hash_value(automatic_polymorphic<PolymorphicT, SizeV, AlignmentV, OffsetT> const& v)
{
    return hash_value(*v);
}

template <class LPolymorphicT, size_t LSizeV, size_t LAlignmentV, typename LOffsetT, class RPolymorphicT, size_t RSizeV, size_t RAlignmentV, typename ROffsetT>
inline bool operator== (automatic_polymorphic<LPolymorphicT, LSizeV, LAlignmentV, LOffsetT> const& l, automatic_polymorphic<RPolymorphicT, RSizeV, RAlignmentV, ROffsetT> const& r)
{
    return *l == *r;
}

}
