//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <fstream>

#include "sonia/filesystem.hpp"
#include "sonia/exceptions.hpp"

#include "sonia/utility/lang/utility.hpp"
#include "sonia/utility/lang/identifier_builder.hpp"
#include "sonia/utility/lang/string_literal_registry.hpp"
#include "sonia/utility/lang/parameterized_identifier_registry.hpp"
#include "sonia/utility/lang/qname_registry.hpp"
#include "sonia/utility/lang/entity_registry.hpp"
#include "sonia/utility/lang/compiler.hpp"

#include "boost/conversion/unicode/utf.hpp"
#include "boost/conversion/push_iterator.hpp"

#include "semantic.hpp"
#include "entities/variable_entity.hpp"
#include "entities/functional_entity.hpp"
#include "entities/functional.hpp"
#include "entities/functional_registry.hpp"

namespace sonia::lang::bang {

/*
struct variable_descriptor
{
    identifier name;
    bang_generic_type type;
    uint32_t index : 31;
    uint32_t is_const : 1;

    explicit variable_descriptor(size_t idx) : index { static_cast<uint32_t>(idx) } {}

    friend inline bool operator==(variable_descriptor const& l, variable_descriptor const& r) noexcept { return l.name == r.name; }
    friend inline auto operator<=>(variable_descriptor const& l, variable_descriptor const& r) noexcept { return l.name <=> r.name; }
    friend inline size_t hash_value(variable_descriptor const& v) noexcept { return hash_value(v.name); }
};

struct variable_equal_to
{
    inline bool operator()(variable_descriptor const& l, identifier const& r) const noexcept { return l.name == r; }
    inline bool operator()(identifier const& l, variable_descriptor const& r) const noexcept { return l == r.name; }
};
*/

class virtual_stack_machine;

namespace vm { class context; }

class unit
{
    using identifier_builder_t = identifier_builder<identifier>;
    //using entity_identifier_builder_t = identifier_builder<entity_identifier>;

    using slregistry_t = string_literal_registry<identifier, small_string>;
    //using qname_registry_t = qname_registry<identifier>;
    using functional_registry_t = functional_registry<functional>;

    using piregistry_t = parameterized_identifier_registry<identifier>;
    using eregistry_t = entity_registry<entity>;
    

    identifier_builder_t identifier_builder_;
    //entity_identifier_builder_t entity_identifier_builder_;
    slregistry_t slregistry_;
    //qname_registry_t qname_registry_;
    functional_registry_t functional_registry_;

    piregistry_t piregistry_;
    eregistry_t eregistry_;

    // semantic
    std::vector<semantic::expression_t> root_expressions_;

    std::vector<qname_identifier> builtins_;

    size_t fn_identifier_counter_;

    std::vector<small_string> strings_;

    compiler_task_tracer task_tracer_;

    object_pool<semantic::expression_list_t::entry_type> semantic_expression_list_entry_pool_;

public:
    enum class builtin_type
    {
        //integer, 
        decimal
    };

    enum class builtin_fn
    {
        arrayify = 0, unpack, weak_create, weak_lock,
        extern_object_create,
        extern_object_set_property,
        extern_object_get_property,
        extern_function_call,
        tostring, negate,
        eof_builtin_type
    };

    unit();
    ~unit();

    void set_efn(size_t idx, qname_identifier);

    inline void set_efn(builtin_fn bi, qname_identifier fnq)
    {
        set_efn((size_t)bi, std::move(fnq));
    }

    //semantic::invoke_function get_builtin_function(builtin_fn bi) const
    //{
    //    return semantic::invoke_function{ builtins_.at((size_t)bi) };
    //}

    identifier new_identifier();
    identifier make_identifier(string_view);

    qname_identifier new_qname_identifier();
    qname_identifier make_qname_identifier(string_view);
    qname_identifier get_function_entity_identifier(string_view signature);

    slregistry_t& slregistry() { return slregistry_; }
    //qname_registry_t& qnregistry() { return qname_registry_; }
    functional_registry_t& fregistry() { return functional_registry_; }

    piregistry_t& piregistry() { return piregistry_; }
    eregistry_t& eregistry() { return eregistry_; }
    
    inline compiler_task_tracer & task_tracer() noexcept { return task_tracer_; }

    // global constants rotine
    //size_t allocate_constant_index();

    virtual_stack_machine& bvm() { return *bvm_; }

    inline entity_identifier get_typename_entity_identifier() const noexcept { return typename_entity_identifier_; }
    inline entity_identifier get_void_entity_identifier() const noexcept { return void_entity_identifier_; }
    inline entity_identifier get_any_entity_identifier() const noexcept { return any_entity_identifier_; }
    inline entity_identifier get_string_entity_identifier() const noexcept { return string_entity_identifier_; }
    inline entity_identifier get_decimal_entity_identifier() const noexcept { return decimal_entity_identifier_; }
    inline entity_identifier get_integer_entity_identifier() const noexcept { return integer_entity_identifier_; }
    inline entity_identifier get_bool_entity_identifier() const noexcept { return bool_entity_identifier_; }
    
