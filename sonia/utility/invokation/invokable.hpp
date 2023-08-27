//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/services/on_close.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/functional/hash/pair.hpp"

#include "sonia/utility/multimethod.hpp"

#include "invokation.hpp"

#include "sonia/logger/logger.hpp"

namespace sonia::invokation {

class invokable
{
public:
    virtual ~invokable() = default;

    bool has_method(string_view methodname);
    smart_blob invoke(string_view methodname, span<const blob_result> args);
    smart_blob invoke(string_view name, std::initializer_list<const blob_result> args)
    {
        return invoke(name, span{args});
    }

    // properties routine
    virtual smart_blob get_property(string_view propname) const;
    virtual void set_property(string_view propname, blob_result&& val);
    virtual void on_propety_change(string_view) {}
};

struct method : multimethod
{
    virtual smart_blob operator()(invokable &, span<const blob_result> args) const = 0;
    virtual smart_blob operator()(invokable const&, span<const blob_result> args) const = 0;
};

struct fn_property : multimethod
{
    virtual ~fn_property() = default;
    virtual smart_blob get(invokable const&) const = 0;
    virtual bool set(invokable&, smart_blob) const = 0; // returns true if updated
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
    smart_blob do_job(std::index_sequence<Is...>, InvokableT& self, span<const blob_result> args) const
    {
        std::tuple<invokable_t&, typename boost::mpl::at_c<pure_args_type, Is>::type ...> argstpl{
            dynamic_cast<invokable_t&>(self), from_blob_at<typename boost::mpl::at_c<pure_args_type, Is>::type>(Is, args) ...};
        if constexpr (is_void_v<result_type>) {
            std::apply(FuncV, std::move(argstpl));
            return smart_blob{};
        } else {
            //blob_result br = particular_blob_result(std::apply(FuncV, std::move(argstpl)), self.get_blob_manager());
            //GLOBAL_LOG_INFO() << "do_job returns: " << br;
            //return br;
            return smart_blob { particular_blob_result(std::apply(FuncV, std::move(argstpl))) };
        }
    }

    smart_blob operator()(invokable & self, span<const blob_result> args) const override
    {
        return do_job(std::make_index_sequence<boost::mpl::size<pure_args_type>::type::value>{}, self, args);
    }

    smart_blob operator()(invokable const& self, span<const blob_result> args) const override
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

template <typename InvokableT, typename GetterT, typename SetterT>
struct concrete_fn_property : fn_property
{
    GetterT getter_;
    SetterT setter_;

    concrete_fn_property(GetterT&& g, SetterT&& s)
        : getter_{ std::move(g) }, setter_{ std::move(s) }
    {}

    smart_blob get(invokable const& obj) const override { return getter_(static_cast<InvokableT const&>(obj)); }
    bool set(invokable& obj, smart_blob val) const override { return setter_(static_cast<InvokableT &>(obj), std::move(val)); }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(concrete_fn_property);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(concrete_fn_property);
};

template <typename InvokableT, typename GetterT>
struct concrete_fn_readonly_property : fn_property
{
    GetterT getter_;

    explicit concrete_fn_readonly_property(GetterT&& g)
        : getter_{ std::move(g) }
    {}

    smart_blob get(invokable const& obj) const override { return getter_(static_cast<InvokableT const&>(obj)); }
    bool set(invokable& obj, smart_blob val) const override { throw exception("an attempt to set readonly property"); }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(concrete_fn_readonly_property);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(concrete_fn_readonly_property);
};

template <auto FieldV>
struct field_fn_property : fn_property
{
    using sig_t = decltype(FieldV);
    using f_type = typename boost::function_types::function_type<sig_t>::type;
    using property_type = remove_reference_t<typename boost::function_types::result_type<f_type>::type>;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;
    using invokable_t = remove_reference_t<typename boost::mpl::at_c<args_type, 0>::type>;

    //field_fn_property()
    //{
    //    GLOBAL_LOG_INFO() << "INV TYPE: " << typeid(invokable_t).name() << ", rtype: " << typeid(property_type).name();
    //}

    smart_blob get(invokable const& obj) const override
    {
        return smart_blob { particular_blob_result(dynamic_cast<invokable_t const&>(obj).*FieldV) };
    }

    bool set(invokable& obj, smart_blob val) const override
    {
        property_type & stored_val = dynamic_cast<invokable_t&>(obj).*FieldV;
        auto newval = from_blob<property_type>{}(*val);
        if (stored_val != newval) {
            stored_val = newval;
            return true;
        }
        return false;
    }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(field_fn_property);
    SONIA_POLYMORPHIC_CLONABLE_IMPL(field_fn_property);
};

template <typename DerivedT, typename InheritedT = void>
class registrar
{
    static std::once_flag registration_flag_;

protected:
    using registrar_type = registrar;

public:
    registrar()
    {
        std::call_once(registration_flag_, [this] {
            services::on_close([] {
                new(&registration_flag_) std::once_flag{};
            });
            static_cast<DerivedT*>(this)->do_registration(*this); 
            if constexpr (!is_void_v<InheritedT>) {
                inherit(typeid(InheritedT));
            }
        });
    }

    template <auto FuncV>
    void register_method(string_view name)
    {
        sonia::services::register_multimethod(concrete_method<FuncV>(), { typeid(DerivedT), name });
    }

    template <typename GetterT>
    void register_readonly_property(string_view name, GetterT && g)
    {
        sonia::services::register_multimethod(concrete_fn_readonly_property<DerivedT, remove_cvref_t<GetterT>>(std::forward<GetterT>(g)), { typeid(DerivedT), name });
    }

    template <typename GetterT, typename SetterT>
    void register_property(string_view name, GetterT && g, SetterT && s)
    {
        sonia::services::register_multimethod(concrete_fn_property<DerivedT, remove_cvref_t<GetterT>, remove_cvref_t<SetterT>>(std::forward<GetterT>(g), std::forward<SetterT>(s)), { typeid(DerivedT), name });
    }

    template <auto FieldV>
    void register_property(string_view name)
    {
        sonia::services::register_multimethod(field_fn_property<FieldV>(), { typeid(DerivedT), name });
    }

private:
    void inherit(std::type_index from)
    {
        sonia::services::copy_multimethods({ from }, { typeid(DerivedT) });
    }
};

template <typename DerivedT, typename InheritedT>
std::once_flag registrar<DerivedT, InheritedT>::registration_flag_{};

}
