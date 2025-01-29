//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/lang/entity.hpp"
#include "ast_terms.hpp"

namespace sonia::lang::bang {

using entity_base_t = sonia::lang::entity<entity_identifier, lex::resource_location>;

class entity_visitor;

class unit;

class fn_compiler_context;

class entity_signature;

class entity : public entity_base_t
{
    entity_identifier entity_type_;

public:
    using entity_base_t::entity_base_t;

    virtual bool is(fn_compiler_context&, entity_identifier eid) const noexcept { return eid == entity_type_; }

    inline entity_identifier get_type() const noexcept { return entity_type_; }

    inline void set_type(entity_identifier eid) noexcept { entity_type_ = eid; }

    virtual entity_signature const* signature() const noexcept { return nullptr; }

    virtual void visit(entity_visitor const&) const
    {
        THROW_NOT_IMPLEMENTED_ERROR("entity::visit");
    }

    virtual size_t hash() const noexcept { return 0; }
    virtual bool equal(entity const&) const noexcept { return true; }

    friend inline bool operator==(entity const& l, entity const& r) noexcept { return l.equal(r); }

    virtual std::ostream& print_to(std::ostream& os, unit const& u) const;
};

inline size_t hash_value(entity const& e) noexcept { return e.hash(); }

template <typename ValueT>
class value_entity : public entity
{
public:
    template <typename ArgT>
    requires(!is_same_v<value_entity, remove_cvref_t<ArgT>>)
    explicit value_entity(ArgT && v)
        : value_{ std::forward<ArgT>(v) }
    {}

    void visit(entity_visitor const& v) const override { v(*this); }

    ValueT const& value() const { return value_; }

    size_t hash() const noexcept override { return hasher{}(value_); }

    bool equal(entity const& rhs) const noexcept override
    {
        if (value_entity const* pr = dynamic_cast<value_entity const*>(&rhs); pr) {
            return pr->value_ == value_;
        }
        return false;
    }

    std::ostream& print_to(std::ostream& os, unit const& u) const override
    {
        return entity::print_to(os, u) << "value_entity("sv << value_ << ")"sv;
    }

private:
    ValueT value_;
};

//class basic_entity : public entity
//{
//    entity_identifier entity_type_;
//public:
//    using entity::entity;
//    void set_type(entity_identifier eid) { entity_type_ = eid; }
//    bool is(fn_compiler_context&, entity_identifier eid) const override { return eid == entity_type_; }
//};

class pack_entity;
using string_literal_entity = value_entity<small_string>;
using bool_literal_entity = value_entity<bool>;
using integer_literal_entity = value_entity<mp::integer>;
using decimal_literal_entity = value_entity<mp::decimal>;
using identifier_entity = value_entity<identifier>;
using qname_identifier_entity = value_entity<qname_identifier>;

class enum_entity;
class variable_entity;
class functional_entity;
class function_entity;
class external_function_entity;
class type_entity;
class variable_entity;

class entity_visitor
{
public:
    virtual ~entity_visitor() = default;

    virtual void operator()(string_literal_entity const&) const = 0;
    virtual void operator()(bool_literal_entity const&) const = 0;
    virtual void operator()(integer_literal_entity const&) const = 0;
    virtual void operator()(decimal_literal_entity const&) const = 0;
    virtual void operator()(identifier_entity const&) const = 0;
    virtual void operator()(qname_identifier_entity const&) const = 0;
    virtual void operator()(pack_entity const&) const = 0;
    virtual void operator()(function_entity const&) const = 0;
    virtual void operator()(external_function_entity const&) const = 0;
    virtual void operator()(variable_entity const&) const = 0;

    virtual void operator()(functional_entity const&) const = 0;

};

// ======================================================================== values
struct function_value { qname_identifier mangled_name; };
using value_t = make_recursive_variant<
    null_t, bool, uint64_t, mp::integer, mp::decimal, small_string, identifier,// shared_ptr<beng_object>,
    entity_identifier, function_value,
    std::vector<recursive_variant_>
>::type; // to do: tuples

}
