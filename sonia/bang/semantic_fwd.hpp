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
public:
    using entity_base_t::entity_base_t;

    virtual bool is(fn_compiler_context&, entity_identifier eid) const noexcept { return eid == get_type(); }

    virtual entity_identifier get_type() const noexcept;
    
    virtual entity_signature const* signature() const noexcept { return nullptr; }

    virtual void visit(entity_visitor const&) const;

    virtual size_t hash() const noexcept { return 0; }
    virtual bool equal(entity const&) const noexcept { return true; }

    friend inline bool operator==(entity const& l, entity const& r) noexcept { return l.equal(r); }

    friend inline size_t hash_value(entity const& e) noexcept { return e.hash(); }

    virtual std::ostream& print_to(std::ostream& os, unit const& u) const;
};

using entity_ptr = shared_ptr<entity>;
template <typename ValueT> class literal_entity;

using string_literal_entity = literal_entity<small_string>;
using bool_literal_entity = literal_entity<bool>;
using integer_literal_entity = literal_entity<mp::integer>;
using decimal_literal_entity = literal_entity<mp::decimal>;
using identifier_entity = literal_entity<identifier>;
using qname_identifier_entity = literal_entity<qname_identifier>;
using qname_entity = literal_entity<qname>;

class empty_entity;
class enum_entity;
class functional_entity;
class function_entity;
class external_function_entity;
class type_entity;
class extern_variable_entity;
//class vector_type_entity;
//class array_type_entity;

class entity_visitor
{
public:
    virtual ~entity_visitor() = default;
    virtual void operator()(entity const&) const = 0;

    virtual void operator()(string_literal_entity const&) const = 0;
    virtual void operator()(bool_literal_entity const&) const = 0;
    virtual void operator()(integer_literal_entity const&) const = 0;
    virtual void operator()(decimal_literal_entity const&) const = 0;
    virtual void operator()(identifier_entity const&) const = 0;
    virtual void operator()(qname_identifier_entity const&) const = 0;
    virtual void operator()(empty_entity const&) const = 0;

    virtual void operator()(function_entity const&) const = 0;
    virtual void operator()(external_function_entity const&) const = 0;
    virtual void operator()(extern_variable_entity const&) const = 0;

    virtual void operator()(functional_entity const&) const = 0;
};

class entity_visitor_adapter : public entity_visitor
{
    void operator()(entity const&) const override {}
    void operator()(string_literal_entity const&) const override {}
    void operator()(bool_literal_entity const&) const override {}
    void operator()(integer_literal_entity const&) const override {}
    void operator()(decimal_literal_entity const&) const override {}
    void operator()(identifier_entity const&) const override {}
    void operator()(qname_identifier_entity const&) const override {}
    void operator()(empty_entity const&) const override {}
    
    void operator()(function_entity const&) const override {}
    void operator()(external_function_entity const&) const override {}
    void operator()(extern_variable_entity const&) const override {}
    void operator()(functional_entity const&) const override {}
};

// typed empty_entity: entities with different types are not equal
class empty_entity : public entity
{
    entity_identifier type_;

public:
    inline explicit empty_entity(entity_identifier type) noexcept : type_{ type } {}

    entity_identifier get_type() const noexcept override { return type_; }

    void visit(entity_visitor const& v) const override { v(*this); }

    bool equal(entity const& rhs) const noexcept override;

    size_t hash() const noexcept override;

    std::ostream& print_to(std::ostream& os, unit const& u) const override;
};


//class vector_type_entity : public entity
//{
//    entity_signature sig_;
//
//public:
//    entity_identifier element_type;
//
//    vector_type_entity(unit&, entity_identifier et) noexcept;
//
//    void visit(entity_visitor const& v) const override { v(*this); }
//
//    entity_signature const* signature() const noexcept override { return &sig_; }
//
//    bool equal(entity const& rhs) const noexcept override;
//
//    size_t hash() const noexcept override;
//
//    std::ostream& print_to(std::ostream& os, unit const& u) const override;
//};
//
//class array_type_entity : public entity
//{
//    entity_signature sig_;
//
//public:
//    entity_identifier element_type;
//    size_t size;
//
//    array_type_entity(unit&, entity_identifier et, size_t sz) noexcept;
//
//    entity_signature const* signature() const noexcept override { return &sig_; }
//
//    bool equal(entity const& rhs) const noexcept override;
//
//    size_t hash() const noexcept override;
//
//    std::ostream& print_to(std::ostream& os, unit const& u) const override;
//};

class local_variable
{
public:
    annotated_identifier name;
    entity_identifier type;
    intptr_t index;
    bool is_weak = false;

    inline friend size_t hash_value(local_variable const& lv) noexcept
    {
        return hash_value(lv.name.value);
    }

    inline friend bool operator==(local_variable const& l, local_variable const& r) noexcept
    {
        return l.name.value == r.name.value;
    }

    inline friend auto operator<=>(local_variable const& l, local_variable const& r) noexcept
    {
        return l.name.value <=> r.name.value;
    }
};

struct local_variable_compare
{
    using is_transparent = void;
    inline bool operator()(local_variable const& l, local_variable const& r) const noexcept
    {
        return l < r;
    }

    inline bool operator()(identifier const& l, local_variable const& r) const noexcept
    {
        return l < r.name.value;
    }

    inline bool operator()(local_variable const& l, identifier const& r) const noexcept
    {
        return l.name.value < r;
    }
};

// ======================================================================== values
struct function_value { qname_identifier mangled_name; };
using value_t = make_recursive_variant<
    null_t, bool, uint64_t, mp::integer, mp::decimal, small_string, identifier,// shared_ptr<beng_object>,
    entity_identifier, function_value,
    std::vector<recursive_variant_>
>::type; // to do: tuples

}

