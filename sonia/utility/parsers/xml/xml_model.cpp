//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "model.hpp"

namespace sonia::parsers::xml {

//model::model() {
//    stack_.reserve(16);
//    strings_.reserve(16);
//    state_stack_.reserve(16);
//}

//model::~model() {}
//
//void model::push_state(state s) {
//    state_stack_.emplace_back(s, stack_.size());
//}
//
//void model::put_null() {
//    stack_.push_back(json_value());
//}
//
//void model::put_boolean(bool v) {
//    stack_.push_back(json_value(v));
//}
//
//void model::put_object() {
//    size_t cnt = stack_.size() - state_stack_.back().second;
//    array_view<json_value> values(&stack_[stack_.size() - cnt], cnt);
//    array_view<std::string> names(&strings_[strings_.size() - cnt], cnt);
//    json_value obj(names, values);
//    stack_.resize(stack_.size() - cnt);
//    strings_.resize(strings_.size() - cnt);
//    stack_.push_back(std::move(obj));
//    state_stack_.pop_back();
//    state_stack_.pop_back();
//}
//
//void model::put_array() {
//    size_t cnt = stack_.size() - state_stack_.back().second;
//    array_view<json_value> values(&stack_[stack_.size() - cnt], cnt);
//    json_value obj(values);
//    stack_.resize(stack_.size() - cnt);
//    stack_.push_back(std::move(obj));
//    state_stack_.pop_back();
//    state_stack_.pop_back();
//}

}
