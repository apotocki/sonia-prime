//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/singleton.hpp"
#include "sonia/services/on_close.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/functional/hash/pair.hpp"

#include "sonia/utility/multimethod.hpp"

#include "invocation.hpp"

#include "sonia/logger/logger.hpp"

namespace sonia::invocation {

class invocable
{
public:
    virtual ~invocable() = default;

    virtual std::type_index get_type_index() const { return typeid(*this); }

    virtual bool has_method(string_view methodname) const;
    virtual bool try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result);
    virtual bool try_get_property(string_view propname, smart_blob& result) const;
    virtual bool try_set_property(string_view propname, blob_result const& val);

    virtual void on_property_change(string_view) {}

    virtual shared_ptr<invocable> self_as_invocable_shared() { return {}; }

    // method routine
    inline bool try_invoke(string_view methodname, span<const blob_result> args)
    {
        smart_blob result;
        return try_invoke(methodname, args, result);
    }

    smart_blob invoke(string_view methodname, span<const blob_result> args);
    inline smart_blob invoke(string_view name, std::initializer_list<const blob_result> args)
    {
        return invoke(name, span{args});
    }
        
    // properties routine
    smart_blob get_property(string_view propname) const;
    void set_property(string_view propname, blob_result const& val);
    void set_property(string_view propname, blob_result&& val);
};

struct method : multimethod
{
    virtual smart_blob operator()(invocable &, span<const blob_result> args) const = 0;
    virtual smart_blob operator()(invocable const&, span<const blob_result> args) const = 0;
};

struct fn_property_reader : multimethod
{
    virtual smart_blob get(invocable const&) const = 0;
};

struct fn_property_writer : multimethod
{
    virtual void set(invocable& object, string_view propname, blob_result const& value) const = 0;
};

template <auto FuncV>
struct concrete_method : method
{
    using sig_t = decltype(FuncV);
    using f_type = typename boost::function_types::function_type<sig_t>::type;
    using result_type = typename boost::function_types::result_type<f_type>::type;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;
    using pure_args_type = typename boost::mpl::pop_front<args_type>::type;
    using invocable_t = remove_reference_t<typename boost::mpl::at_c<args_type, 0>::type>;

    template <typename InvokableT, size_t ... Is>
    smart_blob do_job(std::index_sequence<Is...>, InvokableT& self, span<const blob_result> args) const
    {
        invocable_t* pinv = dynamic_cast<invocable_t*>(std::addressof(self));
        if (!pinv) {
            THROW_INTERNAL_ERROR("while method invoking can not cast from %1% to %2%"_fmt % typeid(self).name() % typeid(invocable_t).name());
        }
        std::tuple<invocable_t&, typename boost::mpl::at_c<pure_args_type, Is>::type ...> argstpl{
            *pinv, from_blob_at<typename boost::mpl::at_c<pure_args_type, Is>::type>(Is, args) ...};
        if constexpr (is_void_v<result_type>) {
            std::apply(FuncV, std::move(argstpl));
            return smart_blob{};
        } else if constexpr (is_same_v<result_type, smart_blob>) {
            return std::apply(FuncV, std::move(argstpl));
        } else {
            //blob_result br = particular_blob_result(std::apply(FuncV, std::move(argstpl)), self.get_blob_manager());
            //GLOBAL_LOG_INFO() << "do_job returns: " << br;
            //return br;
            return smart_blob { particular_blob_result(std::apply(FuncV, std::move(argstpl))) };
        }
    }

    smart_blob operator()(invocable & self, span<const blob_result> args) const override
    {
        return do_job(std::make_index_sequence<boost::mpl::size<pure_args_type>::type::value>{}, self, args);
    }

    smart_blob operator()(invocable const& self, span<const blob_result> args) const override
    {
        if constexpr (std::is_const_v<invocable_t>) {
            return do_job(std::make_index_sequence<boost::mpl::size<pure_args_type>::type::value>{}, self, args);
        } else {
            THROW_INTERNAL_ERROR("a mutable object is expected");
        }
    }

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(concrete_method);
};

template <typename InvokableT, typename GetterT>
struct concrete_fn_property_reader : fn_property_reader
{
    GetterT getter_;

    concrete_fn_property_reader(GetterT&& g)
        : getter_{ std::move(g) }
    {}

    smart_blob get(invocable const& obj) const override { return getter_(dynamic_cast<InvokableT const&>(obj)); }

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(concrete_fn_property_reader);
};

template <typename InvokableT, typename SetterT>
struct concrete_fn_property_writer : fn_property_writer
{
    SetterT setter_;

