//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//
#pragma once

#include <boost/test/unit_test.hpp>

#define AUTOTEST(name) \
namespace { struct test_class { test_class() { name(); }}; test_class test; }

void register_test(boost::unit_test::test_case*);
