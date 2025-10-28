//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "templated_bunch.hpp"
#include "document_element.hpp"
#include "foreach_element.hpp"
#include "file_template_element.hpp"
#include "code_element.hpp"

#include "sonia/singleton.hpp"

#include "sonia/utility/xmlbuilder/basic_attribute_resolver.ipp"

namespace sonia {

class templated_attribute_resolver
    : public xmlbuilder::basic_attribute_resolver
    , public singleton
{
public:
    templated_attribute_resolver()
    {
        setup_set<std::string>("document"sv, { "code"sv });
        setup_set<std::string>("for-each"sv, { "select"sv, "var"sv });
        setup_set<std::string>("if"sv, { "test"sv });
        setup_set<std::string>("file-template"sv, { "src"sv, "input"sv, "result"sv });
        setup_set<std::string>("code"sv, { "input"sv });
    }
};

//class code_element : public invocation::invocable {};

class templated_item_factory
    : public xmlbuilder::bunch_builder::factory
    , public singleton
{
    //template <typename T>
    //static view_ptr factory(string_view id)
    //{
    //    view_ptr result = make_view<T>();
    //    result->name = id;
    //    return result;
    //}

public:
    templated_item_factory()
    {
        //factories["code"] = [](string_view id) { return make_shared<code_element>(); };
        factories["document"] = [](string_view id) { return make_shared<templates::document_element>(id); };
        factories["for-each"] = [](string_view id) { return make_shared<templates::foreach_element>(id); };
        factories["file-template"] = [](string_view id) { return make_shared<templates::file_template_element>(id); };
        factories["code"] = [](string_view id) { return make_shared<templates::code_element>(id); };
    }

    shared_ptr<invocation::invocable> create(string_view type, string_view id) override
    {
        auto it = factories.find(type, hasher{}, string_equal_to{});
        if (it == factories.end())
            throw exception("A template type '%1%' was not found."_fmt % type);
        return it->second(id);
    }

    xmlbuilder::basic_attribute_resolver& get_attribute_resolver() override
    {
        return *as_singleton<templated_attribute_resolver>();
    }

private:
    boost::unordered_map<std::string, function<shared_ptr<invocation::invocable>(string_view)>, hasher> factories;
};

templated_bunch::templated_bunch()
{}

void templated_bunch::build(string_view xml)
{
    parse(xml, *this);
}

shared_ptr<templates::compound> templated_bunch::get_element_by(string_view id) const
{
    auto it = elements_.find(id, hasher{}, string_equal_to{});
    if (it == elements_.end()) throw exception("An element with identifier '%1%' was not found."_fmt % id);
    return it->second;
}

std::string templated_bunch::generate_id() const
{
    std::ostringstream namess;
    namess << "_id" << id_counter_++;
    return namess.str();
}

void templated_bunch::set_text(invocation::invocable& obj, string_view text)
{
    if (!obj.try_set_property("$"sv, string_blob_result(text))) {
        /*
        if (auto code = dynamic_cast<code_element*>(&obj); code) {
            lua::language::append_code(std::string{ text });
            return;
        }
        */
        throw exception("can't append text into %1%"_fmt % typeid(obj).name());
    }
}

void templated_bunch::append_element(span<xmlbuilder::element> parents, xmlbuilder::element& e)
{
    if (!parents.empty()) {
        // is attribute tag?
        xmlbuilder::element const& e_parent = parents.back();
        if (e.name.starts_with(e_parent.name) && e.name.size() > e_parent.name.size() && e.name[e_parent.name.size()] == '.') {
            // just skip for now
            return;
        }
    }

    if (e.id.empty()) {
        e.id = generate_id();
    }

    auto it = elements_.find(e.id, hasher{}, string_equal_to{});
    if (it != elements_.end()) throw exception("A duplicate element identifier '%1%' was found."_fmt % e.id);
    auto v = as_singleton<templated_item_factory>()->create(e.name, e.id);
    auto cmp = dynamic_pointer_cast<templates::compound>(v);
    if (!cmp) {
        auto & obj = *v;
        THROW_INTERNAL_ERROR("The element '%1%' is not a template compound"_fmt % typeid(obj).name());
    }

    if (parents.empty()) {
        if (root_) {
            throw exception("The root element is already defined.");
        }
        root_ = cmp;
    }

    elements_.insert(it, { std::string{e.id}, std::move(cmp) });
}

void templated_bunch::close_element(span<xmlbuilder::element> parents, xmlbuilder::element& e)
{
    auto child = get_element_by(e.id);
    if (!e.text.empty()) {
        set_text(*child, e.text);
    }
    
    std::vector<std::pair<std::string, std::string>> placeholders;
    auto const& ar = as_singleton<templated_item_factory>()->get_attribute_resolver();

    for (auto const& attr_pair : e.attrs) {
        auto const& attrname = attr_pair.first;
        auto const& attrvalue = attr_pair.second;
        auto tpl = ar(e.name, attrname, attrvalue);
        if (std::get<0>(tpl).type == blob_type::function) {
            placeholders.emplace_back(attrname, as<string_view>(std::get<0>(tpl)));
            //e.functionals.emplace_back(std::string{ attrname }, std::move(std::get<1>(tpl)), std::get<2>(tpl));
        } else {
            child->set_property(attrname, std::get<0>(tpl));
            //e.attrs.emplace_back(attrname, std::move(std::get<0>(tpl)));
        }
    }
    /*
    for (auto const& attr : e.attrs) {
        child->set_property(attr.first, *attr.second);
    }

    for (auto const& func : e.functionals) {
        string_view propname = std::get<0>(func);
        string_view code = std::get<1>(func);
        xmlbuilder::func_type no_return = std::get<2>(func);
        placeholders.emplace_back(propname, code);
    }
    */
    if (parents.empty()) return;
    auto parent = get_element_by(parents.back().id);
    parent->append(std::move(child), std::move(placeholders));
}

}
