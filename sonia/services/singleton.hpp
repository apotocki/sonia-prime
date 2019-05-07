//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_SINGLETON_HPP
#define SONIA_SERVICES_SINGLETON_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/cstdint.hpp"

namespace sonia {
    
struct singleton_access;

class singleton
{
    friend struct singleton_access;

public:
    using id = uint32_t;

    explicit singleton(id idval = 0) : id_(idval) {}
    
    singleton(singleton const&) = delete;
    singleton& operator=(singleton const&) = delete;

    virtual ~singleton() = default;

    id get_id() const noexcept { return id_; }
    int get_layer() const noexcept { return layer_; }
    std::string const& get_name() const noexcept { return name_; }

    virtual void close() noexcept {}

private:
    id id_;
    int layer_{0};
    std::string name_;
};

struct singleton_access
{
    static void set_id(singleton & s, singleton::id idval) { s.id_ = idval; }
    static void set_layer(singleton & s, int lval) { s.layer_ = lval; }
    static void set_name(singleton & s, std::string sval) { s.name_ = std::move(sval); }
};


}

#endif // SONIA_SERVICES_SINGLETON_HPP
