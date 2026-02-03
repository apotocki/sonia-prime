//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "advxmlbuilder.hpp"
#include "detail/type_parser.hpp"

namespace sonia::xmlbuilder {

using namespace sonia::xmlbuilder::detail;

class x_generic_type_factory
{
    advanced_external_builder const& aeb;

public:
    using type = x_generic_type;

    explicit x_generic_type_factory(advanced_external_builder const& eb) : aeb{eb} {}

    type create(string_view name) const
    {
        if (name == "bool"sv) return x_bool{};
        if (name == "int"sv) return x_int{};
        if (name == "float"sv) return x_float{};
        if (name == "string"sv) return x_string{};
        if (name == "{}"sv) return x_action{};
        if (auto it = aeb.types_.find(name, hasher{}, std::equal_to{}); it != aeb.types_.end()) {
            return x_object{ *it };
        }
        throw exception("unknown type: '%1%'"_fmt % name);
    }
};

/*
class document_attribute_resolver : public attribute_resolver
{
    advanced_external_builder & bld_;
public:
    explicit document_attribute_resolver(advanced_external_builder & b) : bld_ { b } {}

    std::tuple<blob_result, std::string, func_type> operator()(string_view element, string_view attr_name, string_view attr_value) const override
    {
        if (element != "document"sv) [[unlikely]] {
            throw exception("<document> must be the root element instead of '%1%'"_fmt % element);
        }

        throw exception("unexpected <document> attribute '%1%'"_fmt % attr_name);
    }
};
*/

class x_code_context
    : public external_builder
{
    advanced_external_builder& rb; // root builder

public:
    explicit x_code_context(advanced_external_builder& aeb) : rb{ aeb } {}

    void append_element(span<element> parents, element& e) override
    {
        throw exception("unexpected tag <%1%> in <x:code>"_fmt % e.name);
    }

    void close_element(span<element> parents, element& e) override
    {
        small_string* type = nullptr, * default_value = nullptr;

        for (auto& pair : e.attrs) {
            throw exception("unexpected attribute <%1%> in x-body context"_fmt % pair.first);
        }

        rb.append_code(e.text);

        rb.contexts_.pop_back();
    }
};

class x_generic_parameter_context : public external_builder
{
protected:
    advanced_external_builder& rb; // root builder

    explicit x_generic_parameter_context(advanced_external_builder& aeb) : rb{ aeb } {}

    virtual string_view ctx_name() const = 0;

    void append_element(span<element> parents, element& e) override
    {
        throw exception("unexpected tag <%1%> in %2% context"_fmt % e.name % ctx_name());
    }

