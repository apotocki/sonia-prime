//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/container/small_vector.hpp>
#include "sonia/bang/semantic_fwd.hpp"

namespace sonia::lang::bang {



//class type_entity2 : public entity
//{
//public:
//    explicit type_entity2(entity_identifier id) : entity { std::move(id) }
//    {}
//};

//class const_entity : public entity
//{
//public:
//    using entity::entity;
//};
//
//class string_literal_entity : public const_entity
//{
//public:
//    explicit string_literal_entity(string_view sv)
//        : value_{ sv }
//    {}
//
//    void visit(entity_visitor const& v) const override { v(*this); }
//
//    small_string const& value() const { return value_; }
//
//private:
//    small_string value_;
//};
//
//class decimal_literal_entity : public const_entity
//{
//public:
//    explicit decimal_literal_entity(mp::decimal d)
//        : value_{ std::move(d) }
//    {}
//
//    void visit(entity_visitor const& v) const override { v(*this); }
//
//    mp::decimal const& value() const { return value_; }
//
//private:
//    mp::decimal value_;
//};

//class const_entity : public entity
//{
//public:
//    const_entity(symbol sb, smart_blob value, entity_identifier t)
//        : symbol_{ std::move(sb) }, value_{ std::move(value) }, type_{ std::move(t) }
//    {}
//
//private:
//    symbol symbol_;
//    smart_blob value_;
//    entity_identifier type_;
//};

}
