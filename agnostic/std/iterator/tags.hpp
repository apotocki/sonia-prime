// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_TAGS

namespace std {

struct input_iterator_tag {};

struct output_iterator_tag {};

struct forward_iterator_tag : input_iterator_tag {};

struct bidirectional_iterator_tag : forward_iterator_tag {};

struct random_access_iterator_tag : bidirectional_iterator_tag {};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
