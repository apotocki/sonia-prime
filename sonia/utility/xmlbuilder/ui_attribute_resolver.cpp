//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ui_attribute_resolver.hpp"
#include "basic_attribute_resolver.ipp"

namespace sonia::xmlbuilder {

void ui_attribute_resolver::setup_view(string_view cmp)
{
    setup_set<bool>(cmp, { "disabled"sv, "enabled"sv, "hidden"sv, "empty" });
    setup_set<float>(cmp, { "x-weight"sv, "y-weight", "border-radius"sv });
    setup_set<std::string>(cmp, { "background-color"sv, "cell-valign"sv, "cell-align"sv });
    setup_set<std::array<float, 4>>(cmp, { "padding"sv, "margin"sv, "border-width"sv });
    setup_set<std::array<float, 2>>(cmp, { "weight"sv });
    setup_set<optional<std::array<float, 2>>>(cmp, { "width"sv, "height"sv });
    setup_set<std::tuple<float, float, std::string>>(cmp, { "border"sv });

    // object properties
    setup_set<std::string>(cmp, { "padding-adjuster"sv, "margin-adjuster"sv, "border-width-adjuster"sv, "border-radius-adjuster"sv });
}

void ui_attribute_resolver::setup_field(string_view cmp)
{
    setup_view(cmp);
    setup_set<std::string>(cmp, { "value"sv, "color"sv, "space-replacement"sv, "text-align"sv, "text-valign"sv, "placeholder"sv, "content-mode"sv, "autocapitalization"sv, "autocorrection"sv });
    setup_set<std::vector<float>>(cmp, { "font-size"sv });
    setup_fset(cmp, { "on-change"sv, "on-enter"sv, "on-begin"sv, "on-end"sv, "presentation-text"sv });
}

void ui_attribute_resolver::setup_row(string_view cmp)
{
    setup_set<bool>(cmp, { "highlighted"sv });
    setup_map(cmp, "label"sv, "label"sv);
    setup_map(cmp, "bar"sv, "h-bar"sv);
}

ui_attribute_resolver::ui_attribute_resolver()
{
    setup_set<std::string>("document"sv, { "root"sv });

    setup_view("div"sv);
    setup_set<std::string>("div"sv, { "fit"sv });
    setup_set<std::array<float, 2>>("div"sv, { "size"sv });

    setup_set<std::string>("unlimited-spacer"sv, { "background-color"sv });
    setup_set<std::tuple<float, float, std::string>>("unlimited-spacer"sv, { "border"sv });

    setup_view("h-bar"sv);
    setup_set<bool>("h-bar"sv, { "scrolling"sv });
    setup_set<float>("h-bar"sv, { "spacing"sv });

    setup_view("v-bar"sv);
    setup_set<bool>("v-bar"sv, { "scrolling"sv });
    setup_set<float>("v-bar"sv, { "spacing"sv });

    setup_view("flex-bar"sv);
    setup_set<float>("flex-bar"sv, { "spacing"sv });

    setup_view("label"sv);
    setup_set<int32_t>("label"sv, {"lines"sv});
    setup_set<std::string>("label"sv, { "text"sv, "color"sv, "text-align"sv, "text-valign"sv, "font-weight"sv });
    setup_set<std::vector<float>>("label"sv, { "font-size"sv });

    setup_view("img"sv);
    setup_set<std::string>("img"sv, { "src"sv, "content-mode"sv, "fit"sv });

    setup_view("button"sv);
    setup_set<std::string>("button"sv, {"layout"sv, "image"sv, "text"sv, "text-color"sv });
    setup_set<bool>("button"sv, { "shouldRecognizeSimultaneously"sv} );
    setup_fset("button"sv, { "on-tap"sv });
    setup_map("button"sv, "img"sv, "img"sv);
    setup_map("button"sv, "txt"sv, "label"sv);
    setup_map("button"sv, "bar"sv, "h-bar"sv);

    setup_view("switch-button"sv);
    setup_set<bool>("switch-button"sv, { "is-on"sv });
    setup_fset("switch-button"sv, { "on-change"sv });

    setup_field("text-field"sv);
    setup_set<bool>("text-field"sv, {"editable"sv });
    
    setup_field("num-field"sv);
    setup_set<std::string>("num-field"sv, { "min"sv, "max"sv });
    setup_set<bool>("num-field"sv, { "hex-mode"sv });

    setup_field("bytes-field"sv);
    setup_set<std::string>("bytes-field"sv, { "encoding"sv });
    setup_set<int64_t>("bytes-field"sv, { "max-count"sv });

    setup_set<std::string>("split"sv, {"first"sv, "second"sv});

    // controllers
    setup_set<std::string>("form-ctl"sv, { "descriptor"sv });
    setup_set<std::string>("section"sv, { "title"sv });

    setup_row("row"sv);

    setup_row("text-row"sv);
    setup_map("text-row"sv, "field"sv, "text-field"sv);
    setup_set<std::string>("text-row"sv, {"value"sv});
    setup_fset("text-row"sv, { "on-change"sv, "on-enter"sv, "on-begin"sv, "on-end"sv, "presentation-text"sv });

    setup_row("numeric-row"sv);
    setup_map("numeric-row"sv, "field"sv, "num-field"sv);
    setup_set<std::string>("numeric-row"sv, {"value"sv});
    setup_fset("numeric-row"sv, { "on-change"sv, "on-enter"sv, "on-begin"sv, "on-end"sv, "presentation-text"sv });

    setup_row("bytes-row"sv);
    setup_map("bytes-row"sv, "field"sv, "bytes-field"sv);
    setup_set<std::string>("bytes-row"sv, {"value"sv});
    setup_fset("bytes-row"sv, { "on-change"sv, "on-enter"sv, "on-begin"sv, "on-end"sv });

    setup_row("encoding-row"sv);
    setup_set<std::string>("encoding-row", { "descriptor"sv });
    setup_set<std::string>("encoding-row"sv, {"value"sv});
    setup_fset("encoding-row"sv, { "on-change"sv, "on-enter"sv, "presentation-text"sv, "sample-text"sv });

    setup_row("algorithm-row"sv);
    setup_set<std::string>("algorithm-row", { "descriptor"sv });
    setup_set<std::string>("algorithm-row"sv, {"value"sv});
    setup_fset("algorithm-row"sv, { "on-change"sv, "on-enter"sv, "presentation-text"sv });

    setup_set<std::array<float, 4>>("linear-adjuster"sv, {"height-constraint"sv, "mask"sv});
    setup_set<std::string>("component-adjuster"sv, {"left"sv, "top"sv, "right"sv, "bottom"sv});

    setup_set<std::string>("storage"sv, {"value"sv});
    setup_fset("storage"sv, { "on-change"sv, "on-reset"sv });
    /*
    const char* booleanSet[] = { "is-on" };
        for (const char* attrname : booleanSet) { set<bool>(attrname); }

        const char* floatSet[] = { "progress" };
        for (const char* attrname : floatSet) { set<float>(attrname); }

        const char* stringSet[] = { "name", "type", "value", "color", "tint-color", "font", "font-weight",  , , "image-content-mode", "fit", ,  };
        for (const char* attrname : stringSet) { set<std::string>(attrname); }

        const char* funcSet[] = { "on-click", , "on-enter" };
        for (const char* attrname : funcSet) { 
            auto it = find_iterator(attrname);
            attr_parsers.insert(it, std::pair{ to_string(attrname), in_place_type<functional_attr_parser> });
        }
    */
}

}
