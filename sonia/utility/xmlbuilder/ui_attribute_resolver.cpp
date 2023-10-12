//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ui_attribute_resolver.hpp"
#include "basic_attribute_resolver.ipp"

namespace sonia::xmlbuilder {

void ui_attribute_resolver::setup_view(string_view cmp)
{
    setup_set<bool>(cmp, { "disabled"sv, "enabled"sv, "hidden"sv });
    setup_set<float>(cmp, { "x-weight"sv, "y-weight" });
    setup_set<std::string>(cmp, { "background-color"sv, "cell-valign"sv, "cell-align"sv });
    setup_set<std::array<float, 4>>(cmp, { "padding"sv, "margin"sv });
    setup_set<std::array<float, 2>>(cmp, { "weight"sv, "width"sv, "height"sv, "size"sv });
    setup_set<std::tuple<float, float, std::string>>(cmp, { "border"sv });
}

ui_attribute_resolver::ui_attribute_resolver()
{
    setup_view("div"sv);

    setup_view("h-bar"sv);
    setup_set<float>("h-bar"sv, { "spacing"sv });

    setup_view("v-bar"sv);
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
    setup_fset("button"sv, { "on-tap"sv });
    setup_map("button"sv, "img"sv, "img"sv);
    setup_map("button"sv, "txt"sv, "label"sv);
    setup_map("button"sv, "bar"sv, "h-bar"sv);

    setup_view("switch-button"sv);
    setup_set<bool>("switch-button"sv, { "is-on"sv });
    setup_fset("switch-button"sv, { "on-change"sv });

    setup_view("text-field"sv);
    setup_set<std::string>("text-field"sv, { "value"sv, "color"sv, "space-replacement"sv, "text-align"sv, "text-valign"sv, "placeholder"sv, "content-mode"sv, "autocapitalization"sv, "autocorrection"sv });
    setup_set<std::array<float, 2>>("text-field"sv, { "font-size"sv });
    setup_fset("text-field"sv, { "on-change"sv, "on-enter"sv, "on-begin"sv, "on-end"sv, "presentation-text"sv });

    setup_view("num-field"sv);
    setup_set<blob_result>("num-field"sv, { "value"sv, "color"sv, "text-align"sv, "text-valign"sv, "placeholder"sv, "content-mode"sv});
    setup_set<std::array<float, 2>>("num-field"sv, { "font-size"sv });
    setup_fset("num-field"sv, { "on-change"sv, "on-enter"sv, "on-begin"sv, "on-end"sv, "presentation-text"sv });

    // controllers
    setup_set<std::string>("form-ctl"sv, { "descriptor"sv });
    setup_set<std::string>("section"sv, { "title"sv });
    setup_set<bool>("row", { "highlighted"sv });
    setup_map("row"sv, "label"sv, "label"sv);
    setup_map("row"sv, "bar"sv, "h-bar"sv);

    setup_set<bool>("encoding-row", { "highlighted"sv });
    setup_map("encoding-row"sv, "label"sv, "label"sv);
    setup_map("encoding-row"sv, "bar"sv, "h-bar"sv);
    setup_set<std::array<std::string, 2>>("encoding-row"sv, {"value"sv});
    setup_fset("encoding-row"sv, { "on-change"sv, "on-enter"sv });

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
