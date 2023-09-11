//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "xmlbuilder.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include "sonia/utility/parsers/xml/model.hpp"
#include "sonia/utility/parsers/xml/lexertl_lexer.hpp"
#include "sonia/utility/parsers/xml/parser.hpp"

#include "sonia/utility/automatic_polymorphic.hpp"

#include "sonia/singleton.hpp"
#include "sonia/logger/logger.hpp"

#include <boost/algorithm/string.hpp>

#if __APPLE__ || __ANDROID__
namespace std::views {

template <typename RangeT>
std::vector<RangeT> split(RangeT rng, RangeT delim)
{
    std::vector<RangeT> result;
    boost::split(result, rng, boost::is_any_of(delim));
    return result;
}

}
#else
#   include <ranges>
#endif

namespace sonia::xmlbuilder {

class element
{
public:
    std::string id;
    std::string name;
    std::string text;
    std::vector<std::pair<std::string, smart_blob>> attrs;
    std::vector<std::tuple<std::string, std::string, bool>> functionals; // name, code, no_return
};

class attr_parser
{
public:
    virtual ~attr_parser() = default;

    // {explicit value, funcname, no_return} if null or empty => not takes place
    virtual std::tuple<blob_result, std::string, bool> parse(string_view val, external_builder& eb) const
    {
        if (val.starts_with("{") && val.ends_with("}")) {
            string_view code = val.substr(1, val.size() - 2); // val.subview(1, val.size() - 2);
            bool no_return;
            if (code.starts_with("{") && code.ends_with("}")) {
                code = code.substr(1, code.size() - 2);
                no_return = true;
            } else {
                no_return = false;
            }
            return { nil_blob_result(), std::string{code}, no_return };
        }
        return { nil_blob_result(), {}, false};
    }
};

using automatic_attr_parser = automatic_polymorphic<attr_parser, sizeof(void*)>;

template <typename T>
class particular_attr_parser : public attr_parser
{
public:
    std::tuple<blob_result, std::string, bool> parse(string_view value, external_builder& eb) const override
    {
        if (auto r = attr_parser::parse(value, eb); !std::get<1>(r).empty()) return r;
        try {
            return { particular_blob_result(boost::lexical_cast<T>(value)), {}, false};
        } catch (boost::bad_lexical_cast const&) {
            throw exception("can't parse attribute value '%1%' as '%2%'"_fmt % value % typeid(T).name());
        }
        //throw exception("no attribute parser defined for type '%1%' to parse value: '%2%'"_fmt % typeid(T).name() % name);
    }
};

template <>
class particular_attr_parser<bool> : public attr_parser
{
public:
    std::tuple<blob_result, std::string, bool> parse(string_view value, external_builder& eb) const override
    {
        if (auto r = attr_parser::parse(value, eb); !std::get<1>(r).empty()) {
            return r;
        }
        if (value == "true" || value == "1") return { bool_blob_result(true), {}, false };
        if (value == "false" || value == "0") return { bool_blob_result(false), {}, false };
        throw exception("can't parse attribute value '%1%' as bool"_fmt % value);
    }
};

template <typename T, size_t SzV>
class particular_attr_parser<std::array<T, SzV>> : public attr_parser
{
    static constexpr bool is_simple_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
    using intermediate_element_t = std::conditional_t<is_simple_v, T, blob_result>;

public:
    std::tuple<blob_result, std::string, bool> parse(string_view value, external_builder& eb) const override
    {
        if (auto r = attr_parser::parse(value, eb); !std::get<1>(r).empty()) return r;
        std::vector<intermediate_element_t> result;
        result.resize(SzV);
        auto rng = std::views::split(value, " "sv);
        auto it = rng.begin(), eit = rng.end();
        for (auto elem_it = result.begin(); elem_it != result.end(); ++elem_it) {
            if (it != eit) {
                string_view value{ (*it).begin(), (*it).end() }; ++it;
                smart_blob elem{std::get<0>(particular_attr_parser<T>().parse(value, eb))};
                if constexpr (is_simple_v) {
                    *elem_it = from_blob<T>{}(*elem);
                } else {
                    *elem_it = elem.detach();
                }
            } else if (elem_it != result.begin()) {
                *elem_it = *(elem_it - 1);
            }
        }
        
        blob_result r = array_blob_result(std::span{ result });
        blob_result_allocate(&r);
        return { r , {}, false };
    }
};

template <typename T>
class particular_attr_parser<std::vector<T>> : public attr_parser
{
    static constexpr bool is_simple_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
    using intermediate_element_t = std::conditional_t<is_simple_v, T, blob_result>;

public:
    std::tuple<blob_result, std::string, bool> parse(string_view value, external_builder& eb) const override
    {
        if (auto r = attr_parser::parse(value, eb); !std::get<1>(r).empty()) return r;
        std::vector<intermediate_element_t> result;
        auto rng = std::views::split(value, " "sv);
        for (auto it = rng.begin(), eit = rng.end(); it != eit; ++it) {
            string_view value{ (*it).begin(), (*it).end() }; 
            smart_blob elem{ std::get<0>(particular_attr_parser<T>().parse(value, eb)) };
            if constexpr (is_simple_v) {
                result.push_back(from_blob<T>{}(*elem));
            } else {
                result.push_back(elem.detach());
            }
        }
        if (result.empty()) {
            return { nil_blob_result(), {}, false };
        } else if (result.size() == 1) {
            return { particular_blob_result(result[0]), {}, false };
        }
        blob_result r = array_blob_result(std::span{ result });
        blob_result_allocate(&r);
        return { r , {}, false };
    }
};
/*
template <>
class particular_attr_parser<std::vector<value_type>> : public attr_parser
{
public:
    value_type parse(string_view value) const override
    {
        std::vector<value_type> result;
        auto rng = std::views::split(value, " "sv);
        auto it = rng.begin(), eit = rng.end();
        for (auto elem_it = result.begin(); elem_it != result.end(); ++elem_it) {
            if (it != eit) {
                string_view value{ (*it).begin(), (*it).size() }; ++it;
                result.push_back(particular_attr_parser<T>().parse(value));
            }
            else if (elem_it != result.begin()) {
                *elem_it = *(elem_it - 1);
            }
        }
    }
};
*/

template <typename ... Ts>
class particular_attr_parser<std::tuple<Ts ...>> : public attr_parser
{
    using tuple_t = std::tuple<Ts ...>;

