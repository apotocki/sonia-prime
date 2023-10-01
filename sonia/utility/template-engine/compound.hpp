//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include "sonia/utility/invokation/invokable.hpp"

namespace sonia::templates {

class document_element;
class foreach_element;
class file_template_element;
class code_element;

class build_visitor
{
public:
    virtual ~build_visitor() = default;

    virtual void operator()(document_element&) = 0;
    virtual void operator()(foreach_element&) = 0;
    virtual void operator()(file_template_element&) = 0;
    virtual void operator()(code_element&) = 0;
};

class compound : public invokation::invokable
{
    friend class build_visitor;

public:
    explicit compound(string_view id) : id_{ id } {}

    virtual ~compound() = default;

    using placaholders_t = std::vector<std::pair<std::string, std::string>>;

    void append(shared_ptr<compound> child, placaholders_t placeholders);
    
    virtual void visit(build_visitor&) = 0;
    void visit_elements(build_visitor&);

    auto const& elements() const { return elements_; }
    string_view id() const { return id_; }

protected:
    std::vector<std::tuple<shared_ptr<compound>, placaholders_t>> elements_;
    small_string id_;
};

}
