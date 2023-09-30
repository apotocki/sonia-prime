//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#define TEST_FOLDER "template_test"

#include "sonia/config.hpp"
#include "sonia/filesystem.hpp"
#include "sonia/services.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "applied/sonia_test.hpp"

#include "sonia/utility/template-engine/templated_unit.hpp"
#include "sonia/utility/template-engine/templated_bunch.hpp"
#include "sonia/utility/template-engine/lua_build_visitor.hpp"
#include "sonia/utility/iterators/file_region_iterator.hpp"

using namespace sonia;

void bunch_template_test()
{
    defer { services::shutdown(); };

    std::vector<char> filedata;
    fs::path refpath = "templates/pods/openssl/openssl-iosx.xml";
    //fs::path refpath = "templates/libsln/libsln-template.xml";
    //fs::path refpath = "../../bundles/sonia-prime/tests/testdata/test-template.xml";
    file_region_iterator<const char> it(refpath, 0, 65536);
    for (; !it.empty(); ++it) {
        filedata.insert_range(filedata.end(), span<const char>{*it});
    }

    templated_bunch tb;
    tb.build(string_view{ filedata.data(), filedata.size() });

    templates::lua_build_visitor lbv;
    tb.root().visit(lbv);
    //GLOBAL_LOG_INFO() << lbv.code();

    //tb.run();
}

void basic_template_test()
{
    templated_unit tunit;
    std::vector<char> filedata;
    fs::path refpath = "../../bundles/sonia-prime/tests/testdata/test-template.sot";
    file_region_iterator<const char> it(refpath, 0, 65536);
    for (; !it.empty(); ++it) {
        filedata.insert_range(filedata.end(), span<const char>{*it});
    }

    tunit.build(span{ filedata });
    /*
    tunit.run(R"(
        function write_text(arg)
            io.write(arg)
        end
    )");
    */
}

void template_test_registrar()
{
    register_test(BOOST_TEST_CASE(&bunch_template_test));
    //register_test(BOOST_TEST_CASE(&basic_template_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(template_test_registrar)
#endif
