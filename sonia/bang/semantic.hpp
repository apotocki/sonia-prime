//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <bit>
#include <vector>

#include "sonia/variant.hpp"
#include "sonia/string.hpp"

#include "sonia/bang/utility/linked_list.hpp"

#include "semantic_fwd.hpp"
//#include "sonia/bang/entities/variable_entity.hpp"

namespace sonia::lang::bang {

// ======================================================================== types
#if 0
struct bang_object_t
{
    entity const* value;

    inline bool operator==(bang_object_t const& rhs) const { return value == rhs.value; };
    auto operator<=>(bang_object_t const& rhs) const;

    entity_identifier id() const;
};

template <typename T> struct bang_tuple
{
    std::vector<T> fields;
    std::vector<std::tuple<annotated_identifier, T>> named_fields;
    bool unpacked = false;

    inline bool operator==(bang_tuple const&) const = default;
    inline auto operator<=>(bang_tuple const& r) const
    {
        if (auto res = std::lexicographical_compare_three_way(
            fields.begin(), fields.end(), r.fields.begin(), r.fields.end(), variant_compare_three_way{});
            res != std::strong_ordering::equivalent) return res;
        return std::lexicographical_compare_three_way(
            named_fields.begin(), named_fields.end(), r.named_fields.begin(), r.named_fields.end(),
            [](auto const& ltpl, auto const& rtpl) {
                if (auto res = std::get<0>(ltpl).value <=> std::get<0>(rtpl).value; res != std::strong_ordering::equivalent) return res;
                return variant_compare_three_way{}(std::get<1>(ltpl), std::get<1>(ltpl));
            });
    }

    inline bool empty() const noexcept { return fields.empty() && named_fields.empty(); }
};
#endif

//template <typename T> using bang_fn = bang_fn_base<bang_tuple<T>, T>;

//struct bang_particular_bool_t
//{
//    bool value;
//    inline bool operator==(bang_particular_bool_t const&) const = default;
//    inline auto operator<=>(bang_particular_bool_t const&) const = default;
//};

template <typename T> struct bang_bunion
{
    T true_type;
    T false_type;

    inline bool operator==(bang_bunion const& rhs) const = default; // { return true_type == rhs.true_type && false_type == rhs.false_type; };
    inline auto operator<=>(bang_bunion const& rhs) const = default;
    /*
    {
        if (auto res = true_type <=> rhs.true_type; res != std::strong_ordering::equivalent) return res;
        return false_type <=> rhs.false_type;
    }
    */
};

#if 0
template <typename T> struct bang_union
{
    boost::container::small_vector<T, 8> other_members;
    // 1 reserved for initial state value -1
    enum class basic_type : uint16_t {
        nothing_e = 0, nil_e = 2, bool_e = 4, int_e = 8, float_e = 16, decimal_e = 32, string_e = 64,
        any_e = 128,
        max_type_e = 128
    };
    uint16_t basic_members = 0;
    
    struct member_iterator
    {
        using value_type = T;

        bang_union const* un_;
        T store_ = bang_any_t{};
        int state_;

        member_iterator() : un_ {nullptr} {}

        member_iterator(bang_union const& un, int state) : un_{ &un }, state_{ state }
        {
            next();
        }
        //  (int)basic_type::max_type_e
        void next()
        {
            while (un_) {
                if (state_ >= 0) {
                    if (++state_ >= un_->other_members.size()) {
                        un_ = nullptr;
                    }
                    return;
                } else {
                    state_ *= 2;
                }
                if (-state_ > (int)basic_type::max_type_e) {
                    state_ = 0;
                    if (un_->other_members.empty()) {
                        un_ = nullptr;
                    }
                    return;
                }
                if (un_->basic_members & (-state_)) {
                    switch((basic_type)(-state_)) {
                    case basic_type::nil_e:
                        store_ = bang_tuple<T>{};
                        return;
                    //case basic_type::bool_e:
                    //    store_ = bang_bool_t{};
                    //    return;
                    //case basic_type::int_e:
                    //    store_ = bang_int_t{};
                    //    return;
                    //case basic_type::float_e:
                    //    store_ = bang_float_t{};
                    //    return;
                    //case basic_type::decimal_e:
                    //    store_ = bang_decimal_t{};
                    //    return;
                    //case basic_type::string_e:
                    //    store_ = bang_string_t{};
                    //    return;
                    //case basic_type::any_e:
                    //    store_ = bang_any_t{};
                    //    return;
                    default:
                        THROW_INTERNAL_ERROR("unknown union member type");
                    }
                }
            }
        }

        T const& operator*() const
        {
            if (state_ < 0) {
                return store_;
            } else {
                return un_->other_members[state_];
            }
        }

        void operator++()
        {
            next();
        }

        inline bool operator==(member_iterator const& rhs) const { return !un_ ? !rhs.un_ : (un_ == rhs.un_ && state_ == rhs.state_); }
    };

    member_iterator begin() const { return member_iterator(*this, -1); }
    member_iterator end() const { return member_iterator(); }

    //inline auto to_tuple() const noexcept { std::tuple{ bool_member }; }

    inline bool operator==(bang_union const& r) const
    {
        return basic_members == r.basic_members && other_members == r.other_members;
    }

    inline auto operator<=>(bang_union const& r) const
    {
        if (auto res = basic_members <=> r.basic_members; res != std::strong_ordering::equivalent) return res;
        return std::lexicographical_compare_three_way(other_members.begin(), other_members.end(), r.other_members.begin(), r.other_members.end(), variant_compare_three_way{});
    }

    size_t size() const
    {
        return other_members.size() + std::popcount(basic_members);
    }

    struct appender_visitor : static_visitor<void>
    {
        bang_union & un_;
        explicit appender_visitor(bang_union & u) : un_{u} {}

        //inline void operator()(bang_bool_t) const { un_.basic_members |= (uint8_t)basic_type::bool_e; }
        //inline void operator()(bang_int_t) const { un_.basic_members |= (uint8_t)basic_type::int_e; }
        //inline void operator()(bang_float_t) const { un_.basic_members |= (uint8_t)basic_type::float_e; }
        //inline void operator()(bang_decimal_t) const { un_.basic_members |= (uint8_t)basic_type::decimal_e; }
        //inline void operator()(bang_string_t) const { un_.basic_members |= (uint8_t)basic_type::string_e; }
        //inline void operator()(bang_any_t) const { un_.basic_members |= (uint8_t)basic_type::any_e; }

        inline void operator()(bang_union u) const
        {
            for (auto const& m : u) { this->operator()(m); }
        }

        inline void operator()(bang_tuple<T> t) const
        {
            if (t.empty()) {
                un_.basic_members |= (uint8_t)basic_type::nil_e;
            } else {
                append(std::move(t));
            }
        }

        inline void operator()(T arg) const
        {
            apply_visitor(*this, std::move(arg));
        }

        template <typename TypeT>
        inline void operator()(TypeT arg) const
        {
            append(std::move(arg));
        }

        void append(T arg) const
        {
            auto it = std::lower_bound(un_.other_members.begin(), un_.other_members.end(), arg);
            if (it != un_.other_members.end() && *it == arg) return; // already exists
            un_.other_members.insert(it, std::move(arg));
        }
    };

    template <typename ArgT>
    inline void append(ArgT && m)
    {
        appender_visitor{ *this }(std::forward<ArgT>(m));
    }

    //inline bool has(bang_bool_t const&) const { return !!(basic_members & (uint16_t)basic_type::bool_e); }
    //inline bool has(bang_int_t const&) const { return !!(basic_members & (uint16_t)basic_type::int_e); }
    //inline bool has(bang_float_t const&) const { return !!(basic_members & (uint16_t)basic_type::float_e); }
    //inline bool has(bang_decimal_t const&) const { return !!(basic_members & (uint16_t)basic_type::decimal_e); }
    //inline bool has(bang_string_t const&) const { return !!(basic_members & (uint16_t)basic_type::string_e); }
    inline bool has(bang_tuple<T> const& t) const
    {
        if (t.empty()) return !!(basic_members & (uint16_t)basic_type::nil_e);
        auto it = std::lower_bound(other_members.begin(), other_members.end(), T{t});
        return it != other_members.end() && *it == t;
    }
    template <typename ArgT>
    bool has(T const& t)
    {
        auto it = std::lower_bound(other_members.begin(), other_members.end(), T{t});
        return it != other_members.end() && *it == t;
    }
};
#endif

//using bang_type_variant = make_recursive_variant<
//    bang_any_t, bang_bool_t, bang_int_t, bang_float_t, bang_decimal_t, bang_string_t, bang_object_t,
//    bang_fn<recursive_variant_>,
//    bang_vector<recursive_variant_>,
//    bang_array<recursive_variant_>,
//    bang_tuple<recursive_variant_>,
//    bang_union<recursive_variant_>,
//    bang_bunion<recursive_variant_>
//>::type;


#if 0
struct bang_type;
using bang_type_variant = variant<
    bang_any_t, bang_object_t,
    recursive_wrapper<bang_fn<bang_type>>,
    recursive_wrapper<bang_vector<bang_type>>,
    recursive_wrapper<bang_array<bang_type>>,
    recursive_wrapper<bang_tuple<bang_type>>,
    recursive_wrapper<bang_union<bang_type>>,
    recursive_wrapper<bang_bunion<bang_type>>
>;

struct bang_type : bang_type_variant
{
    template <typename T>
    requires(!std::is_same_v<bang_type, std::remove_cvref_t<T>>)
    bang_type(T && val) : bang_type_variant(std::forward<T>(val)) {}

