//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <list>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/container/small_vector.hpp>

#include "xmlbuilder.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"

#include "sonia/utility/lang/vm.hpp"
#include "detail/types.hpp"

namespace sonia::xmlbuilder {

inline const string_view TARGET_NS = "http://sonia.one/builder/target"sv;
inline const string_view X_NS = "http://sonia.one/builder/x-templates"sv;

class advanced_external_builder;

namespace detail {

struct x_parameter
{
    small_string name;
    boost::container::small_vector<x_generic_type, 2> type;
    optional<smart_blob> default_value;
    int priority = 0;

    inline friend bool operator== (x_parameter const& l, x_parameter const& r) { return l.name == r.name; }
    inline friend bool operator== (x_parameter const& l, string_view r) { return l.name == r; }
    inline friend size_t hash_value(x_parameter const& xp) { return hash_value(xp.name); }

    bool has_type(x_generic_type const& t) const
    {
        return std::find(type.begin(), type.end(), t) != type.end();
    }

    void merge_types(x_parameter const& ft)
    {
        for (auto const& t : ft.type) {
            if (!has_type(t)) {
                type.emplace_back(t);
            }
        }
    }

};

class x_parameter_adopter
{
public:
    virtual ~x_parameter_adopter() = default;
    virtual void append_parameter(x_parameter const&) = 0;
};

class x_body_adopter
{
public:
    virtual ~x_body_adopter() = default;
    virtual void set_body_parameter(x_parameter const&) = 0;
};

struct x_type : public x_parameter_adopter, public x_body_adopter
{
    small_string name;
    boost::unordered_set<x_parameter> parameters;
    optional<x_parameter> body_parameter;

    inline friend bool operator== (x_type const& l, x_type const& r) { return l.name == r.name; }
    inline friend bool operator== (x_type const& l, string_view r) { return l.name == r; }
    inline friend size_t hash_value(x_type const& xt) { return hash_value(xt.name); }

    void append_parameter(x_parameter const& p) override
    {
        auto fit = parameters.find(p);
        if (fit == parameters.end()) {
            parameters.insert(fit, p);
        } else if (fit->priority < p.priority) {
            const_cast<x_parameter&>(*fit) = p;
        } else {
            const_cast<x_parameter&>(*fit).merge_types(p);
        }
    }

    void set_body_parameter(x_parameter const& p) override
    {
        if (!body_parameter) {
            body_parameter = p;
        } else if (body_parameter->priority < p.priority) {
            *body_parameter = p;
        } else {
            body_parameter->merge_types(p);
        }
    }

    x_parameter const* find_parameter(string_view attrname) const
    {
        auto it = parameters.find(attrname, hasher{}, std::equal_to{});
        return it != parameters.end() ? &*it : nullptr;
    }

    /*
    x_type() = default;

    x_type(small_string nm, boost::unordered_set<x_parameter> ps)
        : name {std::move(nm)}
        , parameters {std::move(ps)}
    {}
    */
};

}

class advanced_external_builder 
    : public external_builder
    , public attribute_resolver
{
    friend class x_generic_type_factory;
    friend class x_code_context;
    friend class x_type_context;
    friend class x_parameter_context;
    friend class x_body_context;
    friend class x_type_instance_context;

public:
    advanced_external_builder();

    virtual void create(string_view type, string_view id) = 0;
    virtual void set_text(string_view id, string_view text) = 0;
    virtual void set_property(string_view id, string_view propname, blob_result const& value) = 0;
    virtual void set_property_functional(string_view id, string_view propname, string_view code, func_type ft) = 0;
    virtual void append(string_view parentid, string_view childid) = 0;
    virtual void append_code(string_view code) = 0;

    class vm_context
    {
    public:
        advanced_external_builder& eb;

        using variable_type = smart_blob;
        static constexpr size_t command_max_size = sizeof(void*) * 2;
        bool is_zero(smart_blob const& b) const {
            return b.is_nil() || (is_basic_integral(b->type) && !b.as<uint64_t>());
        }
    };

    void append_element(span<element> parents, element&) override;
    void close_element(span<element> parents, element&) override;

    //attribute_resolver const& ar() const override final { return *this; }
    std::tuple<blob_result, func_type> operator()(string_view element, string_view attr_name, string_view attr_value) const override;

protected:
    string_view query_ns(element const&, span<const element> parents) const;
    string_view local_name(element const&) const;
    void append_type(detail::x_type &, small_string* ext);
    
    void instantiate(detail::x_type const&);
    std::string generate_id() const;

protected:
    vm::virtual_stack_machine<vm_context> vm_;
    size_t push_vm_value(blob_result b);
    boost::unordered_map<smart_blob, size_t, hasher> vm_values_; // value -> vm stack index

    /*
    struct create_object_cmd : vm::command<vm_context>
    {
        SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(create_object_cmd)
        size_t operator()(vm_context& ctx, span<smart_blob> stack) const override
        {
            ctx.eb.create(stack.back().as<string_view>(), stack[stack.size() - 2].as<string_view>());
            return stack.size() - 2;
        }
    };
    size_t create_object_cmd_id_;
    detail::x_type const* append_vm_object_creation_code(string_view lname, string_view eid);

    struct append_child_object_cmd : vm::command<vm_context>
    {
        SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(append_child_object_cmd)
        size_t operator()(vm_context& ctx, span<smart_blob> stack) const override
        {
            // append(string_view parentid, string_view childid)
            ctx.eb.append(stack.back().as<string_view>(), stack[stack.size() - 2].as<string_view>());
            return stack.size() - 2;
        }
    };
    size_t append_child_object_cmd_id_;
    void append_vm_append_child_object_creation_code(string_view parentid, string_view childid);
    */

    //automatic_polymorphic<attribute_resolver, 2 * sizeof(void*)> attr_rslv_;
    std::list<shared_ptr<external_builder>> contexts_;
    std::vector<std::tuple<small_string, std::string>> ns_prefixes_;
    std::string ns_;

    boost::unordered_set<detail::x_type> types_;
    //boost::unordered_map<std::string, size_t, hasher> root_objects_; // vm ip by object id
    mutable int id_counter_{ 0 };
};

}
