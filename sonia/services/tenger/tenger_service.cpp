//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tenger_service.hpp"

#include "sonia/utility/iterators/file_region_iterator.hpp"
#include "sonia/utility/template-engine/templated_bunch.hpp"
#include "sonia/utility/template-engine/lua_build_visitor.hpp"

namespace sonia::services {

tenger_service::tenger_service(tenger_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "Tenger");
}

void tenger_service::open()
{
    std::vector<char> filedata;
    file_region_iterator<const char> it(cfg_.template_path, 0, 65536);
    for (; !it.empty(); ++it) {
        filedata.insert_range(filedata.end(), span<const char>{*it});
    }

    templated_bunch tb;
    tb.build(string_view{ filedata.data(), filedata.size() });

    templates::lua_build_visitor lbv;
    lbv.template_path = cfg_.template_path.parent_path();
    tb.root().visit(lbv);
}

}
