//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cstdint>
#include <type_traits>

#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/serialization/serialization_fwd.hpp"

namespace sonia {

namespace serialization {

// A lower bound of the encoded size of a T, used to check element counts read from an untrusted
// input before allocating (see check_decoded_count); 0 means unknown, no check. A coder that can
// encode a non-empty T in zero bytes must specialize it to 0.
template <typename TagT, typename T>
struct min_encoded_size : integral_constant<size_t, std::is_empty_v<T> ? 0 : 1> {};

template <typename TagT, typename T>
inline constexpr size_t min_encoded_size_v = min_encoded_size<TagT, remove_cv_t<T>>::value;

}

// For decoders, before allocating `count` elements of at least `min_element_size` encoded bytes
// each, as read from the input: if the input iterator knows how much is left (remaining(), e.g.
// empty_check_iterator over safe_iterator), a count that can't fit in it fails as eof instead of
// requesting a huge allocation. Other iterators aren't checked.
template <typename InputIteratorT>
void check_decoded_count(InputIteratorT const& ii, std::uint64_t count, size_t min_element_size)
{
    if constexpr (requires { ii.remaining(); }) {
        if (min_element_size && count > ii.remaining() / min_element_size) {
            throw eof_exception();
        }
    }
}

template <typename TagT, typename T, typename InputIteratorT>
void check_decoded_count(InputIteratorT const& ii, std::uint64_t count)
{
    check_decoded_count(ii, count, serialization::min_encoded_size_v<TagT, T>);
}

template <typename TagT, typename T, typename OutputIteratorT>
OutputIteratorT encode(T const& arg, OutputIteratorT oi)
{
    return serialization::coder<TagT, T>{}.encode(arg, std::move(oi));
}

template <typename TagT, typename T, typename InputIteratorT, typename ArgT>
InputIteratorT decode(InputIteratorT ii, ArgT && arg)
{
    return serialization::coder<TagT, T>{}.decode(std::move(ii), std::forward<ArgT>(arg));
}

template <typename TagT, typename InputIteratorT, typename T>
InputIteratorT decode(InputIteratorT ii, T & arg)
{
    return serialization::coder<TagT, T>{}.decode(std::move(ii), arg);
}

template <typename TagT, typename InputIteratorT, typename T>
InputIteratorT decode(InputIteratorT ii, T * arg)
{
    return serialization::coder<TagT, T>{}.decode(std::move(ii), arg);
}

template <typename TagT, typename InputIteratorT, typename T>
requires(is_rvalue_reference_v<T&&>)
InputIteratorT decode(InputIteratorT ii, T&& arg)
{
    return serialization::coder<TagT, T>{}.decode(std::move(ii), std::move(arg));
}

template <typename TagT, typename InputIteratorT, typename T>
InputIteratorT default_decode_ptr(InputIteratorT ii, T * arg)
{
    new(arg) T;
    try {
        return serialization::coder<TagT, T>().decode(std::move(ii), *arg);
    } catch (...) {
        std::destroy_at(arg);
        throw;
    }
}

template <typename TagT, typename IteratorT>
struct in_place_decoder_factory
    : boost::in_place_factory_base
{
    explicit in_place_decoder_factory(IteratorT it) : ii_(std::move(it)) {}

    template <class T>
    void apply(void* address) const
    {
        ii_ = serialization::coder<TagT, T>().decode(std::move(ii_), reinterpret_cast<T*>(address));
    }

    mutable IteratorT ii_;
};

template <typename TagT, typename IteratorT>
struct in_place_decoder_factory<TagT, IteratorT&>
    : boost::in_place_factory_base
{
    explicit in_place_decoder_factory(IteratorT & it) : ii_{it} {}

    template <class T>
    void apply(void* address) const
    {
        ii_ = serialization::coder<TagT, T>().decode(std::move(ii_), reinterpret_cast<T*>(address));
    }

    IteratorT & ii_;
};

template <typename TagT, typename InputIteratorT>
in_place_decoder_factory<TagT, InputIteratorT&> in_place_decode(InputIteratorT & ii)
{
    return in_place_decoder_factory<TagT, InputIteratorT&>(ii);
}

template <typename TagT, typename InputIteratorT>
in_place_decoder_factory<TagT, InputIteratorT> in_place_decode(InputIteratorT && ii)
{
    return in_place_decoder_factory<TagT, InputIteratorT>(std::move(ii));
}

template <typename TagT, class OutputIteratorT>
class encoder
{
public:
    explicit encoder(OutputIteratorT it) : oi(std::move(it)) {}

    template <typename T>
    encoder & operator& (T && arg)
    {
        oi = encode<TagT>(std::forward<T>(arg), std::move(oi));
        return *this;
    }

    operator OutputIteratorT && () { return std::move(oi); }

    OutputIteratorT & iterator() { return oi; }

private:
    OutputIteratorT oi;
};

template <typename TagT, class InputIteratorT>
class decoder
{
public:
    explicit decoder(InputIteratorT it) : ii{std::move(it)} {}

    template <typename T>
    decoder & operator& (T * arg)
    {
        ii = decode<TagT, T>(std::move(ii), arg);
        return *this;
    }

    template <typename T>
    decoder & operator& (T & arg)
    {
        ii = decode<TagT, T>(std::move(ii), arg);
        return *this;
    }

    template <typename T>
    decoder & operator& (T const& arg)
    {
        ii = decode<TagT, T>(std::move(ii), arg);
        return *this;
    }

    operator InputIteratorT && () { return std::move(ii); }

    InputIteratorT & iterator() { return ii; }

private:
    InputIteratorT ii;
};

template <typename TagT, class OutputIteratorT>
encoder<TagT, remove_cvref_t<OutputIteratorT>> make_encoder(OutputIteratorT && oi)
{
    return encoder<TagT, remove_cvref_t<OutputIteratorT>>(std::forward<OutputIteratorT>(oi));
}

template <typename TagT, class InputIteratorT>
decoder<TagT, InputIteratorT> make_decoder(InputIteratorT ii)
{
    return decoder<TagT, InputIteratorT>(std::move(ii));
}

}
