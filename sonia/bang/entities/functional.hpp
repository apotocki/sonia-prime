//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>
//#include <boost/container/flat_map.hpp>
#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/small_vector.hpp"
#include "sonia/utility/lang/qname.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

//#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/semantic/managed_expression_list.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

class functional_binding_set
{
    small_vector<std::pair<identifier, small_vector<entity_identifier, 1>>, 16> binding_;

public:
    inline void reset(size_t cnt)
    {
        binding_.clear();
        binding_.resize(cnt);
    }

    optional<span<const entity_identifier>> lookup(identifier id)
    {
        auto it = std::lower_bound(binding_.begin(), binding_.end(), id, [](auto const& pair, identifier id) { return pair.first < id; });
        if (it == binding_.end() || it->first != id) return {};
        return span{ it->second.data(), it->second.size() };
    }
};

struct functional_match_descriptor
{
    functional_binding_set bindings;
    entity_signature signature;
    entity_identifier result;
    semantic::managed_expression_list call_expressions;

    using se_cont_iterator = semantic::expression_list_t::const_iterator;
    using se_rng_t = std::pair<se_cont_iterator, se_cont_iterator>;
    small_vector<std::pair<identifier, small_vector<se_rng_t, 8>>, 8> named_arguments; // arg name -> [expressions]
    small_vector<small_vector<se_rng_t, 8>, 8> unnamed_arguments; // argnum -> [expressions]
    int weight{ 0 };

    inline explicit functional_match_descriptor(unit& u) noexcept : call_expressions{ u } {}

    functional_match_descriptor(functional_match_descriptor const&) = delete;
    functional_match_descriptor& operator= (functional_match_descriptor const&) = delete;

    void push_named_argument_expressions(identifier, se_cont_iterator &, semantic::managed_expression_list const&);
    void push_unnamed_argument_expressions(size_t, se_cont_iterator&, semantic::managed_expression_list const&);

    span<const se_rng_t> get_named_arguments(identifier) const noexcept;
    span<const se_rng_t> get_position_arguments(size_t pos) const noexcept;

    inline void reset() noexcept { signature.reset_fields(); result = {}; }

private:
    static void do_prepare_range(se_cont_iterator& it_before, semantic::managed_expression_list const& exprs, optional<se_rng_t>& rng);
};

using functional_match_descriptor_ptr = shared_ptr<functional_match_descriptor>;

class functional
{
public:
    using identifier_type = qname_identifier;
    using qname_view_type = qname_view;
    //using binding_set_t = small_vector<std::pair<identifier, small_vector<entity_identifier, 1>>, 16>;
    //using binding_set_t = boost::container::small_flat_map<annotated_identifier, entity_identifier, 16>;

    class pattern
    {
    protected:
        mp::decimal weight_{ 1 };
        lex::resource_location location_;

    public:
        virtual std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const& expected_result) const = 0; // returns the match weight or an error
        virtual std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx, functional_match_descriptor&) const = 0; // returns const value
        virtual std::expected<entity_identifier, error_storage> apply(fn_compiler_context& ctx, functional_match_descriptor&) const; // returns type of result

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
    std::expected<std::pair<functional::pattern const*, functional_match_descriptor_ptr>, error_storage> find(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const& expected_result) const;

private:
    qname_identifier id_;
    small_vector<identifier, 4> qnameids_;
    entity_identifier default_entity_; // corresponds to name without call
    small_vector<shared_ptr<pattern>, 1> patterns_;
};

//struct pattern_local_variable { size_t var_index; };
//struct pattern_variable { identifier id; };
//using pattern_expression_t = variant<nullptr_t, annotated_qname_identifier, entity_identifier, pattern_local_variable, pattern_variable>;
using patern_fieldset_t = fieldset<boost::container::small_vector<syntax_expression_t, 1>>;

struct pattern_parameter_descriptor
{
    parameter_constraint_modifier_t modifier;
    parameter_constraint_set_t constraints;
    optional<syntax_expression_t> default_value;
};

class function_descriptor
{
public:
    struct generic_field
    {
        optional<annotated_identifier> iname;
        optional<syntax_expression_t> constraint;
        boost::container::small_vector<syntax_expression_t, 1> concepts;
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

    syntax_expression_t const* result_type() const { return result_type_ ? &*result_type_ : nullptr; }
    void set_result_type(syntax_expression_t rt) { result_type_ = rt; }

    inline variables_t& variables() noexcept { return variables_; }
    inline variables_t& varparams() noexcept { return varparams_; }

    named_field const* find_named_field(identifier name) const;

private:
    std::vector<named_field> nfields_;
    std::vector<positioned_field> pfields_;

    optional<syntax_expression_t> result_type_;

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

    std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const&) const override;
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx, functional_match_descriptor&) const override;
};





}
