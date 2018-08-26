//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "local_service_registry.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"
#include "sonia/serialization/boost_serialization_tuple.hpp"

namespace sonia {

local_service_registry::local_service_registry(shared_ptr<persister> sp)
    : counter_(0), state_persister_(std::move(sp))
{
    restore();
}

service::id local_service_registry::get_id(string_view name)
{
    std::lock_guard<std::mutex> guard(mtx_);

    auto it = registry_.find(name, string_hasher(), string_equal_to());
    if (it == registry_.end()) {
        registry_.insert(it, std::make_tuple(to_string(name), ++counter_));
        backup();
        return counter_;
    }
    return std::get<1>(*it);
}

string_view local_service_registry::get_name(service::id id) const 
{
    std::lock_guard<std::mutex> guard(mtx_);
    auto it = registry_.get<1>().find(id);
    if (it == registry_.get<1>().end()) {
        BOOST_THROW_EXCEPTION(internal_error("no service registered for id: " + boost::lexical_cast<std::string>(id)));
    }
    return std::get<0>(*it);
}

void local_service_registry::backup() const
{
    state_persister_->write_stream([this](std::ostream & os) {
        boost::archive::xml_oarchive oa(os);
        const_cast<local_service_registry&>(*this).serialize(oa);
    });
}

void local_service_registry::restore()
{
    state_persister_->read_stream([this](std::istream & is) {
        boost::archive::xml_iarchive ia(is);
        this->serialize(ia);
    });
}

}