    template <size_t I, typename IteratorT>
    inline static void set(IteratorT & it, std::vector<blob_result>& v, external_builder& eb)
    {
        using element_t = std::tuple_element_t<I, tuple_t>;
        string_view value{(*it).begin(), (*it).end()}; ++it;
        v.push_back(std::get<0>(particular_attr_parser<element_t>().parse(value, eb)));
    }

    template <size_t ... Ids, typename RangeT>
    static void do_parse(std::index_sequence<Ids ...>, RangeT rng, std::vector<blob_result>& v, external_builder& eb)
    {
        auto it = rng.begin();
        (..., set<Ids>(it, v, eb));
        //(..., (std::get<Ids>(v) = particular_attr_parser<std::tuple_element_t<Ids, tuple_t>>().parse(*it++)));
    }

public:
    std::tuple<blob_result, std::string, bool> parse(string_view value, external_builder& eb) const override
    {
        if (auto r = attr_parser::parse(value, eb); !std::get<1>(r).empty()) return r;
        std::vector<blob_result> result;
        do_parse(std::make_index_sequence<sizeof... (Ts)>(), std::views::split(value, " "sv), result, eb);
        blob_result r = array_blob_result(std::span{ result });
        blob_result_allocate(&r);
        return { r, {}, false };
    }
};

/*
template <>
class particular_attr_parser<int64_t> : public attr_parser
{
public:
    value_type parse(string_view name) const override
    {
        return boost::lexical_cast<int64_t>(name);
    }
};

template <>
class particular_attr_parser<float> : public attr_parser
{
public:
    value_type parse(string_view name) const override
    {
        return boost::lexical_cast<int64_t>(name);
    }
};
*/

class functional_attr_parser: public attr_parser
{
public:
    std::tuple<blob_result, std::string, bool> parse(string_view value, external_builder& eb) const override
    {
        if (auto r = attr_parser::parse(value, eb); !std::get<1>(r).empty()) {
            return { nil_blob_result(), std::get<1>(r), true };
        }
        throw exception("can't parse attribute value '%1%' as a function body"_fmt % value);
    }
};

class attr_type_mapping : public singleton
{
    boost::unordered_map<std::string, automatic_attr_parser, hasher> attr_parsers;