    detail::x_generic_type parse_type(string_view typestr) const
    {
        using type_grammar = detail::type_parser_grammar<string_view::const_iterator, x_generic_type_factory>;
        type_grammar tg{rb};
        detail::x_generic_type tp;

        using boost::spirit::ascii::space;
        auto iter = typestr.begin();
        auto end = typestr.end();
        bool r = phrase_parse(iter, end, tg, space, tp);

        if (!r || iter != end) [[unlikely]] {
            //auto some = iter + int(end - iter);
            //std::string context(iter, (some > end) ? end : some);
            throw exception("wrong type declaration, `%1%`"_fmt % typestr);
        }
        return tp;
    }
};

class x_parameter_context 
    : public x_generic_parameter_context
    , public x_parameter
{
    x_parameter_adopter& adopter;

public:
    x_parameter_context(advanced_external_builder& aeb, x_parameter_adopter& a) 
        : x_generic_parameter_context{ aeb }
        , adopter{a}
    {}

    string_view ctx_name() const override { return "x-parameter"sv; }
    
    void close_element(span<element> parents, element& e) override
    {
        small_string *name = nullptr, *default_value = nullptr;
        
        for (auto& pair : e.attrs) {
            if (pair.first == "name"sv) {
                name = &pair.second;
            } else if (pair.first == "type"sv) {
                type.emplace_back(std::move(parse_type(pair.second)));
            } else if (pair.first == "default"sv) {
                default_value = &pair.second;
            } else {
                throw exception("unexpected attribute <%1%> in x-parameter context"_fmt % pair.first);
            }
        }
        if (!name) [[unlikely]] {
            throw exception("no name attribute in x-parameter context");
        }
        if (type.empty()) [[unlikely]] {
            throw exception("no type attribute in x-parameter context");
        }
        this->name = std::move(*name);

        adopter.append_parameter(*this);

        rb.contexts_.pop_back();
    }
};

class x_body_context 
    : public x_generic_parameter_context
    , public x_parameter
{
    x_body_adopter& adopter;

public:
    x_body_context(advanced_external_builder& aeb, x_body_adopter& a)
        : x_generic_parameter_context{ aeb }
        , adopter{ a }
    {}

    string_view ctx_name() const override { return "x-body"sv; }
    
    void close_element(span<element> parents, element& e) override
    {
        small_string *type = nullptr, *default_value = nullptr;
        
        for (auto& pair : e.attrs) {
            if (pair.first == "type"sv) {
                type = &pair.second;
            } else if (pair.first == "default"sv) {
                default_value = &pair.second;
            } else {
                throw exception("unexpected attribute <%1%> in x-body context"_fmt % pair.first);
            }
        }
        if (!type) [[unlikely]] {
            throw exception("no type attribute in x-body context");
        }

        auto tp = parse_type(*type);
        adopter.set_body_parameter(*this);

        rb.contexts_.pop_back();
    }
};

class x_type_context 
    : public external_builder
    , public x_type
{
    advanced_external_builder& rb; // root builder

public:
    explicit x_type_context(advanced_external_builder& aeb) : rb{aeb} {}

    void append_element(span<element> parents, element& e) override
    {
        string_view ens = rb.query_ns(e, parents);
        if (ens == X_NS) {
            string_view lname = rb.local_name(e);
            if (lname == "parameter"sv) {
                rb.contexts_.emplace_back(make_shared<x_parameter_context>(rb, *this));
                return;
            } else if (lname == "body"sv) {
                rb.contexts_.emplace_back(make_shared<x_body_context>(rb, *this));
                return;
            }
        }
        throw exception("unexpected tag <%1%> in x-type context"_fmt % e.name);
    }

    void close_element(span<element>, element& e) override
    {
        small_string *name = nullptr, *extends = nullptr;
        
        for (auto& pair : e.attrs) {
            if (pair.first == "name"sv) {
                name = &pair.second;
            } else if (pair.first == "extends"sv) {
                extends = &pair.second;
            } else {
                throw exception("unexpected attribute <%1%> in x-type context"_fmt % pair.first);
            }
        }
        if (!name) [[unlikely]] {
            throw exception("no name attribute in x-type context");
        }
        this->name = std::move(*name);
        rb.append_type(*this, extends);
        rb.contexts_.pop_back();
    }
};

class x_type_instance_context : public external_builder
{
    advanced_external_builder& rb; // root builder
    x_type const& xt_;

public:
    explicit x_type_instance_context(advanced_external_builder& aeb, x_type const& xt) : rb{ aeb }, xt_{xt} {}
    
    void append_element(span<element> parents, element& e) override
    {
        string_view ens = rb.query_ns(e, parents);
        string_view lname = rb.local_name(e);
        if (ens == TARGET_NS) {
            if (e.id.empty()) {
                e.id = rb.generate_id();
            }
            // to do: id may be expression
            /*
            if (x_type const* pobj = rb.append_vm_object_creation_code(lname, e.id); pobj) {

            } else if (lname == "if"sv || lname == "for"sv || lname == "dynamic"sv || lname == "repeat"sv) {
                THROW_NOT_IMPLEMENTED_ERROR("advanced_external_builder::append_target_element '%1%'"_fmt % lname);
            } else {
                throw exception("unknown element <%1% xmlns=\"%2%\">"_fmt % lname % ens);
            }
            */
            // is parent an attribute tag?
            if (parents.size() > 1) { // <some-tag><some-tag.some-attr><e>
                element& e_parent = parents.back();
                element const& e_snd_parent = parents[parents.size() - 2];
                if (e_parent.name.starts_with(e_snd_parent.name) && e_parent.name.size() > e_snd_parent.name.size() && e_parent.name[e_snd_parent.name.size()] == '.') {
                    if (!e_parent.text.empty()) {
                        e_parent.text.push_back(',');
                    }
                    e_parent.text.push_back('$');
                    e_parent.text.append(e.id);
                    return;
                }
            }
            //rb.append_vm_append_child_object_creation_code(parents.back().id, e.id);
        }
    }

