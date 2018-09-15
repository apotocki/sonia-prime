//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_LOGGER_LOGGABLE_HPP
#define SONIA_LOGGER_LOGGABLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/log/attributes.hpp>

#include "sonia/shared_ptr.hpp"
#include "logger.hpp"

namespace sonia {

class loggable {
public:
    loggable() : logger_(make_shared<logger::logger_type>()) {}
    loggable(logger::logger_ptr shl) : logger_(std::move(shl)) {}

    virtual ~loggable() {}

    logger::logger_ptr logger() { return logger_; }

    template <typename AttrT>
    void set_attribute(std::string const& name, AttrT const& value) const {
        logger_->add_attribute(name, boost::log::attributes::constant<AttrT>(value));
    }

private:
    shared_ptr<logger::logger_type> logger_;
};

}
#endif // SONIA_LOGGER_LOGGABLE_HPP
