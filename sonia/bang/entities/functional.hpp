//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>
#include <boost/container/small_vector.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/utility/lang/qname.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class functional
{
public:
    using identifier_type = qname_identifier;

    class pattern
    {
        mp::decimal weight_ = 1;

    public:
        //explicit pattern(shared_ptr<entity> ent)
        //    : entity_{ std::move(ent) }
        //{
        //
        //}

        virtual error_storage is_matched(fn_compiler_context& ctx, pure_call_t& call) const = 0;
        virtual std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx) const = 0; // returns const value
        virtual std::expected<entity_identifier, error_storage> apply(fn_compiler_context& ctx) const; // returns type of result

        inline mp::decimal const& get_weight() const noexcept { return weight_; }

        
    private:
        //shared_ptr<entity> entity_;
    };

    explicit functional(qname_identifier idval) : id_{ idval } {}

    qname_identifier id() const { return id_; }
    entity_identifier default_entity() const { return default_entity_; }

    void set_default_entity(entity_identifier e) { default_entity_ = std::move(e); }

    void push(shared_ptr<pattern> p)
    {
        patterns_.push_back(std::move(p));
    }

    // looking by argument expressions
    std::expected<pattern const*, error_storage> find(fn_compiler_context&, pure_call_t& call) const;

private:
    qname_identifier id_;
    entity_identifier default_entity_; // corresponds to name without call
    boost::container::small_vector<shared_ptr<pattern>, 1> patterns_;
};

class fieldset_pattern : public functional::pattern
{
   
public:
    fieldset_pattern() = default;

    virtual fieldset const& get_fieldset() const noexcept = 0;

    error_storage is_matched(fn_compiler_context&, pure_call_t&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx) const override;
};





}
