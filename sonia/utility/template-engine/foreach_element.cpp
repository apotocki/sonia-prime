//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "foreach_element.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia::templates {

void foreach_element::do_registration(registrar_type& mr)
{
    register_property<&foreach_element::select>("select");
    register_property<&foreach_element::var>("var");

    //mr.register_method<&foreach_element::append>("append");
}

#if 0
class foreach_element_lua_resolver : public lua::language::resolver
{
    sonia::lua::language& lang_;
    foreach_element& ctx_;

public:
    explicit foreach_element_lua_resolver(foreach_element& ctx, sonia::lua::language& lang) : ctx_{ ctx }, lang_{ lang } {}

    blob_result index(string_view key) override
    {
        if (key == "operation"sv) {
            return function_blob_result(key);
        }
        return nil_blob_result();
    }

    bool newindex(string_view key, blob_result&& value) override
    {
        blob_result_unpin(&value);
        return false; 
    }

    blob_result invoke(string_view name, std::span<const blob_result> args) override
    {
        if (name == "operation"sv) {
            ctx_.run_elements(lang_);
        }
        return nil_blob_result();
    }
};

void foreach_element::run_elements(sonia::lua::language& lang)
{
    for (auto& elem : elements_) {
        shared_ptr<invokation::invokable> child = std::get<0>(elem);
        placaholders_t const& ps = std::get<1>(elem);
        for (auto const& pair : ps) {
            smart_blob actualvalue = lang.eval_inplace(pair.second, {});
            child->set_property(pair.first, *actualvalue);
        }
        if (auto cmp = dynamic_pointer_cast<templates::compound>(child); cmp) {
            cmp->run(lang);
        }
    }
}

template <typename BT>
inline void foreach_element::selector(sonia::identity<BT>, sonia::lua::language& lang, blob_result const& val)
{
    if constexpr (!is_void_v<BT>) {
        for (size_t i = 0; i < val.size / sizeof(blob_result); ++i) {
            BT const& value = reinterpret_cast<BT const*>(val.data)[i];

            if constexpr (is_same_v<blob_result, BT>) {
                lang.set_global_property(var, value);
            } else {
                blob_result bval = particular_blob_result(value);
                lang.set_global_property(var, bval);
            }
            run_elements(lang);
        }
    }
}

void foreach_element::run(sonia::lua::language & lang)
{
    std::ostringstream code;
    code << "for i," << var << " in ipairs(" << select << ") do\n";
    code << var << ".operation()\n";
    code << "end";
    
    cstring_view ifn = lang.append_inplace(code.str(), true);
    foreach_element_lua_resolver resolver{ *this, lang };
    smart_blob _ = lang.eval_inplace(ifn, {}, &resolver);

    /*
    smart_blob coll = lang.get_global_property(select);
    if (!coll->is_array) {
        lang.set_global_property(var, *coll);
        run_elements(lang);
    } else {
        blob_result_type_selector(*coll, [this, &lang](auto idt, blob_result const& val) {
            return selector(idt, lang, val);
        });
    }
    */
}
#endif

}
