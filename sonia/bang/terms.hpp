//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/polymorphic_traits.hpp"

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/qname.hpp"

#include "sonia/string.hpp"
#include "sonia/mp/basic_decimal.hpp"

namespace sonia::lang::bang {

struct lex_tag;
struct enity_tag;
struct variable_tag;

using identifier = sonia::lang::identifier<uint32_t, lex_tag>;
using qname = sonia::lang::qname<identifier>;
using qname_view = sonia::lang::qname_view<identifier>;
using entity_identifier = sonia::lang::identifier<uint32_t, enity_tag>;
using variable_identifier = sonia::lang::identifier<uint32_t, variable_tag>;

template <typename T>
struct annotated
{
    T value;
    lex::resource_location location;

    inline bool operator==(annotated const& r) const noexcept { return value == r.value; }
    inline auto operator<=>(annotated const& r) const noexcept { return value <=> r.value; }

    inline explicit operator bool() const noexcept { return (bool)value; }
};

using annotated_string_view = annotated<string_view>;
using annotated_identifier = annotated<identifier>;
using annotated_qname = annotated<qname>;
using annotated_qname_identifier = annotated<qname_identifier>;
using annotated_entity_identifier = annotated<entity_identifier>;
using annotated_bool = annotated<bool>;
using annotated_integer = annotated<mp::integer>;
using annotated_decimal = annotated<mp::decimal>;
using annotated_string = annotated<small_string>;

struct annotated_nil
{
    lex::resource_location location;
};

enum class parameter_constraint_modifier_t : uint8_t
{
    mutable_value_type = 1,
    const_value_type = 2,
    value_type = 3,
    const_value = 4
};

class indirect : public polymorphic_clonable_and_movable
{
public:
    virtual ~indirect() = default;
};

}
