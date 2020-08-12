// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#   include "agnostic/std/type_traits/alignment_of.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DESTRUCTIBLE
#   include "agnostic/std/type_traits/is_trivially_destructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_FORWARD
#   include "agnostic/std/utility/forward.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_COPY
#   include "agnostic/std/memory/uninitialized_copy.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_DEFAULT_CONSTRUCT
#   include "agnostic/std/memory/uninitialized_default_construct.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_BEGIN
#   include "agnostic/std/iterator/begin.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_END
#   include "agnostic/std/iterator/end.hpp"
#endif

#include "agnostic/utility/ceiling.hpp"

namespace agnostic {

template <class ABT>
constexpr size_t adjacent_buffer_offset_v = ceiling_v<sizeof(ABT), ABT::type_alignment>;

template <typename T, typename BaseT>
class adjacent_buffer : public BaseT
{
public:
    using value_type = T;
    using size_type = size_t;
    static constexpr size_t type_alignment = std::alignment_of_v<T>;

    template <typename RngT, typename ... ArgsT>
    adjacent_buffer(size_type sz, RngT&& rng, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        std::uninitialized_copy(std::begin(rng), std::end(rng), begin());
    }

    template <typename ... ArgsT>
    adjacent_buffer(size_type sz, in_place_t, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        std::uninitialized_default_construct(begin(), end());
    }

    adjacent_buffer(adjacent_buffer const& rhs) = delete;
    adjacent_buffer(adjacent_buffer&& rhs) = delete;
    adjacent_buffer& operator= (adjacent_buffer const&) = delete;
    adjacent_buffer& operator= (adjacent_buffer&&) = delete;

    ~adjacent_buffer()
    {
        if constexpr (!is_trivially_destructible_v<T>) {
            std::for_each(begin(), end(), [](T& obj) { obj.~T(); });
        }
    }

    T* begin() noexcept { return std::launder(reinterpret_cast<T*>(((char*)this) + adjacent_buffer_offset_v<adjacent_buffer>)); }
    T const* begin() const noexcept { return std::launder(reinterpret_cast<T const*>(((char const*)this) + adjacent_buffer_offset_v<adjacent_buffer>)); }
    T const* cbegin() const noexcept { return begin(); }

    T* end() noexcept { return begin() + sz_; }
    T const* end() const noexcept { return begin() + sz_; }
    T const* cend() const noexcept { return end(); }

    T& front() noexcept { return *begin(); }
    T const& front() const noexcept { return *begin(); }

    T& back() noexcept { return *(end() - 1); }
    T const& back() const noexcept { return *(end() - 1); }

    T& operator[](size_t ind) noexcept { return begin()[ind]; }
    T const& operator[](size_t ind) const noexcept { return begin()[ind]; }


    inline size_type size() const noexcept { return sz_; }
    inline bool empty() const noexcept { return !sz_; }
    inline explicit operator bool() const noexcept { return !empty(); }

private:
    size_type sz_;
};

}
