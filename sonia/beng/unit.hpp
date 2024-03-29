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
#include "sonia/utility/lang/entity_registry.hpp"

#include "boost/conversion/unicode/utf.hpp"
#include "boost/conversion/push_iterator.hpp"

#include "semantic.hpp"
#include "entities/variable_entity.hpp"
#include "entities/functional_entity.hpp"

#include "builtins.hpp"

#include "vm/beng_vm.hpp"

//#include "functional_entity.hpp"

namespace sonia::lang::beng {

/*
struct variable_descriptor
{
    identifier name;
    beng_generic_type type;
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

class unit
{
    using identifier_builder_t = identifier_builder<identifier>;

    using slregistry_t = string_literal_registry<identifier, small_string>;
    using piregistry_t = parameterized_identifier_registry<identifier>;
    using eregistry_t = entity_registry<entity>;

    identifier_builder_t identifier_builder_;
    slregistry_t slregistry_;
    piregistry_t piregistry_;
    eregistry_t eregistry_;

    // semantic
    
    //std::vector<shared_ptr<function_t>> functions_;
    //shared_ptr<function_t> main_function_;
    std::vector<semantic_expression_type> root_expressions_;

    //shared_ptr<entity> arrayify_entity_;
    //shared_ptr<entity> print_entity_;

    std::vector<qname_view> builtins_;
    //qname arrayify_entity_name_;
    //qname print_string_name_;

    virtual_stack_machine bvm_;

    size_t fn_identifier_counter_;

    std::vector<small_string> strings_;

public:
    enum class builtin_fn
    {
        arrayify = 0,
        tostring,
        eof_builtin_type
    };

    unit();

    void set_efn(size_t idx, qname_view fnq);

    inline void set_efn(builtin_fn bi, qname_view fnq)
    {
        set_efn((size_t)bi, std::move(fnq));
    }

    semantic::invoke_function get_builtin_function(builtin_fn bi) const
    {
        return semantic::invoke_function(builtins_.at((size_t)bi));
    }

    identifier new_identifier() { return identifier_builder_(); }

    slregistry_t& slregistry() { return slregistry_; }
    piregistry_t& piregistry() { return piregistry_; }
    eregistry_t& eregistry() { return eregistry_; }

    virtual_stack_machine& bvm() { return bvm_; }

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
    
    std::string print(identifier const& id) const;
    
    std::string print(qname const& q) const
    {
        return print((qname_view)q);
    }

    std::string print(qname_view q) const;
    
    std::string print(beng_preliminary_type const& tp) const;
    
    std::string print(beng_type const& tp) const;
    
    string_view as_string(identifier const& id) const;
    
    small_u32string as_u32string(identifier const& id) const;

    small_u32string as_u32string(qname_view name) const;
    
protected:
    std::vector<char> read_file(fs::path const& rpath);

private:
    // entities registry:
    //qname -> entity

    std::vector<fs::path> additional_paths_;
};

}