    bang_type(bang_type const&) = default;
    bang_type(bang_type &&) = default;
    bang_type& operator=(bang_type const&) = default;
    bang_type& operator=(bang_type&&) = default;

    template <typename T>
    bang_type& operator=(T&& val) { bang_type_variant::operator= (std::forward<T>(val)); return *this; }

    template <typename T>
    inline bool operator==(T const& rhs) const { return bang_type_variant::operator==(bang_type_variant{rhs}); }
    inline bool operator==(bang_type const& rhs) const { return bang_type_variant::operator==(static_cast<bang_type_variant const&>(rhs)); }

    inline auto operator<=>(bang_type const& rhs) const { return variant_compare_three_way{}(*this, rhs); }

    template <typename T>
    T const* as() const { return get<T>(static_cast<bang_type_variant const*>(this)); }
};

//using bang_vector_t = bang_vector<bang_type>;
//using bang_array_t = bang_array<bang_type>;
//using bang_tuple_t = bang_tuple<bang_type>;
//using bang_union_t = bang_union<bang_type>;
//using bang_bunion_t = bang_bunion<bang_type>;
using bang_fn_t = bang_fn<bang_type>;

bang_type make_union_type(bang_type, bang_type const*);
bang_type operator|| (bang_type const& l, bang_type const& r);
bang_type operator- (bang_union_t const& l, bang_type const& r);
#endif
// ======================================================================== function

//class symbol
//{
//public:
//    explicit symbol(qname_identifier id) : id_{ id } {}
//
//private:
//    qname_identifier id_;
//    boost::container::small_vector<entity_identifier, 4> entities_;
//    boost::container::small_vector<identifier, 4> names_;
//
//    // entities_.size() >= names_.size();
//};

template <typename ConstraintT>
class fieldset
{
public:
    // to do: optimize
    struct named_field
    {
        annotated_identifier ename;
        optional<annotated_identifier> iname;
        ConstraintT constraint;
        parameter_constraint_modifier_t constraint_type;
    };