    auto find_iterator(string_view name)
    {
        auto it = attr_parsers.find(name, hasher{}, string_equal_to{});
        if (it != attr_parsers.end()) {
            throw exception("an attribute parser for '%1%' is already registered"_fmt % name);
        }
        return it;
    }

    template <typename T>
    void set(string_view name)
    {
        auto it = find_iterator(name);
        attr_parsers.insert(it, std::pair{std::string{name}, in_place_type<particular_attr_parser<T>>});
    }

public:
    attr_type_mapping()
    {
        const char* booleanSet[] = { "is-on", "disabled", "enabled", "hidden" };
        for (const char* attrname : booleanSet) { set<bool>(attrname); }

        const char* intSet[] = { "lines" };
        for (const char* attrname : intSet) { set<int64_t>(attrname); }

        const char* floatSet[] = { "x-weight", "y-weight", "spacing", "progress" };
        for (const char* attrname : floatSet) { set<float>(attrname); }

        const char* stringSet[] = { "name", "type", "value", "placeholder", "src", "text", "image", "color", "text-color", "background-color", "tint-color", "font", "font-weight", "cell-valign", "cell-align", "text-align", "text-valign", "content-mode", "image-content-mode", "fit", "autocapitalization", "autocorrection", "layout" };
        for (const char* attrname : stringSet) { set<std::string>(attrname); }

        const char* tuple2Set[] = { "weight", "width", "height", "size" };
        for (const char* attrname : tuple2Set) { set<std::array<float, 2>>(attrname); }

        const char* tuple4Set[] = { "margin", "padding" };
        for (const char* attrname : tuple4Set) { set<std::array<float, 4>>(attrname); }

        const char* vecSet[] = { "font-size" };
        for (const char* attrname : vecSet) { set<std::vector<float>>(attrname); }

        const char* funcSet[] = { "on-tap", "on-click", "on-change", "on-end", "on-enter" };
        for (const char* attrname : funcSet) { 
            auto it = find_iterator(attrname);
            attr_parsers.insert(it, std::pair{ to_string(attrname), in_place_type<functional_attr_parser> });
        }

        set<std::tuple<float, float, std::string>>("border");
    }

    std::tuple<blob_result, std::string, bool> do_parse(string_view attr_name, string_view attr_value, external_builder& eb)
    {
        string_view attr_name_local = attr_name;
        if (auto it = std::find(attr_name.rbegin(), attr_name.rend(), '.'); it != attr_name.rend()) {
            attr_name_local = attr_name_local.substr(it.base() - attr_name.begin());
        }

        auto it = attr_parsers.find(attr_name_local, hasher{}, string_equal_to{});
        if (it == attr_parsers.end()) {
            throw exception("an attribute parser for '%1%' is not registered to parse value '%2%'"_fmt % attr_name_local % attr_value);
        }
        return it->second->parse(attr_value, eb);
    }

    static std::tuple<blob_result, std::string, bool> parse(string_view attr_name, string_view attr_value, external_builder& eb)
    {
        return as_singleton<attr_type_mapping>().get().do_parse(attr_name, attr_value, eb);
    }
};

class builder_model : public sonia::parsers::xml::model_base<builder_model>
{
    using base_t = sonia::parsers::xml::model_base<builder_model>;

public:
    explicit builder_model(external_builder& e) : eb{e} {}

    bool is_xml_decl_optional() const { return true; }

    void on_begin_element();
    void on_end_element();// on '>'
    void on_close_element(); // on '/>'
    void on_close_tag();// on '</' name

    void on_char_data(); // element's text

    template <typename IteratorT>
    void put_cdata_string(IteratorT b, IteratorT e)
    {
        base_t::put_cdata_string(std::move(b), std::move(e));
        on_char_data();
    }

