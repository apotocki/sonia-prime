//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_HPP
#define SONIA_SERIALIZATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <boost/utility/in_place_factory.hpp>
#include "sonia/utility/serialization/serialization_fwd.hpp"

namespace sonia {

template <typename TagT, typename T, typename OutputIteratorT>
OutputIteratorT encode(T const& arg, OutputIteratorT oi)
{
    return serialization::coder<TagT, T>().encode(arg, std::move(oi));
}

template <typename TagT, typename T, typename InputIteratorT, typename ArgT>
InputIteratorT decode(InputIteratorT ii, ArgT && arg)
{
    return serialization::coder<TagT, T>().decode(std::move(ii), std::forward<ArgT>(arg));
}

template <typename TagT, typename InputIteratorT, typename T>
InputIteratorT decode(InputIteratorT ii, T & arg)
{
    return serialization::coder<TagT, T>().decode(std::move(ii), arg);
}

template <typename TagT, typename InputIteratorT, typename T>
InputIteratorT decode(InputIteratorT ii, T * arg)
{
    return serialization::coder<TagT, T>().decode(std::move(ii), arg);
}

template <typename TagT, typename InputIteratorT, typename T>
enable_if_t<is_rvalue_reference_v<T&&>, InputIteratorT> decode(InputIteratorT ii, T&& arg)
{
    return serialization::coder<TagT, T>().decode(std::move(ii), std::move(arg));
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
    explicit in_place_decoder_factory(IteratorT & it) : ii_(it) {}

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
    explicit decoder(InputIteratorT it) : ii(std::move(it)) {}

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
encoder<TagT, OutputIteratorT> make_encoder(OutputIteratorT oi)
{
    return encoder<TagT, OutputIteratorT>(std::move(oi));
}

template <typename TagT, class InputIteratorT>
decoder<TagT, InputIteratorT> make_decoder(InputIteratorT ii)
{
    return decoder<TagT, InputIteratorT>(std::move(ii));
}

}

#endif // SONIA_SERIALIZATION_HPP
