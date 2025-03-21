//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic_fwd.hpp"
#include "sonia/bang/unit.hpp"

namespace sonia::lang::bang {

template <typename ValueT>
class literal_entity : public entity
{
public:
    template <typename ArgT>
    literal_entity(ArgT&& v, entity_identifier t)
        : value_{ std::forward<ArgT>(v) }
        , type_{ std::move(t) }
    {}

    template <typename ArgT>
    requires(!is_same_v<literal_entity, remove_cvref_t<ArgT>>)
    explicit literal_entity(ArgT&& v)
        : value_{ std::forward<ArgT>(v) }
    {}

    void visit(entity_visitor const& v) const override { v(*this); }

    entity_identifier get_type() const noexcept override { return type_; }
    void set_type(entity_identifier type) noexcept { type_ = type; }

    ValueT const& value() const { return value_; }

    size_t hash() const noexcept override { return hasher{}(value_, type_); }

    bool equal(entity const& rhs) const noexcept override
    {
        if (literal_entity const* pr = dynamic_cast<literal_entity const*>(&rhs); pr) {
            return pr->value_ == value_ && pr->type_ == type_;
        }
        return false;
    }

    std::ostream& print_to(std::ostream& os, unit const& u) const override
    {
#ifdef SONIA_LANG_DEBUG
        if constexpr (std::is_same_v<ValueT, identifier> || std::is_same_v<ValueT, qname_identifier>) {
            return u.print_to(do_print_name(entity::print_to(os, u)) << '(' << value_.debug_name << "): "sv, type_);
        } else if constexpr (std::is_same_v<ValueT, bool>) {
            return u.print_to(do_print_name(entity::print_to(os, u)) << '(' << (value_ ? "true"sv : "false"sv) << "): "sv, type_);
        }
#endif
        return u.print_to(do_print_name(entity::print_to(os, u)) << '(' << value_ << "): "sv, type_);
    }

private:
    inline std::ostream& do_print_name(std::ostream& os) const
    {
        if constexpr (std::is_same_v<ValueT, identifier>) {
            return os << "identifier_entity"sv;
        } else if constexpr (std::is_same_v<ValueT, qname_identifier>) {
            return os << "qname_entity"sv;
        } else if constexpr (std::is_same_v<ValueT, bool>) {
            return os << "boolean_entity"sv;
        } else if constexpr (std::is_same_v<ValueT, small_string>) {
            return os << "string_entity"sv;
        } else if constexpr (std::is_same_v<ValueT, mp::integer>) {
            return os << "integer_entity"sv;
        } else if constexpr (std::is_same_v<ValueT, mp::decimal>) {
            return os << "decimal_entity"sv;
        } else {
            return os << "literal_entity"sv;
        }
    }

    ValueT value_;
    entity_identifier type_;
};

}
