// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ITERATOR_TAGS_HPP
#define AGNOSTIC_STD_ITERATOR_TAGS_HPP

#pragma once

namespace std {

struct input_iterator_tag {};

struct output_iterator_tag {};

struct forward_iterator_tag : input_iterator_tag {};

struct bidirectional_iterator_tag : forward_iterator_tag {};

struct random_access_iterator_tag : bidirectional_iterator_tag {};

}

#endif // AGNOSTIC_STD_ITERATOR_TAGS_HPP
