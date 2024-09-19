//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>
#include <boost/container/small_vector.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/utility/lang/qname.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class functional
{
public:
    using identifier_type = qname_identifier;
    using qname_view_type = qname_view;
    using binding_set_t = boost::container::small_flat_map<annotated_identifier, entity_identifier, 16>;

    struct match_descriptor
    {
        binding_set_t bindings;
        entity_signature signature;
        entity_identifier result;
        inline void reset() noexcept { bindings.clear(); signature.reset_fields(); result = {}; }
    };

    class pattern
    {
    protected:
        mp::decimal weight_{1};
        lex::resource_location location_;

    public:
        virtual std::expected<int, error_storage> is_matched(fn_compiler_context& ctx, pure_call_t const& call, match_descriptor&) const = 0; // returns the match weight or an error
        virtual std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx, functional::match_descriptor&) const = 0; // returns const value
        virtual std::expected<entity_identifier, error_storage> apply(fn_compiler_context& ctx, functional::match_descriptor&) const; // returns type of result

        inline mp::decimal const& get_weight() const noexcept { return weight_; }
        inline lex::resource_location const& location() const noexcept { return location_; }
     };

    functional(qname_identifier idval, qname_view qv)
        : id_{ idval }
        , qnameids_{ qv.begin(), qv.end() }
    {
        BOOST_ASSERT(qv.is_absolute());
    }

    inline qname_identifier id() const noexcept { return id_; }
    inline qname_view name() const noexcept { return qname_view{ qnameids_, true }; }

    inline entity_identifier const& default_entity() const noexcept { return default_entity_; }
    inline void set_default_entity(entity_identifier e) noexcept { default_entity_ = std::move(e); }

    void push(shared_ptr<pattern> p)
    {
        patterns_.push_back(std::move(p));
    }

    // looking by argument expressions
    std::expected<pattern const*, error_storage> find(fn_compiler_context&, pure_call_t const&, match_descriptor &) const;

private:
    qname_identifier id_;
    boost::container::small_vector<identifier, 4> qnameids_;
    entity_identifier default_entity_; // corresponds to name without call
    boost::container::small_vector<shared_ptr<pattern>, 1> patterns_;
};

struct pattern_variable { identifier id; };
using pattern_expression_t = variant<annotated_qname_identifier, entity_identifier, pattern_variable>;
using patern_fieldset_t = fieldset<boost::container::small_vector<pattern_expression_t, 1>>;

class function_descriptor
{
public:
    struct generic_field
    {
        optional<annotated_identifier> iname;
        optional<pattern_expression_t> constraint;
        boost::container::small_vector<pattern_expression_t, 1> concepts;
        boost::container::small_vector<annotated_identifier, 1> bindings;
        parameter_constraint_modifier_t constraint_type;
    };

    struct named_field : generic_field
    {
        annotated_identifier ename;
        named_field(annotated_identifier n, generic_field f)
            : ename{ std::move(n) }
            , generic_field { std::move(f) }
        {}

        friend inline bool operator==(named_field const& l, named_field const& r) noexcept
        {
            return l.ename.value == r.ename.value;
        }

        friend inline auto operator<=>(named_field const& l, named_field const& r) noexcept
        {
            return l.ename.value <=> r.ename.value;
        }
    };

    struct positioned_field : generic_field
    {

    };

    using variables_t = boost::unordered_set<identifier>;

    function_descriptor() = default;

    explicit function_descriptor(qname_identifier idval) : id_{ idval } {}

    inline qname_identifier id() const noexcept { return id_; }
    inline void set_id(qname_identifier idval) noexcept { id_ = idval; }

    template <typename ArgT>
    void set_nfields(ArgT&& arg) { nfields_ = std::forward<ArgT>(arg); }

    template <typename ArgT>
    void set_pfields(ArgT&& arg) { pfields_ = std::forward<ArgT>(arg); }

    inline span<const named_field> named_fields() const noexcept { return nfields_; }
    inline span<const positioned_field> positioned_fields() const noexcept { return pfields_; }

    pattern_expression_t const* result_type() const { return result_type_ ? &*result_type_ : nullptr; }
    void set_result_type(pattern_expression_t rt) { result_type_ = rt; }

    inline variables_t& variables() noexcept { return variables_; }
    inline variables_t& varparams() noexcept { return varparams_; }

    named_field const* find_named_field(identifier name) const;

private:
    std::vector<named_field> nfields_;
    std::vector<positioned_field> pfields_;

    optional<pattern_expression_t> result_type_;

    variables_t variables_;
    variables_t varparams_;

    qname_identifier id_;
};

class fieldset_pattern : public functional::pattern
{
public:
    fieldset_pattern() = default;

    using fieldset_t = patern_fieldset_t;

    virtual fieldset_t const& get_fieldset() const noexcept = 0;

    std::expected<int, error_storage> is_matched(fn_compiler_context&, pure_call_t const&, functional::match_descriptor&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx, functional::match_descriptor&) const override;
};





}
