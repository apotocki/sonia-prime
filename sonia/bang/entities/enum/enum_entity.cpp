//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "enum_entity.hpp"

#include "sonia/bang/unit.hpp"
#include "sonia/bang/entities/functional.hpp"

namespace sonia::lang::bang {

enum_entity::enum_entity(unit& u, functional& fn, std::vector<identifier> cases)
    : name_{ fn.name() }
    , cases_{ std::move(cases) }
{
    sig_.name = fn.id();
    sig_.result.emplace(u.get(builtin_eid::typename_));
}

optional<size_t> enum_entity::find(identifier val) const
{
    auto it = std::find(cases_.begin(), cases_.end(), val);
    if (it != cases_.end()) {
        return std::distance(cases_.begin(), it);
    }
    return nullopt;
}
//enum_entity::enum_case const* enum_entity::find(identifier id)
//{
//    auto it = std::ranges::lower_bound(cases, id, {}, [](auto const& e) { return e.name; });
//    return it == cases.end() ? nullptr : &*it;
//}

}
