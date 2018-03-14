//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_HPP
#define SONIA_SERIALIZATION_HPP

#include <utility>

namespace sonia {

template <typename TagT, typename T, typename EnableT = void> class coder;

namespace serialization {

struct default_t {};
struct compressed_t {};
struct ordered_t {};

}

template <typename TagT, typename T, typename ArgT, typename OutputIteratorT>
OutputIteratorT encode(ArgT && arg, OutputIteratorT oi) {
    return coder<TagT, T>().encode(std::forward<ArgT>(arg), std::move(oi));
}

template <typename TagT, typename T, typename InputIteratorT, typename ArgT>
InputIteratorT decode(InputIteratorT ii, ArgT && arg) {
    return coder<TagT, T>().decode(std::move(ii), std::forward<ArgT>(arg));
}

template <typename TagT, typename IteratorT>
struct in_place_decoder_factory
{
    explicit in_place_decoder_factory(IteratorT it) : ii_(it) {}

    template <class T>
    void apply(void* address) const {
        ii_ = coder<TagT, T>().decode(std::move(ii_), reinterpret_cast<T*>(address));
    }

    mutable IteratorT ii_;
};

template <typename TagT, typename InputIteratorT>
in_place_decoder_factory<TagT, InputIteratorT&> in_place_decode(InputIteratorT & ii) {
    return in_place_decoder_factory<TagT, InputIteratorT&>(ii);
}

template <typename TagT, typename InputIteratorT>
in_place_decoder_factory<TagT, InputIteratorT> in_place_decode(InputIteratorT ii) {
    return in_place_decoder_factory<TagT, InputIteratorT>(ii);
}

}

#endif // SONIA_SERIALIZATION_HPP
