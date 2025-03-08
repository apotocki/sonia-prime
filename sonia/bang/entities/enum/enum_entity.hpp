//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/optional.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

namespace sonia::lang::bang {

class enum_entity : public basic_signatured_entity
{
    qname name_;
    std::vector<identifier> cases_;
    mutable entity_identifier underlying_tuple_eid_;

public:
    explicit enum_entity(qname qn, entity_identifier type, entity_signature&& sgn, std::vector<identifier> cases)
        : basic_signatured_entity{ std::move(type), std::move(sgn) }
        , name_{ qn }
        , cases_{ std::move(cases) }
    {}

    inline qname_view name() const noexcept { return name_; }

    optional<size_t> find(identifier) const;

    /*
    struct enum_case
    {
        identifier name;
        value_t value;

        template <typename VArgT>
        enum_case(identifier n, VArgT && v) : name{ std::move(n) }, value{ std::forward<VArgT>(v) } {}

        friend inline bool operator==(enum_case const& l, enum_case const& r) { return l.name == r.name; }
        friend inline auto operator<=>(enum_case const& l, enum_case const& r) { return l.name <=> r.name; }
    };

    std::vector<enum_case> cases;

    
    */
};

}
