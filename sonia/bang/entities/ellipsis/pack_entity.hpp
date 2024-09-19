//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/bang/semantic_fwd.hpp"

namespace sonia::lang::bang {

class pack_entity : public signatured_entity
{
    entity_signature sig_;

public:
    pack_entity(entity_identifier type, entity_signature&& sig)
        : signatured_entity{ type }, sig_{ std::move(sig) }
    {
        //BOOST_ASSERT(sig_.named_fields().empty());
        //BOOST_ASSERT(sig_.positioned_fields().size() == 1);
        BOOST_ASSERT(sig_.names().empty());
        BOOST_ASSERT(sig_.fields().size() == 1);
    }

    inline entity_signature const* signature() const noexcept override final { return &sig_; }

    entity_identifier element_type() const { return sig_.fields().front().entity_id(); }

    void visit(entity_visitor const& v) const override { v(*this); }
};

}
