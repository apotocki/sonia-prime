//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <expected>
#include <boost/container/flat_map.hpp>
#include <boost/unordered_set.hpp>

#include "sonia/variant.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/small_vector.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/lang/qname.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class fn_compiler_context;

class functional_binding
{
public:
    virtual ~functional_binding() = default;

    using value_type = variant<entity_identifier, shared_ptr<entity>>;

    // bouund entity_identifier can not be changed, but bound entity can be updated
    virtual optional<value_type> lookup(identifier) const noexcept = 0;
    virtual void emplace_back(annotated_identifier, value_type) = 0;
};

class functional_binding_set : public functional_binding
{
    small_vector<value_type, 16> binding_;
    small_vector<identifier, 16> binding_names_;
    small_vector<lex::resource_location, 16> binding_locations_;

public:
    inline void reset() noexcept;
    
    optional<value_type> lookup(identifier) const noexcept override;

    void emplace_back(annotated_identifier, value_type) override;

    template <typename FT>
    requires std::is_invocable_v<FT, identifier, lex::resource_location const&, value_type&>
    void for_each(FT && ftor)
    {
        for (auto it = binding_names_.begin(), eit = binding_names_.end(); it != eit; ++it) {
            auto pos = it - binding_names_.begin();
            std::forward<FT>(ftor)(*it, binding_locations_[pos], binding_[pos]);
        }
    }
};

struct parameter_match_result
{
    enum class modifier : uint8_t
    {
        undefined = 0,
        is_expr = 1,
        is_constexpr = 2,
        is_uniadic = 4,
        is_variadic = 8,
    };
    using se_cont_iterator = semantic::expression_list_t::const_iterator;
    using se_rng_t = std::pair<se_cont_iterator, se_cont_iterator>;

    small_vector<se_rng_t, 4> expressions;
    small_vector<entity_identifier, 4> result;
    uint8_t mod = (uint8_t)modifier::undefined;

    void append_result(bool variadic, entity_identifier, se_cont_iterator before_start_it, semantic::expression_list_t&);
    void append_result(bool variadic, entity_identifier);

    void set_constexpr(bool);
    void set_variadic(bool);

    inline bool is_undefined() const noexcept { return mod == (uint8_t)modifier::undefined; }
    inline bool is_constexpr() const noexcept { return !!(mod & (uint8_t)modifier::is_constexpr); }
    inline bool is_variadic() const noexcept { return !!(mod & (uint8_t)modifier::is_variadic); }
};

class functional_match_descriptor
{
    //using se_cont_iterator = semantic::expression_list_t::const_iterator;
    //using se_rng_t = std::pair<se_cont_iterator, se_cont_iterator>;
    //small_vector<std::pair<identifier, small_vector<se_rng_t, 8>>, 8> named_arguments; // arg name -> [expressions]
    //small_vector<small_vector<se_rng_t, 8>, 8> unnamed_arguments; // argnum -> [expressions]
    
    std::list<parameter_match_result> pmrs_;
    boost::container::small_flat_map<identifier, parameter_match_result*, 8> named_matches_;
    small_vector<parameter_match_result*, 8> positional_matches_;

public:
    functional_binding_set bindings;
    semantic::managed_expression_list call_expressions;
    lex::resource_location location;
    entity_identifier result;
    int weight{ 0 };

    inline explicit functional_match_descriptor(unit& u) noexcept : call_expressions{ u } {}
    virtual ~functional_match_descriptor() = default;

    functional_match_descriptor(functional_match_descriptor const&) = delete;
    functional_match_descriptor& operator= (functional_match_descriptor const&) = delete;

    template <typename FT>
    inline void for_each_named_match(FT && ft) const
    {
        for (auto [nm, pmr] : named_matches_) {
            if constexpr (std::is_invocable_v<FT, identifier, parameter_match_result&>) {
                std::forward<FT>(ft)(nm, *pmr);
            } else {
                std::forward<FT>(ft)(*pmr);
            }
        }
    }

