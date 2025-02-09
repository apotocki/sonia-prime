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

class virtual_stack_machine;

namespace vm { class context; }

class external_fn_pattern;

enum class builtin_id : size_t
{
    type = 0, // type:
    to, // to:
    self, // self:
    property, // property:
    object, // object:
    numargs, // $$
    init, // init
    eof_builtin_id_value
};

enum class builtin_qnid : size_t
{
    fn = 0,
    ellipsis, tuple, identifier, qname, object, string, decimal, integer, boolean, any,
    metaobject, typeof,
    make_tuple, new_, eq, ne, plus, negate, implicit_cast, get, set,
    eof_builtin_qnids_value
};

enum class builtin_eid : size_t
{
    typename_ = 0, void_, any, identifier, qname, object, string, decimal, integer, boolean, metaobject,
    arrayify, // builtin ::arrayify(...)->array
    array_at, // builtin ::array_at(array, index)-> elementT
    eof_builtin_eid_value
};

class unit
{
    using identifier_builder_t = identifier_builder<identifier>;
    //using entity_identifier_builder_t = identifier_builder<entity_identifier>;

    using slregistry_t = string_literal_registry<identifier, small_string>;
    //using qname_registry_t = qname_registry<identifier>;
    using functional_registry_t = functional_registry<functional>;

    using piregistry_t = parameterized_identifier_registry<identifier>;
    using eregistry_t = entity_registry<entity>;
    

    // pools first
    object_pool<semantic::expression_list_t::entry_type> semantic_expression_list_entry_pool_;
    object_pool<statement_entry> statements_entry_pool_;

    identifier_builder_t identifier_builder_;
    //entity_identifier_builder_t entity_identifier_builder_;
    slregistry_t slregistry_;
    //qname_registry_t qname_registry_;
    functional_registry_t functional_registry_;

    piregistry_t piregistry_;
    eregistry_t eregistry_;

    // ast store
    managed_statement_list ast_;

    // semantic
    semantic::expression_span root_expressions_;

    // vm builtin id -> qname
    std::vector<qname_identifier> vm_builtins_;

    size_t fn_identifier_counter_;

    std::vector<small_string> strings_;

    compiler_task_tracer task_tracer_;



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
    
    ////// entity registry routines
    void eregistry_insert(shared_ptr<entity>);
    entity const& eregistry_get(entity_identifier) const;
    template <typename FactoryT>
    inline entity & eregistry_find_or_create(entity const& sample, FactoryT const& factory)
    {
#ifdef SONIA_LANG_DEBUG
        return eregistry_.find_or_create(sample, [this, &factory]() {
            shared_ptr<entity> e = factory();
            std::ostringstream ss;
            e->print_to(ss, *this);
            e->debug_name = ss.str();
            return e;
        });
#else
        return eregistry_.find_or_create(sample, factory);
#endif
    }

    template <typename FtorT>
    inline void eregistry_traverse(FtorT const& ftor) const
    {
        eregistry_.traverse(ftor);
    }

    inline compiler_task_tracer & task_tracer() noexcept { return task_tracer_; }

    // global constants rotine
    //size_t allocate_constant_index();

    virtual_stack_machine& bvm() { return *bvm_; }

    inline entity_identifier get(builtin_eid bi) const noexcept { return builtin_eids_[(size_t)bi]; }
    inline qname_identifier get(builtin_qnid bi) const noexcept { return builtin_qnids_[(size_t)bi]; }
    inline functional const& fget(builtin_qnid bi) const noexcept { return functional_registry_.resolve(builtin_qnids_[(size_t)bi]); }
    inline identifier get(builtin_id bi) const noexcept { return builtin_ids_[(size_t)bi]; }

    functional& resolve_functional(qname_identifier);

    //void push_entity(shared_ptr<entity>);

    //variable_entity& new_variable(qname_view, lex::resource_location const&, entity_identifier type, variable_entity::kind);

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

    //std::string print(bang_preliminary_type const& tp) const;
    
    std::string print(syntax_expression_t const&) const;
    std::string print(semantic::expression const&) const;
    std::string print(semantic::expression_list_t const& elist) const;

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

    void push_back_expression(semantic::expression_list_t&, semantic::expression&&);
    void release(semantic::expression_list_t::entry_type&&);

    statement_entry& acquire(statement&& st);
    //void push_back_statement(statement_list&, statement&&);
    void release(statement_entry_type&&);

    statement_span push_ast(fs::path const&, managed_statement_list&&);

protected:
    void setup_type(string_view type_name, qname_identifier&, entity_identifier&);

    std::vector<char> read_file(fs::path const& rpath);

    template <typename OutputIteratorT, typename UndefinedFT>
    OutputIteratorT identifier_printer(identifier const&, string_view prefix, OutputIteratorT, UndefinedFT const&) const;

    template <typename OutputIteratorT, typename UndefinedFT>
    OutputIteratorT name_printer(qname_view const&, OutputIteratorT, UndefinedFT const&) const;


    std::pair<functional*, fn_pure> parse_extern_fn(string_view signature);

    template <std::derived_from<external_fn_pattern> PT>
    void set_extern(string_view sign, void(*pfn)(vm::context&));

    template <std::derived_from<functional::pattern> PT>
    void set_const_extern(string_view sig);

    entity_identifier set_builtin_extern(string_view name, void(*pfn)(vm::context&));

private:
    //identifier fn_result_identifier_; // ->
    std::array<identifier, (size_t)builtin_id::eof_builtin_id_value> builtin_ids_;
    std::array<qname_identifier, (size_t)builtin_qnid::eof_builtin_qnids_value> builtin_qnids_;
    std::array<entity_identifier, (size_t)builtin_eid::eof_builtin_eid_value> builtin_eids_;

    //entity_identifier typename_entity_identifier_;
    //entity_identifier void_entity_identifier_;
    //entity_identifier any_entity_identifier_;
    //entity_identifier identifier_entity_identifier_;
    //entity_identifier object_entity_identifier_;
    //entity_identifier string_entity_identifier_;
    //entity_identifier decimal_entity_identifier_;
    //entity_identifier integer_entity_identifier_;
    //entity_identifier bool_entity_identifier_;
    //entity_identifier metaobject_entity_identifier_;

    //entity_identifier arrayify_entity_identifier_; // builtin ::arrayify(...)->array
    //entity_identifier array_at_entity_identifier_; // builtin ::array_at(array, index)-> elementT

    // entities registry:
    //qname -> entity

    std::unique_ptr<virtual_stack_machine> bvm_;
    std::vector<fs::path> additional_paths_;

    //functional_entity* integer_entity_;
    functional_entity* decimal_entity_;

    function<void(string_view)> cout_writer_;
};

}
