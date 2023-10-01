//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "lua_build_visitor.hpp"

#include "document_element.hpp"
#include "foreach_element.hpp"
#include "file_template_element.hpp"
#include "code_element.hpp"

#include "templated_unit.hpp"
#include "sonia/utility/iterators/file_region_iterator.hpp"

namespace sonia::templates {

void lua_build_visitor::visit_children(compound & cmp)
{
    for (auto& elem : cmp.elements()) {
        auto child = std::get<0>(elem);
        compound::placaholders_t const& ps = std::get<1>(elem);
        for (auto const& pair : ps) {
            cstring_view ifn = append_inplace(pair.second, false);
            smart_blob actualvalue = eval_inplace(ifn, {});
            child->set_property(pair.first, *actualvalue);
        }
        child->visit(*this);
    }
}

void lua_build_visitor::operator()(document_element& de)
{
    if (!de.code.empty()) {
        append_code(de.code);
    }
    for (auto& elem : de.elements()) {
        //std::string ename = ("%1%_e%2%"_fmt % de.id() % (size_t)(&elem - de.elements().data())).str();
        auto child = std::get<0>(elem);
        compound::placaholders_t const& ps = std::get<1>(elem);
        for (auto const& pair : ps) {
            cstring_view ifn = append_inplace(pair.second, false);
            smart_blob actualvalue = eval_inplace(ifn, {});
            child->set_property(pair.first, *actualvalue);
            
            //code_ << "function " << ename << "_" << pair.first << "() return " << pair.second << " end\n";
            //code_ << "set(\"" << child->id() << "\", \"" << pair.first << "\", " << ename << "_" << pair.first << "())\n";
        }
        child->visit(*this);
    }
}

void lua_build_visitor::operator()(foreach_element& fe)
{
    // get numebr of items
    smart_blob bcount = eval(("#%1%"_fmt % fe.select).str());
    uint32_t count = from_blob<uint32_t>{}(*bcount);
    for (uint32_t index = 1; index <= count; ++index) {
        append_code(("%1% = %2%[%3%]\n"_fmt % fe.var % fe.select % index).str());
        visit_children(fe);
    }
}

void lua_build_visitor::operator()(file_template_element& fte)
{
    std::vector<char> filedata;
    if (!fte.src.empty()) {
        fs::path refpath = std::u8string_view{reinterpret_cast<char8_t const*>(fte.src.data()), fte.src.size()};
        if (!refpath.is_absolute()) {
            fs::path try_path0 = template_path / refpath;
            if (fs::exists(try_path0) && fs::is_regular_file(try_path0)) {
                refpath = try_path0;
            } else if (!fs::exists(refpath) || !fs::is_regular_file(refpath)) {
                throw exception("template %1% is not found"_fmt % refpath);
            }
        }
        file_region_iterator<const char> it(refpath, 0, 65536);
        for (; !it.empty(); ++it) {
            span<const char> sp{*it};
            filedata.insert(filedata.end(), sp.begin(), sp.end());
        }
    }
    if (!fte.code.empty()) {
        filedata.insert(filedata.end(), fte.code.begin(), fte.code.end());
    }

    if (auto it = installed_ids_.find(fte.id(), hasher{}, string_equal_to{}); it == installed_ids_.end()) {
        templated_unit tunit;
        tunit.build(span{ filedata });

        std::ostringstream fncode;
        fncode << "function " << fte.id() << "_file(writer, input)\n" << tunit.result() << "\nend";
        append_code(fncode.str());
        installed_ids_.insert(it, std::string{ fte.id() });
    }

    std::ostringstream code;
    code << "file = io.open('" << fte.result << "', 'wb')\n"
        << fte.id() << "_file(function(t) file.write(file, t) end, " << fte.input << ")\n"
        //<< "print(file.write)\n"
        //<< "file.write(file, 'hello')\n"
        << "file:close()\n";
    append_code(code.str());
}

void lua_build_visitor::operator()(code_element& ce)
{
    if (auto it = installed_ids_.find(ce.id(), hasher{}, string_equal_to{}); it == installed_ids_.end()) {
        std::ostringstream fncode;
        fncode << "function " << ce.id() << "_code(input)\n" << ce.code << "\nend";
        append_code(fncode.str());
        installed_ids_.insert(it, std::string{ ce.id() });
    }
    append_code(("%1%_code(%2%)\n"_fmt % ce.id() % ce.input).str());
}

}
