//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/singleton.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/functional/hash/pair.hpp"

#include "invokation.hpp"

#include "sonia/logger/logger.hpp"

namespace sonia::invokation {

class invokable
{
public:
    virtual ~invokable() = default;

    bool has_method(std::string_view methodname);
    blob_result invoke(std::string_view methodname, std::span<const blob_result> args);
    blob_result invoke(std::string_view name, std::initializer_list<const blob_result> args)
    {
        return invoke(name, std::span{args});
    }

    // properties routine
    blob_result get_property(std::string_view propname, bool pin = true) const;
    void set_property(std::string_view propname, blob_result&& val);
    virtual void on_propety_change(std::string_view) {}

    virtual blob_manager * get_blob_manager() const = 0;
};

struct method : polymorphic_movable, polymorphic_clonable
{
    virtual ~method() = default;
    virtual blob_result operator()(invokable &, std::span<const blob_result> args) const = 0;
    virtual blob_result operator()(invokable const&, std::span<const blob_result> args) const = 0;
};

struct fn_property : polymorphic_movable, polymorphic_clonable
{
    virtual ~fn_property() = default;
    virtual blob_result get(invokable const&) const = 0;
    virtual bool set(invokable&, blob_result) = 0; // returns true if updated
};

template <typename GetterT, typename SetterT>
struct concrete_fn_property: fn_property
{
    GetterT getter_;
    SetterT setter_;

    concrete_fn_property(GetterT && g, SetterT && s)
        : getter_{std::move(g)}, setter_{std::move(s)}
    {}

    blob_result get(invokable const& obj) const override { return getter_(obj); }
    bool set(invokable & obj, blob_result val) override { return setter_(obj, std::move(val)); }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(concrete_fn_property);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(concrete_fn_property);
};

template <typename GetterT>
struct concrete_fn_readonly_property: fn_property
{
    GetterT getter_;

    explicit concrete_fn_readonly_property(GetterT && g)
        : getter_{std::move(g)}
    {}

    blob_result get(invokable const& obj) const override { return getter_(obj); }
    bool set(invokable& obj, blob_result val) override { throw exception("an attempt to set readonly property"); }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(concrete_fn_readonly_property);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(concrete_fn_readonly_property);
};

template <auto FuncV>
struct concrete_method : method
{
    using sig_t = decltype(FuncV);
    using f_type = typename boost::function_types::function_type<sig_t>::type;
    using result_type = typename boost::function_types::result_type<f_type>::type;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;
    using pure_args_type = typename boost::mpl::pop_front<args_type>::type;
    using invokable_t = typename boost::mpl::at_c<args_type, 0>::type;

    template <typename InvokableT, size_t ... Is>
    blob_result do_job(std::index_sequence<Is...>, InvokableT& self, std::span<const blob_result> args) const
    {
        std::tuple<invokable_t&, typename boost::mpl::at_c<pure_args_type, Is>::type ...> argstpl{
            dynamic_cast<invokable_t&>(self), from_blob_at<typename boost::mpl::at_c<pure_args_type, Is>::type>(Is, args) ...};
        if constexpr (is_void_v<result_type>) {
            std::apply(FuncV, std::move(argstpl));
            return nil_blob_result();
        } else {
            //blob_result br = particular_blob_result(std::apply(FuncV, std::move(argstpl)), self.get_blob_manager());
            //GLOBAL_LOG_INFO() << "do_job returns: " << br;
            //return br;
            return particular_blob_result(std::apply(FuncV, std::move(argstpl)), self.get_blob_manager());
        }
    }

    blob_result operator()(invokable & self, std::span<const blob_result> args) const override
    {
        return do_job(std::make_index_sequence<boost::mpl::size<pure_args_type>::type::value>{}, self, args);
    }

