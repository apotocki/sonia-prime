//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_COMMAND_HPP
#define SONIA_COMMAND_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <typeinfo>

namespace sonia {

class command_base {
public:
    virtual ~command_base() {}
    virtual std::type_info const& get_type_info() const = 0;
};

template <typename R, typename ... ArgsT>
class command : public command_base {
public:
    virtual R operator()(ArgsT ...) const = 0;
};

}

#endif // SONIA_COMMAND_HPP