    struct positioned_field
    {
        optional<annotated_identifier> iname;
        ConstraintT constraint;
        parameter_constraint_modifier_t constraint_type;
    };

    fieldset() = default;

    template <typename ArgT>
    void set_nfields(ArgT&& arg) { nfields_ = std::forward<ArgT>(arg); }

    template <typename ArgT>
    void set_pfields(ArgT&& arg) { pfields_ = std::forward<ArgT>(arg); }

    span<const named_field> named_fields() const { return nfields_; }
    span<const positioned_field> positioned_fields() const { return pfields_; }

    named_field const* find_named_field(identifier name) const
    {
        auto it = std::lower_bound(nfields_.begin(), nfields_.end(), name,
            [](named_field const& l, identifier r) { return l.ename.value < r; });
        if (it != nfields_.end() && it->ename.value == name) return &*it;
        return nullptr;
    }

protected:
    std::vector<named_field> nfields_;
    std::vector<positioned_field> pfields_;
};





#if 0
struct function_signature
{
    bang_fn_t fn_type;
    //bang_type result_type;
    //bang_tuple_t parameters;
    //std::vector<std::tuple<annotated_identifier, bang_type>> named_parameters;
    //std::vector<bang_type> position_parameters;
    identifier mangled_id;

    size_t parameters_count() const { return fn_type.arg.fields.size() + fn_type.arg.named_fields.size(); }