    void on_attribute_value();

private:
    std::string generate_id();
    void do_close_tag();

private:
    external_builder& eb;
    std::vector<element> stack_;
    int id_counter_{ 0 };
};

std::string builder_model::generate_id()
{
    std::ostringstream namess;
    namess << "_id" << id_counter_++;
    return namess.str();
}

void builder_model::on_begin_element()
{
    auto tag = to_string_view(tempstr_);
    stack_.emplace_back();
    stack_.back().name = std::string{tag};
    //GLOBAL_LOG_INFO() << "begin " << tag;
}

void builder_model::on_end_element()
{
    if (stack_.size() == 1) return; // skip root

    element& e = stack_.back();

    // is attribute tag?
    if (stack_.size() > 1) {
        element& e_parent = stack_[stack_.size() - 2];
        if (e.name.starts_with(e_parent.name) && e.name.size() > e_parent.name.size() && e.name[e_parent.name.size()] == '.') {
            // just skip for now
            return;
        }
    }
    
    if (e.id.empty()) {
        e.id = generate_id();
    }

    eb.create(e.name, e.id);
    for (auto const& attr : e.attrs) {
        eb.set_property(e.id, attr.first, *attr.second);
    }
    for (auto const& func : e.functionals) {
        eb.set_property_functional(e.id, std::get<0>(func), std::get<1>(func), std::get<2>(func));
    }
    if (stack_.size() > 2) {
        eb.append(stack_[stack_.size() - 2].id, e.id);
    }
}

void builder_model::on_close_element()
{
    on_end_element();
    do_close_tag();
}

void builder_model::on_close_tag()
{
    auto tag = to_string_view(tempstr_);
    if (stack_.empty()) {
        throw exception("unexpected closing tag, no closeing expected");
    }
    if (stack_.empty() || stack_.back().name != tag) {
        throw exception("unexpected closing tag, expected </%1%>"_fmt % stack_.back().name);
    }
    do_close_tag();
    //GLOBAL_LOG_INFO() << "end " << tag;
}

void builder_model::do_close_tag()
{
    element& e = stack_.back();

    // is attribute tag?
    if (stack_.size() > 1) {
        element& e_parent = stack_[stack_.size() - 2];
        if (e.name.starts_with(e_parent.name) && e.name.size() > e_parent.name.size() && e.name[e_parent.name.size()] == '.') {
            // the element's text is the attribute value
            size_t parent_tag_name_sz = e_parent.name.size();
            string_view attrname{ e.name.data() + parent_tag_name_sz + 1, e.name.size() - parent_tag_name_sz - 1 };

            auto tpl = attr_type_mapping::parse(attrname, e.text, eb);
            if (!std::get<1>(tpl).empty()) {
                eb.set_property_functional(e_parent.id, attrname, std::move(std::get<1>(tpl)), std::get<2>(tpl));
            } else {
                eb.set_property(e_parent.id, attrname, std::move(std::get<0>(tpl)));
            }
            stack_.pop_back();
            return;
        }
    }

    if (!e.text.empty()) {
        eb.set_text(e.id, e.text);
    }
    stack_.pop_back();
}

void builder_model::on_char_data()
{
    boost::algorithm::trim(tempstr_);
    if (tempstr_.empty()) return;
    
    element& e = stack_.back();
    e.text += string_view(tempstr_.data(), tempstr_.size());
    //GLOBAL_LOG_INFO() << string_view(tempstr_.data(), tempstr_.size());
}

void builder_model::on_attribute_value()
{
    element& e = stack_.back();
    auto attrname = to_string_view(attr_name_);
    auto attrvalue = to_string_view(tempstr_);
    if (attrname == "id"sv) {
        e.id = std::string{attrvalue};
    } else {
        auto tpl = attr_type_mapping::parse(attrname, attrvalue, eb);
        if (!std::get<1>(tpl).empty()) {
            e.functionals.emplace_back(std::string{attrname}, std::move(std::get<1>(tpl)), std::get<2>(tpl));
        } else {
            e.attrs.emplace_back(std::string{attrname}, std::move(std::get<0>(tpl)));
        }
    }
}

//////////////////////////////////////////////////////////////////

void parse(string_view code, external_builder& eb)
{
    builder_model model{ eb };

    sonia::parsers::parse<
        sonia::parsers::xml::lexertl_lexer,
        sonia::parsers::xml::parser
    >(model, code.data(), code.data() + code.size());

    //return { std::move(model.vars), std::move(model.result) };

}


/*

create_component(typename, id)
set_component_property(component id, property name, property value)
append(parent component id, child component id)

*/

}