    blob_result operator()(invokable const& self, std::span<const blob_result> args) const override
    {
        if constexpr (std::is_const_v<invokable_t>) {
            return do_job(std::make_index_sequence<boost::mpl::size<pure_args_type>::type::value>{}, self, args);
        } else {
            THROW_INTERNAL_ERROR("a mutable object is expected");
        }
    }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(concrete_method);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(concrete_method);
};


//#define CONCRETE_METHOD(func, ...) \
//sonia::in_place_type<::sonia::invokation::concrete_method<func>>

template <typename DerivedT>
class registrar
{
    static std::once_flag registration_flag_;

protected:
    using registrar_type = registrar;

public:
    void register_methods_and_properties()
    {
        std::call_once(registration_flag_, [this] {
            services::on_close([] {
                new(&registration_flag_) std::once_flag{};
            });
            static_cast<DerivedT*>(this)->do_registration(*this); 
        });
    }

    template <auto FuncV>
    void register_method(std::string_view name);

    template <typename GetterT>
    void register_readonly_property(std::string_view name, GetterT const&);

    template <typename GetterT, typename SetterT>
    void register_property(std::string_view name, GetterT const&, SetterT const&);

    void inherit(std::type_index from);
};

template <typename DerivedT>
std::once_flag registrar<DerivedT>::registration_flag_{};

class registry : public singleton
{
    using method_holder = automatic_polymorphic<method, 2 * sizeof(void*)>;
    using methods_map_t = boost::unordered_map<std::type_index, std::vector<std::pair<std::string, method_holder>>>;
    using concrete_methods_map_t = boost::unordered_map<std::pair<std::type_index, std::string>, method_holder, hasher>;

    using fn_property_holder = automatic_polymorphic<invokation::fn_property, 4 * sizeof(void*)>;
    using concrete_properties_map_t = boost::unordered_map<std::pair<std::type_index, std::string>, fn_property_holder, hasher>;
    using properties_map_t = boost::unordered_map<std::type_index, std::vector<std::pair<std::string, fn_property_holder>>>;

    methods_map_t methods_map_;
    concrete_methods_map_t concrete_methods_map_;
    properties_map_t properties_map_;
    concrete_properties_map_t concrete_properties_;
    mutable fibers::mutex mtx_, methods_map_mtx_, props_mtx_, props_map_mtx_;

public:
    registry()
    {
        
    }

    void register_method(std::type_index ti, std::string_view name, method_holder mh);
    
    template <std::derived_from<method> T>
    inline void register_method(std::type_index ti, std::string_view name)
    {
        register_method(ti, name, method_holder(sonia::in_place_type<T>));
    }

    void register_property(std::type_index ti, std::string_view name, fn_property_holder mh);

    void inherit(std::type_index from, std::type_index to);

    bool has_method(std::type_index ti, std::string_view methodname);
    blob_result invoke(std::type_index ti, invokable& obj, std::string_view name, std::span<const blob_result> args);

    blob_result get_property(std::type_index ti, invokable const& obj, std::string_view propname) const;
    bool set_property(std::type_index ti, invokable& obj, std::string_view propname, blob_result&& val);
};

template <typename DerivedT>
template <auto FuncV>
void registrar<DerivedT>::register_method(std::string_view name)
{
    as_singleton<invokation::registry>()
        ->register_method<concrete_method<FuncV>>(typeid(DerivedT), name);
}

template <typename DerivedT>
void registrar<DerivedT>::inherit(std::type_index from)
{
    as_singleton<invokation::registry>()->inherit(from, typeid(DerivedT));
}

template <typename DerivedT>
template <typename GetterT>
void registrar<DerivedT>::register_readonly_property(std::string_view name, GetterT const&)
{
    THROW_NOT_IMPLEMENTED_ERROR("register_readonly_property");
    //as_singleton<invokation::registry>()
    //    ->register_readonly_property<concrete_method<FuncV>>(typeid(DerivedT), name);
}

template <typename DerivedT>
template <typename GetterT, typename SetterT>
void registrar<DerivedT>::register_property(std::string_view name, GetterT const&, SetterT const&)
{
    // invokation::concrete_fn_property{
    THROW_NOT_IMPLEMENTED_ERROR("register_property");
}

/*
template <typename T>
class concrete_invokable : public virtual invokable
{
public:

    bool has_method(std::string_view methodname) override
    {
        throw;
    }

    using invokable::invoke;
    blob_result invoke(cstring_view methodname, std::span<const blob_result> args) override final
    {
        return as_singleton<invokation::registry>()->invoke(typeid(T), *this, methodname, args);
    }
};
*/

}