    inline auto& position_parameters() noexcept { return fn_type.arg.fields; }
    inline auto const& position_parameters() const noexcept { return fn_type.arg.fields; }
    inline auto& named_parameters() noexcept { return fn_type.arg.named_fields; }
    inline auto const& named_parameters() const noexcept { return fn_type.arg.named_fields; }

    void setup(fn_compiler_context&, parameter_woa_list_t&);
    void normilize(fn_compiler_context&);
    //void build_symbol(unit&, symbol&);
    void build_mangled_id(unit&);

    //bang_type to_function_type() const { return bang_fn_t{}}
    //{
    //    bang_tuple_t argtpl;
    //    argtpl.fields = position_parameters;
    //    argtpl.named_fields = named_parameters;
    //    return bang_fn_t{ std::move(argtpl), }
    //    argtpl.named_fields.reserve(named_parameters.size());
    //    std::ranges::for_each(named_parameters, std::back_inserter(argtpl.named_fields),
    //        [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).id, std::get<1>(tpl) }; });
    //}
};
#endif


namespace semantic {

struct push_by_offset { size_t offset; }; // offset from the stack top
struct push_value { value_t value; };
struct push_local_variable
{
#ifdef SONIA_LANG_DEBUG
    identifier debug_name;
#endif
    intptr_t index;

    inline explicit push_local_variable(local_variable const& v) noexcept
        : index{ v.index }
    {
#ifdef SONIA_LANG_DEBUG
        debug_name = v.name.value;
#endif
    }
};

struct set_local_variable { intptr_t index; };
struct set_variable { extern_variable_entity const* entity; };
struct set_by_offset { size_t offset; }; // offset from the stack top
struct truncate_values {
    uint32_t count : 31;
    uint32_t keep_back : 1;

    truncate_values(size_t cnt, bool keepb) : count {static_cast<uint32_t>(cnt)}, keep_back{ keepb } {}
};

struct return_statement {};

struct invoke_function
{
    //qname_identifier varname;
    entity_identifier fn;
};

//enum class condition_type : uint8_t
//{
//    logic,
//    optionality
//};


//template <typename SemanticExpressionT>
//struct logic_tree_node
//{
//    std::vector<SemanticExpressionT> condition_expression;
//    bang_type expression_type = bang_tuple_t{};
//    shared_ptr<logic_tree_node> true_branch;
//    shared_ptr<logic_tree_node> false_branch;
//};

struct expression_entry;
using expression_span = linked_list_node_span<expression_entry>;

struct conditional_t
{
    //condition_type type;
    expression_span true_branch;
    expression_span false_branch;
    uint8_t true_branch_finished : 1;
    uint8_t false_branch_finished : 1;

    conditional_t() : true_branch_finished{ 0 }, false_branch_finished{ 0 } {}
};

struct not_empty_condition_t
{
    expression_span branch;
};

struct loop_scope_t
{
    expression_span branch;
    expression_span continue_branch;
};

struct loop_continuer {};
struct loop_breaker {};

using expression = variant<
    empty_t, // no op
    push_value, push_local_variable, push_by_offset, truncate_values,
    set_local_variable, set_variable, set_by_offset, invoke_function, return_statement, loop_breaker, loop_continuer,
    expression_span,
    conditional_t,
    not_empty_condition_t,
    loop_scope_t
    //logic_tree_node<recursive_variant_>
>;

using expression_entry_type = linked_list_node<expression>;
struct expression_entry : expression_entry_type { using expression_entry_type::expression_entry_type; };

//using conditional_t = conditional<expression>;
//using not_empty_condition_t = not_empty_condition<expression>;
//using loop_scope_t = loop_scope<expression>;
//using logic_tree_node_t = logic_tree_node<expression_t>;

using expression_list_t = linked_list<expression>;
using managed_expression_list = managed_linked_list<expression, unit>;

}

//using semantic_expression_pair = std::pair<semantic::expression_t, bang_type>;

}


///#include "entities/type_entity.hpp"

//namespace sonia::lang::bang {
//
//inline entity_identifier bang_object_t::id() const
//{
//    return value->id();
//}
//
//inline auto bang_object_t::operator<=>(bang_object_t const& rhs) const
//{
//    return value->id() <=> rhs.value->id();
//};
//
//}
