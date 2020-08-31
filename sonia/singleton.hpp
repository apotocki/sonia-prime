//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SINGLETON_HPP
#define SONIA_SINGLETON_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <mutex>

#include "sonia/prime_config.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/services/singleton.hpp"
#include "sonia/services/on_close.hpp"
#include "sonia/utility/in_place_factory.hpp"

namespace sonia { namespace services {
    
SONIA_PRIME_API singleton & locate_singleton(std::type_index const& ti, function<shared_ptr<singleton>(singleton::id)> const&);

}

template <typename T>
struct singleton_wrapper
{
    template <typename ... ArgsT>
    explicit singleton_wrapper(ArgsT&& ... args)
    {
        std::call_once(once_flag_, [f = in_place_factory(std::forward<ArgsT>(args) ...)]() {
            instance_ = static_cast<T*>(&services::locate_singleton(typeid(T), [&f](singleton::id sid) -> shared_ptr<singleton> {
                shared_ptr<singleton> s = construct_shared<T>(f);
                singleton_access::set_name(*s, typeid(T).name());
                singleton_access::set_id(*s, sid);
                return s;
            }));
        });
        services::on_close([]{instance_ = nullptr; std::destroy_at(&once_flag_), new(&once_flag_) std::once_flag{}; });
    }
    singleton_wrapper(singleton_wrapper const&) = delete;
    singleton_wrapper& operator= (singleton_wrapper const&) = delete;

    T * operator->() const { return instance_; }

    T & get() const { return *instance_; }

private:
    static T * instance_;
    static std::once_flag once_flag_;
};

template <typename T, typename ... ArgsT>
singleton_wrapper<T> as_singleton(ArgsT&& ... args)
{
    return singleton_wrapper<T>(std::forward<ArgsT>(args) ...);
}

template <typename T>
T* singleton_wrapper<T>::instance_{nullptr};

template <typename T>
std::once_flag singleton_wrapper<T>::once_flag_{};

}

#endif // SONIA_SINGLETON_HPP