    concrete_fn_property_writer(SetterT&& s)
        : setter_{ std::move(s) }
    {}

    void set(invocable& obj, string_view propname, blob_result const& val) const override { setter_(dynamic_cast<InvokableT &>(obj), val); }

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(concrete_fn_property_writer);
};

template <auto FieldV>
struct field_fn_property_base
{
    using sig_t = decltype(FieldV);
    using f_type = typename boost::function_types::function_type<sig_t>::type;
    using property_type = remove_reference_t<typename boost::function_types::result_type<f_type>::type>;
    using args_type = typename boost::function_types::parameter_types<f_type>::type;
    using invocable_t = remove_reference_t<typename boost::mpl::at_c<args_type, 0>::type>;
};

template <auto FieldV>
struct field_fn_property_reader : field_fn_property_base<FieldV>, fn_property_reader
{
    smart_blob get(invocable const& obj) const override
    {
        return smart_blob{ particular_blob_result(dynamic_cast<typename field_fn_property_reader::invocable_t const&>(obj).*FieldV) };
    }

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(field_fn_property_reader);
};

template <auto FieldV>
struct field_fn_property_writer : field_fn_property_base<FieldV>, fn_property_writer
{
    using base_t = field_fn_property_base<FieldV>;
    using property_type = typename base_t::property_type;
    using invocable_t = typename base_t::invocable_t;

    void set(invocable& obj, string_view propname, blob_result const& val) const override
    {
        property_type& stored_val = dynamic_cast<invocable_t&>(obj).*FieldV;
        auto newval = from_blob<property_type>{}(val);
        if (stored_val != newval) {
            stored_val = newval;
            obj.on_property_change(propname);
        }
    }

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(field_fn_property_writer);
};

template <typename DerivedT, typename InheritedT = void>
class registrar
{
    //static std::once_flag registration_flag_;

    struct impl : singleton
    {
        impl()
        {
            DerivedT::do_registration(*this);
            if constexpr (!is_void_v<InheritedT>) {
                inherit(typeid(InheritedT));
            }
        }

        template <auto FuncV>
        void register_method(string_view name)
        {
            sonia::services::register_multimethod(concrete_method<FuncV>(), { typeid(DerivedT), name });
        }

        template <typename GetterT>
        void register_readonly_property(string_view name, GetterT&& g)
        {
            sonia::services::register_multimethod(concrete_fn_property_reader<DerivedT, remove_cvref_t<GetterT>>(std::forward<GetterT>(g)), { typeid(DerivedT), typeid(fn_property_reader), name });
        }

        template <typename SetterT>
        void register_writeonly_property(string_view name, SetterT&& g)
        {
            sonia::services::register_multimethod(concrete_fn_property_writer<DerivedT, remove_cvref_t<SetterT>>(std::forward<SetterT>(g)), { typeid(DerivedT), typeid(fn_property_writer), name });
        }

        template <typename GetterT, typename SetterT>
        void register_property(string_view name, GetterT&& g, SetterT&& s)
        {
            register_readonly_property(name, std::forward<GetterT>(g));
            register_writeonly_property(name, std::forward<SetterT>(s));
        }

        template <auto FieldV>
        void register_readonly_property(string_view name)
        {
            sonia::services::register_multimethod(field_fn_property_reader<FieldV>(), { typeid(DerivedT), typeid(fn_property_reader), name });
        }

        template <auto FieldV>
        void register_writeonly_property(string_view name)
        {
            sonia::services::register_multimethod(field_fn_property_writer<FieldV>(), { typeid(DerivedT), typeid(fn_property_writer), name });
        }

        template <auto FieldV>
        void register_property(string_view name)
        {
            register_readonly_property<FieldV>(name);
            register_writeonly_property<FieldV>(name);
        }

        void inherit(std::type_index from)
        {
            sonia::services::copy_multimethods({ from }, { typeid(DerivedT) });
        }
    };

protected:
    using registrar_type = impl;

public:
    registrar()
    {
        as_singleton<impl>();
        /*
        std::call_once(registration_flag_, [this] {
            services::on_close([] {
                new(&registration_flag_) std::once_flag{};
            });
            static_cast<DerivedT*>(this)->do_registration(*this); 
            if constexpr (!is_void_v<InheritedT>) {
                inherit(typeid(InheritedT));
            }
        });
        */
    }
};

//template <typename DerivedT, typename InheritedT>
//std::once_flag registrar<DerivedT, InheritedT>::registration_flag_{};

}