    void close_element(span<element> parents, element& e) override
    {
        if (parents.size() == 1) {
            rb.vm_.append_ret();
            rb.contexts_.pop_back();
        }
    }
};

advanced_external_builder::advanced_external_builder()
{
    //attr_rslv_.emplace<document_attribute_resolver>(*this);
    //create_object_cmd_id_ = vm_.add_command<create_object_cmd>();
    //append_child_object_cmd_id_ = vm_.add_command<append_child_object_cmd>();
}

std::string advanced_external_builder::generate_id() const
{
    std::ostringstream namess;
    namess << "_id" << id_counter_++;
    return namess.str();
}

std::tuple<blob_result, func_type> advanced_external_builder::operator()(string_view element, string_view attr_name, string_view attr_value) const
{
    auto it = types_.find(element, hasher{}, std::equal_to{});
    if (it == types_.end()) [[unlikely]] {
        throw exception("unknown element <%1%> while attribute %2% resolving"_fmt % element % attr_name);
    }
    x_type const* xt = &*it;
    string_view full_attr_name = attr_name;
    for (;;) {
        auto it = std::find(attr_name.begin(), attr_name.end(), '.');
        if (it == attr_name.end()) break;
        string_view attr{ attr_name.data(), (size_t)(it - attr_name.begin()) };
        x_parameter const* p = xt->find_parameter(attr);
        if (!p) {
            throw exception("an unknown attribute '%1%' of element '%2%' with a value '%3%'"_fmt % attr % element % attr_value);
        }
        boost::container::small_vector<x_object, 1> resolved_types;
        for (auto const& t : p->type) {
            if (x_object const* pobjt = get<x_object>(&t); pobjt) {
                resolved_types.push_back(*pobjt);
            }
        }
        if (resolved_types.empty()) {
            throw exception("the attribute '%1%' of element <%2%> hasn't object type"_fmt % attr % element);
        }
        if (resolved_types.size() != 1) {
            THROW_NOT_IMPLEMENTED_ERROR("multiple object types");
        }
        xt = resolved_types.front().type;
        attr_name = attr_name.substr(attr.size() + 1);
    }

    x_parameter const* p = xt->find_parameter(attr_name);
    if (!p) {
        throw exception("an unknown attribute '%1%' of element '%2%' with a value '%3%'"_fmt % full_attr_name % element % attr_value);
    }

    for (auto const& t : p->type) {
        if (auto pairval = apply_visitor(type_value_parser_visitor{ attr_value, true }, t); pairval.first) {
            return { *pairval.first, func_type::unspecified };
        }
    }
    //if (attr_rslv_) {
    //    return attr_rslv_->operator()(element, attr_name, attr_value);
    //}
    THROW_NOT_IMPLEMENTED_ERROR("advanced_external_builder::operator()");
}

string_view advanced_external_builder::query_ns(element const& e, span<const element> parents) const
{
    auto delim_it = std::find(e.name.begin(), e.name.end(), ':');
    string_view ns;
    if (delim_it != e.name.end()) {
        ns = string_view{ &e.name.front(), static_cast<size_t>(delim_it - e.name.begin()) };
    }

    for (element const* pelem = &e;;) {
        if (ns.empty() && !pelem->ns.empty()) {
            return pelem->ns;
        }
        if (!ns.empty()) {
            auto prefix_it = std::find_if(pelem->prefixes.begin(), pelem->prefixes.end(), [ns](auto const& tpl) { return std::get<0>(tpl) == ns; });
            if (prefix_it != pelem->prefixes.end()) {
                return std::get<1>(*prefix_it);
            }
        }
        if (parents.empty()) break;
        pelem = &parents.back();
        parents = parents.subspan(0, parents.size() - 1);
    }
    if (ns.empty()) {
        return ns_;
    }
    auto prefix_it = std::find_if(ns_prefixes_.begin(), ns_prefixes_.end(), [ns](auto const& tpl) { return std::get<0>(tpl) == ns; });
    if (prefix_it != ns_prefixes_.end()) {
        return std::get<1>(*prefix_it);
    }
    throw exception("can't resolve the prefix '%1%' of an element <%2%>"_fmt % ns % e.name);
}

string_view advanced_external_builder::local_name(element const& e) const
{
    auto delim_it = std::find(e.name.begin(), e.name.end(), ':');
    if (delim_it != e.name.end()) {
        ++delim_it;
        return string_view{ &*delim_it, static_cast<size_t>(e.name.end() - delim_it) };
    }
    return e.name;
}

template <typename DelimFunctorT, typename ConsumerT>
void split(string_view str, DelimFunctorT const& df, ConsumerT const& f)
{
    auto it = str.begin();
    for (;;) {
        for (;; ++it) {
            if (it == str.end()) return;
            if (!df(*it)) break;
        }
        auto b = it;
        for (++it;; ++it) {
            if (it == str.end() || df(*it)) {
                if (!f(string_view{ &*b, (size_t)(it - b) }) || it == str.end()) return;
                ++it;
                break;
            }
        }
    }
}

void advanced_external_builder::append_type(x_type & t, small_string* ext)
{
    if (ext) {
        auto params = std::move(t.parameters);
        t.parameters.clear();
        split(*ext, [](char c) { return std::isspace(c); }, [this, &t](string_view bname) {
            auto it = types_.find(bname, hasher{}, std::equal_to<>{});
            if (it == types_.end()) [[unlikely]] {
                throw exception("unknown type '%1%'"_fmt % bname);
            }
            if (t.parameters.empty()) {
                t.parameters.insert(it->parameters.begin(), it->parameters.end());
            } else {
                for (auto const& p : it->parameters) {
                    t.append_parameter(p);
                }
            }
            return true;
        });
        for (auto const& p : params) {
            t.append_parameter(p);
        }
    }

    auto it = types_.find(t);
    if (it != types_.end()) [[unlikely]] {
        throw exception("type '%1%' is already defined"_fmt % t.name);
    }
    types_.insert(it, std::move(t));
}

void advanced_external_builder::append_element(span<element> parents, element& e)
{
    if (parents.empty()) { // root
        if (e.name != "document"sv) [[unlikely]] {
            throw exception("<document> must be the root element instead of '%1%'"_fmt % e.name);
        }
        ns_prefixes_ = std::move(e.prefixes);
        ns_ = std::move(e.ns);
        return;
    }

    if (!contexts_.empty()) {
        return contexts_.back()->append_element(parents, e);
    }

    string_view ens = query_ns(e, parents);
    string_view lname = local_name(e);
    if (ens == X_NS) {
        if (lname == "type"sv) {
            contexts_.emplace_back(make_shared<x_type_context>(*this));
            return;
        } else if (lname == "code"sv) {
            contexts_.emplace_back(make_shared<x_code_context>(*this));
            return;
        } else if (lname == "template"sv) {
            THROW_NOT_IMPLEMENTED_ERROR("advanced_external_builder::append_element template");
        } 
    } else if (ens == TARGET_NS) {
        if (e.id.empty()) {
            e.id = generate_id();
        }

        auto it = types_.find(lname, hasher{}, std::equal_to{});
        if (it != types_.end()) {
            create(lname, e.id);

            // is parent an attribute tag?
            if (parents.size() > 1) { // <some-tag><some-tag.some-attr><e>
                element& e_parent = parents.back();
                element const& e_snd_parent = parents[parents.size() - 2];
                string_view e_parent_lname = local_name(e_parent);
                string_view e_snd_parent_lname = local_name(e_snd_parent);
                if (e_parent_lname.starts_with(e_snd_parent_lname) && e_parent_lname.size() > e_snd_parent_lname.size() && e_parent_lname[e_snd_parent_lname.size()] == '.') {
                    if (!e_parent.text.empty()) {
                        e_parent.text.push_back(',');
                    }
                    e_parent.text.push_back('$');
                    e_parent.text.append(e.id);
                    return;
                }
            }
            // to do: check parent's body type
            if (parents.size() > 1) {
                append(parents.back().id, e.id);
            }
            return;
        }
        /*
        auto roit = root_objects_.find(e.id, hasher{}, string_equal_to{});
        if (roit != root_objects_.end()) {
            throw exception("a tag with id: '%1%' has been already defined"_fmt % e.id);
        }
        root_objects_.insert(roit, std::pair{ std::string{e.id}, vm_.get_ip() });
        
        if (x_type const* pobj = append_vm_object_creation_code(lname, e.id); pobj) {
            contexts_.emplace_back(make_shared<x_type_instance_context>(*this, *pobj));
            return;
        }
        */

        // loking for a template
    }
    throw exception("unexpected tag <%1%> xmlns=\"%2%\">"_fmt % lname % ens);

    //append_target_element(ens, lname, parents, e);
    
}

void advanced_external_builder::close_element(span<element> parents, element& e)
{
    if (!contexts_.empty()) {
        return contexts_.back()->close_element(parents, e);
    }
    
    if (parents.empty()) { // document tag
        for (auto& pair : e.attrs) {
            this->set_property(""sv, pair.first, string_blob_result(pair.second));
        }
    } else {
        string_view lname = local_name(e);
        //auto it = types_.find(lname, hasher{}, std::equal_to{});
        //BOOST_ASSERT(it != types_.end());
        //x_type const& xt = *it;

        for (auto const& attr_pair : e.attrs) {
            auto const& attrname = attr_pair.first;
            auto const& attrvalue = attr_pair.second;
            auto tpl = this->operator()(lname, attrname, attrvalue);
            if (std::get<0>(tpl).type == blob_type::function) {
                set_property_functional(e.id, attrname, as<string_view>(std::get<0>(tpl)), func_type::is_functor); // std::get<1>(tpl));
            } else {
                set_property(e.id, attrname, std::get<0>(tpl));
            }
        }
    }

    /*
    if (instance) {
        auto iit = root_objects_.find(string_view{*instance}, hasher{}, string_equal_to{});
        if (iit == root_objects_.end()) {
            throw exception("can't find tag with id: '%1%'"_fmt % *instance);
        }
        vm_context ctx{ *this };
        vm_.run(ctx, iit->second);
        this->set_property(""sv, "root"sv, string_blob_result(*instance));
    }
    */
}
/*
detail::x_type const* advanced_external_builder::append_vm_object_creation_code(string_view lname, string_view eid)
{
    auto it = types_.find(lname, hasher{}, std::equal_to{});
    if (it == types_.end()) return nullptr;
    size_t lname_idx = push_vm_value(string_blob_result(lname));
    size_t eid_idx = push_vm_value(string_blob_result(eid));
    vm_.append_push(eid_idx);
    vm_.append_push(lname_idx);
    vm_.append_command(create_object_cmd_id_); // embedded pop(2)
    return &*it;
}

void advanced_external_builder::append_vm_append_child_object_creation_code(string_view parentid, string_view childid)
{
    size_t parentid_idx = push_vm_value(string_blob_result(parentid));
    size_t childid_idx = push_vm_value(string_blob_result(childid));
    vm_.append_push(childid_idx);
    vm_.append_push(parentid_idx);
    vm_.append_command(append_child_object_cmd_id_); // embedded pop(2)
}
*/

size_t advanced_external_builder::push_vm_value(blob_result b)
{
    THROW_NOT_IMPLEMENTED_ERROR("advanced_external_builder::push_vm_value");
    //auto it = vm_values_.find(b, hasher{}, std::equal_to<>{});
    //if (it == vm_values_.end()) {
    //    smart_blob val{ b };
    //    val.allocate();
    //    size_t index = vm_.push_on_stack(val);
    //    it = vm_values_.insert(it, std::pair{ std::move(val), index });
    //}
    //return it->second;
}

void advanced_external_builder::instantiate(detail::x_type const&)
{

}



}
