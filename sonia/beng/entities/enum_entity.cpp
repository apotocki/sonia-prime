//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "enum_entity.hpp"

namespace sonia::lang::beng {

enum_entity::enum_case const* enum_entity::find(identifier id)
{
    auto it = std::ranges::lower_bound(cases, id, {}, [](auto const& e) { return e.name; });
    return it == cases.end() ? nullptr : &*it;
}

}