    template <typename FT>
    inline void for_each_positional_match(FT&& ft) const
    {
        size_t pos = 0;
        for (auto pmr : positional_matches_) {
            if constexpr (std::is_invocable_v<FT, size_t, parameter_match_result&>) {
                std::forward<FT>(ft)(pos++, *pmr);
            } else {
                std::forward<FT>(ft)(*pmr);
            }
        }
    }

    template <typename FT>
    inline void for_each_match(FT&& ft) const
    {
        for_each_named_match(std::forward<FT>(ft));
        for_each_positional_match(std::forward<FT>(ft));
    }

    parameter_match_result& get_match_result(identifier /* external name */);
    parameter_match_result& get_match_result(size_t);

    entity_signature build_signature(unit&, qname_identifier);
    void reset() noexcept;

private:
    //static void do_prepare_range(se_cont_iterator& it_before, semantic::managed_expression_list const& exprs, optional<se_rng_t>& rng);
};

using functional_match_descriptor_ptr = shared_ptr<functional_match_descriptor>;

class functional
{
public:
    using identifier_type = qname_identifier;
    using qname_view_type = qname_view;
    
    class pattern
    {
    protected:
        mp::decimal weight_{ 1 };
        lex::resource_location location_;

    public:
        virtual std::expected<functional_match_descriptor_ptr, error_storage> try_match(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const& expected_result) const = 0; // returns the match weight or an error
        virtual std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const = 0; // returns const value
        virtual error_storage apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const;

        inline mp::decimal const& get_weight() const noexcept { return weight_; }
        inline lex::resource_location const& location() const noexcept { return location_; }

        virtual std::ostream& print(unit const&, std::ostream& s) const { return s << "some pattern"; }
    };

    class match
    {
        qname_identifier fid_;
        pattern const* ptrn_;
        functional_match_descriptor_ptr md_;

    public:
        inline match(qname_identifier fid, pattern const* p, functional_match_descriptor_ptr md) noexcept
            : fid_{ fid }, ptrn_{ p }, md_{ std::move(md) } 
        {}
        
        inline [[nodiscard]] error_storage apply(fn_compiler_context& ctx)
        {
            return ptrn_->apply(ctx, fid_, *md_);
        }

        inline std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context& ctx)
        {
            return ptrn_->const_apply(ctx, fid_, *md_);
        }
    };

    class entity_resolver
    {
    public:
        virtual ~entity_resolver() = default;
        virtual std::expected<entity_identifier, error_storage> const_resolve(fn_compiler_context&) const = 0;
        virtual lex::resource_location const& location() const = 0;
    };

    functional(qname_identifier idval, qname_view qv)
        : id_{ idval }
        , qnameids_{ qv.begin(), qv.end() }
    {
        BOOST_ASSERT(qv.is_absolute());
    }

    inline qname_identifier id() const noexcept { return id_; }
    inline qname_view name() const noexcept { return qname_view{ qnameids_, true }; }

    std::expected<entity_identifier, error_storage> default_entity(fn_compiler_context&) const;
    [[nodiscard]] error_storage set_default_entity(annotated_entity_identifier);
    [[nodiscard]] error_storage set_default_entity(shared_ptr<entity_resolver>);

    void push(shared_ptr<pattern> p)
    {
        patterns_.push_back(std::move(p));
    }

    // looking by argument expressions (pattern matching)
    std::expected<match, error_storage> find(fn_compiler_context&, pure_call_t const&, annotated_entity_identifier const& expected_result = annotated_entity_identifier{}) const;

private:
    qname_identifier id_;
    small_vector<identifier, 4> qnameids_;
    mutable variant<annotated_entity_identifier, shared_ptr<entity_resolver>> default_entity_; // corresponds to name without call
    small_vector<shared_ptr<pattern>, 1> patterns_;

    mutable fibers::mutex default_entity_mtx_;
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
    std::expected<entity_identifier, error_storage> const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const override;
};





}
