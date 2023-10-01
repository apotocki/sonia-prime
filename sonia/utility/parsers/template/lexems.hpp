//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

namespace sonia::parsers::tmpl {

enum lexem {
    ID_LINE_COMMENT = 1,    // // up to eol
    ID_COMMENT,         // /* ... */
    ID_CODE_SWITCH_START,     // <%
    ID_CODE_SWITCH_END,     // %>
    ID_EXPR_SWITCH,         // %
    ID_ESCAPED_PROCENT, // %%
    ID_NAME,            // prop name
    ID_ANY          // any character
};

}
