//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PARAMETERS_VALUE_DESCRIPTOR_HPP
#define SONIA_PARAMETERS_VALUE_DESCRIPTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <boost/any.hpp>
#include "sonia/utility/json/value.hpp"

namespace sonia { namespace parameters {

class value_descriptor {
public:
    virtual ~value_descriptor() {}

    value_descriptor() : required_(false) {}

    void set_name(std::string val) { name_ = std::move(val); }
    void set_description(std::string val) { descr_ = std::move(val); }

    std::string const& name() const { return name_; }
    std::string const& description() const { return descr_; }
    boost::any const& get_default() const { return default_; }
    bool is_required() const { return required_; }

    virtual void assign(json_value const&, boost::any &) {};

protected:
    std::string name_, descr_;
    boost::any default_;
    bool required_;
};

template <typename T>
class typed_value_descriptor : public value_descriptor {
public:
    typed_value_descriptor * required() noexcept { required_ = true; return this; }

    void assign(json_value const& jv, boost::any & res) override {
        // res = json_cast<T>(jv);
    }

    template <typename ... ArgsT>
    typed_value_descriptor * default_value(ArgsT&& ... args) {
        try {
            default_ = T(std::forward<ArgsT>(args) ...);
            return this;
        } catch (...) {
            delete this;
            throw;
        }
    }
};

template <typename T>
class typed_arrayvalue_descriptor : public value_descriptor {
public:
    typed_arrayvalue_descriptor * required() noexcept { required_ = true; return this; }
};

template <typename T>
value_descriptor * value() {
    return new typed_value_descriptor<T>();
}

template <typename T>
value_descriptor * array_value() {
    return new typed_arrayvalue_descriptor<T>();
}

}}

#endif // SONIA_PARAMETERS_VALUE_DESCRIPTOR_HPP
