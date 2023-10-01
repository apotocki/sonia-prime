//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <sstream>

#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/parsers/template/model.hpp"

namespace sonia {

class templated_unit 
    : public parsers::tmpl::model
    //, private sonia::lua::language
{
public:
    void build(span<const char> code);
    //void run(string_view prefix);

    std::string result() { return result_builder_.str(); }

    // model
    void commit_text();
    void commit_expr();
    void commit_code();

protected:
    void write_escaped(string_view);
    void write_text_line(string_view, bool eol);

private:
    std::ostringstream result_builder_;
    bool text_need_trim_left_ = false;
};

}