    inline qname_identifier get_fn_qname_identifier() const noexcept { return fn_qname_identifier_; }
    inline qname_identifier get_ellipsis_qname_identifier() const noexcept { return ellipsis_qname_identifier_; }
    inline qname_identifier get_tuple_qname_identifier() const noexcept { return tuple_qname_identifier_; }
    inline qname_identifier get_string_qname_identifier() const noexcept { return string_qname_identifier_; }
    inline qname_identifier get_decimal_qname_identifier() const noexcept { return decimal_qname_identifier_; }
    inline qname_identifier get_integer_qname_identifier() const noexcept { return integer_qname_identifier_; }
    inline qname_identifier get_bool_qname_identifier() const noexcept { return bool_qname_identifier_; }
    inline qname_identifier get_any_qname_identifier() const noexcept { return any_qname_identifier_; }

    inline qname_identifier get_eq_qname_identifier() const noexcept { return eq_qname_identifier_; }
    inline qname_identifier get_ne_qname_identifier() const noexcept { return ne_qname_identifier_; }
    inline qname_identifier get_plus_qname_identifier() const noexcept { return plus_qname_identifier_; }
    inline qname_identifier get_negate_qname_identifier() const noexcept { return negate_qname_identifier_; }
    inline qname_identifier get_implicit_cast_qname_identifier() const noexcept { return implicit_cast_qname_identifier_; }

    //inline identifier get_fn_result_identifier() const noexcept { return fn_result_identifier_; }
    inline identifier get_to_parameter_identifier() const noexcept { return to_parameter_identifier_; }

    functional& resolve_functional(qname_identifier);

    //void push_entity(shared_ptr<entity>);

    void set_extern(string_view sign, void(*pfn)(vm::context&));

    //void put_function(shared_ptr<function_t> f)
    //{
    //    functions_.emplace_back(std::move(f));
    //}

    ////////////////////// builtins
    //qname const& builtin(builtin_type bit) const
    //{
    //    return builtins_[(int)bit];
    //}


    ///
    void append_path(fs::path p)
    {
        additional_paths_.emplace_back(std::move(p));
    }

    std::vector<char> get_file_content(fs::path const& rpath, fs::path const* context = nullptr);
    
    functional& resolve_functional(qname_view);

    std::string print(identifier const& id) const;
    std::string print(entity_identifier const& id) const;
    std::string print(entity const&) const;
    std::string print(entity_signature const&) const;

    std::string print(qname_view q) const;
    std::string print(qname const& q) const
    {
        return print((qname_view)q);
    }
    std::string print(qname_identifier) const;

    std::string print(small_u32string const&, bool in_quotes = false) const;

    std::string print(lex::resource_location const&) const;

    std::string print(bang_preliminary_type const& tp) const;
    
    std::string print(bang_type const& tp) const;
    
    std::string print(syntax_expression_t const&) const;
    std::string print(semantic::expression_t const&) const;

    std::string print(error const&) const;

    std::string describe_efn(size_t index) const;

    small_string as_string(identifier const& id) const;
    small_string as_string(entity_identifier const& id) const;
    small_string as_string(qname_view name) const;
    small_string as_string(qname_identifier name) const;

    //small_u32string as_u32string(identifier const& id) const;
    
    //functional_entity& get_functional_entity(binary_operator_type);

    void set_cout_writer(function<void(string_view)> writer)
    {
        cout_writer_ = std::move(writer);
    }

    void write_cout(string_view str)
    {
        if (cout_writer_) cout_writer_(str);
        else {
            GLOBAL_LOG_INFO() << str;
        }
    }

    void push_back_expression(semantic::expression_list_t&, semantic::expression_t&&);
    void release(semantic::expression_list_t::entry_type&&);

protected:
    void setup_type(string_view type_name, qname_identifier&, entity_identifier&);

    std::vector<char> read_file(fs::path const& rpath);

    template <typename OutputIteratorT, typename UndefinedFT>
    OutputIteratorT identifier_printer(identifier const&, string_view prefix, OutputIteratorT, UndefinedFT const&) const;

    template <typename OutputIteratorT, typename UndefinedFT>
    OutputIteratorT name_printer(qname_view const&, OutputIteratorT, UndefinedFT const&) const;

private:
    //identifier fn_result_identifier_; // ->
    identifier to_parameter_identifier_; // to:

    qname_identifier fn_qname_identifier_; // :: __fn -- the marker of a function type
    qname_identifier ellipsis_qname_identifier_; // :: ...
    qname_identifier tuple_qname_identifier_; //(...)
    qname_identifier string_qname_identifier_; // :: string
    qname_identifier decimal_qname_identifier_; // :: decimal
    qname_identifier integer_qname_identifier_; // :: integer
    qname_identifier bool_qname_identifier_; // :: bool
    qname_identifier any_qname_identifier_; // :: any
    qname_identifier eq_qname_identifier_; // :: ==
    qname_identifier ne_qname_identifier_; // :: !=
    qname_identifier plus_qname_identifier_; // binary +
    qname_identifier negate_qname_identifier_; // :: !
    qname_identifier implicit_cast_qname_identifier_;

    entity_identifier void_entity_identifier_;
    entity_identifier any_entity_identifier_;
    entity_identifier typename_entity_identifier_;
    entity_identifier string_entity_identifier_;
    entity_identifier decimal_entity_identifier_;
    entity_identifier integer_entity_identifier_;
    entity_identifier bool_entity_identifier_;

    // entities registry:
    //qname -> entity

    std::unique_ptr<virtual_stack_machine> bvm_;
    std::vector<fs::path> additional_paths_;

    //functional_entity* integer_entity_;
    functional_entity* decimal_entity_;

    function<void(string_view)> cout_writer_;
};

}
