//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <bit>
#include <vector>
#include "sonia/variant.hpp"
#include "sonia/string.hpp"

#include "semantic_fwd.hpp"

namespace sonia::lang::beng {

// ======================================================================== types
struct beng_object_t
{
    entity const* value;

    inline bool operator==(beng_object_t const& rhs) const { return value == rhs.value; };
    auto operator<=>(beng_object_t const& rhs) const;

    qname_view name() const;
};

template <typename T> struct beng_tuple
{
    std::vector<T> fields;
    std::vector<std::tuple<annotated_identifier, T>> named_fields;
    bool unpacked = false;

    inline bool operator==(beng_tuple const&) const = default;
    inline auto operator<=>(beng_tuple const& r) const
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

template <typename T> using beng_fn = beng_fn_base<beng_tuple<T>, T>;

//struct beng_particular_bool_t
//{
//    bool value;
//    inline bool operator==(beng_particular_bool_t const&) const = default;
//    inline auto operator<=>(beng_particular_bool_t const&) const = default;
//};

template <typename T> struct beng_union
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

        beng_union const* un_;
        T store_;
        int state_;

        member_iterator() : un_ {nullptr} {}

        member_iterator(beng_union const& un, int state) : un_{ &un }, state_{ state }
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
                        store_ = beng_tuple<T>{};
                        return;
                    case basic_type::bool_e:
                        store_ = beng_bool_t{};
                        return;
                    case basic_type::int_e:
                        store_ = beng_int_t{};
                        return;
                    case basic_type::float_e:
                        store_ = beng_float_t{};
                        return;
                    case basic_type::decimal_e:
                        store_ = beng_decimal_t{};
                        return;
                    case basic_type::string_e:
                        store_ = beng_string_t{};
                        return;
                    case basic_type::any_e:
                        store_ = beng_any_t{};
                        return;
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

    inline bool operator==(beng_union const& r) const
    {
        return basic_members == r.basic_members && other_members == r.other_members;
    }

    inline auto operator<=>(beng_union const& r) const
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
        beng_union & un_;
        explicit appender_visitor(beng_union & u) : un_{u} {}

        inline void operator()(beng_bool_t) const { un_.basic_members |= (uint8_t)basic_type::bool_e; }
        inline void operator()(beng_int_t) const { un_.basic_members |= (uint8_t)basic_type::int_e; }
        inline void operator()(beng_float_t) const { un_.basic_members |= (uint8_t)basic_type::float_e; }
        inline void operator()(beng_decimal_t) const { un_.basic_members |= (uint8_t)basic_type::decimal_e; }
        inline void operator()(beng_string_t) const { un_.basic_members |= (uint8_t)basic_type::string_e; }
        inline void operator()(beng_any_t) const { un_.basic_members |= (uint8_t)basic_type::any_e; }

        inline void operator()(beng_union u) const
        {
            for (auto const& m : u) { this->operator()(m); }
        }

        inline void operator()(beng_tuple<T> t) const
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
};

using beng_type = make_recursive_variant<
    beng_any_t, beng_bool_t, beng_int_t, beng_float_t, beng_decimal_t, beng_string_t, beng_object_t,
    beng_fn<recursive_variant_>,
    beng_vector<recursive_variant_>,
    beng_array<recursive_variant_>,
    beng_tuple<recursive_variant_>,
    beng_union<recursive_variant_>
>::type;

using beng_vector_t = beng_vector<beng_type>;
using beng_array_t = beng_array<beng_type>;
using beng_tuple_t = beng_tuple<beng_type>;
using beng_union_t = beng_union<beng_type>;
using beng_fn_t = beng_fn<beng_type>;

beng_type make_union_type(beng_type, beng_type const*);

// ======================================================================== function

struct function_signature
{
    beng_fn_t fn_type;
    //beng_type result_type;
    //beng_tuple_t parameters;
    //std::vector<std::tuple<annotated_identifier, beng_type>> named_parameters;
    //std::vector<beng_type> position_parameters;
    identifier mangled_id;

    size_t parameters_count() const { return fn_type.arg.fields.size() + fn_type.arg.named_fields.size(); }

    inline auto& position_parameters() noexcept { return fn_type.arg.fields; }
    inline auto const& position_parameters() const noexcept { return fn_type.arg.fields; }
    inline auto& named_parameters() noexcept { return fn_type.arg.named_fields; }
    inline auto const& named_parameters() const noexcept { return fn_type.arg.named_fields; }

    void setup(fn_compiler_context&, parameter_woa_list_t&);
    void normilize(fn_compiler_context&);
    void build_mangled_id(unit&);

    //beng_type to_function_type() const { return beng_fn_t{}}
    //{
    //    beng_tuple_t argtpl;
    //    argtpl.fields = position_parameters;
    //    argtpl.named_fields = named_parameters;
    //    return beng_fn_t{ std::move(argtpl), }
    //    argtpl.named_fields.reserve(named_parameters.size());
    //    std::ranges::for_each(named_parameters, std::back_inserter(argtpl.named_fields),
    //        [](auto const& tpl) { return std::tuple{ std::get<0>(tpl).id, std::get<1>(tpl) }; });
    //}
};



namespace semantic {
struct push_by_offset { size_t offset; }; // offset from the stack top
struct push_variable { variable_entity const* entity; };
struct push_value { value_t value; };
struct set_variable { variable_entity const* entity; };
struct set_by_offset { size_t offset; }; // offset from the stack top
struct truncate_values {
    uint32_t count : 31;
    uint32_t keep_back : 1;

    truncate_values(size_t cnt, bool keepb) : count {static_cast<uint32_t>(cnt)}, keep_back{ keepb } {}
};

struct return_statement {};

struct invoke_function
{
    variant<qname, qname_view> varname;
    qname_view function_entity_name() const
    {
        if (qname_view const* qv = get<qname_view>(&varname); qv) return *qv;
        return get<qname>(varname);
    }
};

enum class condition_type : uint8_t
{
    logic,
    optionality
};

template <typename SemanticExpressionT>
struct conditional
{
    condition_type type;
    std::vector<SemanticExpressionT> true_branch;
    std::vector<SemanticExpressionT> false_branch;
};

template <typename SemanticExpressionT>
struct logic_tree_node
{
    std::vector<SemanticExpressionT> condition_expression;
    beng_type expression_type = beng_tuple_t{};
    shared_ptr<logic_tree_node> true_branch;
    shared_ptr<logic_tree_node> false_branch;
};

// make_recursive_variant<
using expression_type = make_recursive_variant<
    empty_t, // no op
    push_variable, push_value, push_by_offset, truncate_values,
    set_variable, set_by_offset, invoke_function, return_statement,
    std::vector<recursive_variant_>,
    conditional<recursive_variant_>,
    logic_tree_node<recursive_variant_>
>::type;

using logic_tree_node_t = logic_tree_node<expression_type>;

}

using semantic_expression_pair = std::pair<semantic::expression_type, beng_type>;



class function_scope_type
{

};

/*
class function_t
{
public:
    virtual ~function_t() = default;
};

class implemented_function : public function_t
{
public:
    qname name;
    std::vector<semantic::expression_type> body;
    bool is_inline = false;
};
*/

}


///#include "entities/type_entity.hpp"

namespace sonia::lang::beng {

inline qname_view beng_object_t::name() const
{
    return value->name();
}

inline auto beng_object_t::operator<=>(beng_object_t const& rhs) const
{
    return value->name() <=> rhs.value->name();
};

}
