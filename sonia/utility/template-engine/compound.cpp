//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "compound.hpp"

namespace sonia::templates {

void compound::append(shared_ptr<compound> child, placaholders_t placaholders)
{
    elements_.emplace_back(std::move(child), std::move(placaholders));
}

//void compound::visit_elements(build_visitor bv)
//{
//    for (auto& elem : elements_) {
//        auto child = std::get<0>(elem);
//        placaholders_t const& ps = std::get<1>(elem);
//        for (auto const& pair : ps) {
//            smart_blob actualvalue = lang.eval_inplace(pair.second, {});
//            child->set_property(pair.first, *actualvalue);
//        }
//        child->visit(bv);
//    }
//}

}
