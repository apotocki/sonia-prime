//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "view_model.hpp"
#include "view_model.ipp"
#include <boost/container/small_vector.hpp>

namespace sonia {

using namespace std::string_view_literals;

view_model::view_model(int32_t idval)
    : id_{idval}, locked_{0}
{

}

void view_model::do_registration(registrar_type & mr)
{
    mr.register_method<&view_model::async_cancel>("async_cancel"sv);
    mr.register_method<&view_model::echo_method>("echo"sv);
    mr.register_method<&view_model::call_method>("call"sv);
    mr.register_method<&view_model::get_method>("get"sv);
    mr.register_method<&view_model::set_method>("set"sv);
    mr.register_method<&view_model::inherit>("inherit"sv);
}

void view_model::set(int32_t idval)
{
    id_ = idval;
}

void view_model::inherit(int32_t baseid)
{
    if (baseid == id()) throw exception("can't inherit itself");
    bases_.insert(baseid);
}

bool view_model::has_method(string_view methodname) const
{
    if (invocable::has_method(methodname)) return true;
    shared_ptr<manager> mng = get_manager();
    if (!mng) return false;
    for (int32_t baseid : bases_) {
        if (mng->get_content_view(baseid)->has_method(methodname)) return true;
    }
    return false;
}

bool view_model::try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept
{
    if (invocable::try_invoke(methodname, args, result)) return true;
    shared_ptr<manager> mng = get_manager();
    if (!mng) return false;
    for (int32_t baseid : bases_) {
        if (mng->get_content_view(baseid)->try_invoke(methodname, args, result)) return true;
    }
    return false;
}

bool view_model::try_get_property(string_view propname, smart_blob& result) const
{
    if (invocable::try_get_property(propname, result)) return true;
    shared_ptr<manager> mng = get_manager();
    if (!mng) return false;
    for (int32_t baseid : bases_) {
        if (mng->get_content_view(baseid)->try_get_property(propname, result)) return true;
    }
    return false;
}

bool view_model::try_set_property(string_view propname, blob_result const& val)
{
    //GLOBAL_LOG_INFO() << "view_model::try_set_property: " << propname;
    if (invocable::try_set_property(propname, val)) return true;
    shared_ptr<manager> mng = get_manager();
    if (!mng) return false;
    for (int32_t baseid : bases_) {
        //GLOBAL_LOG_INFO() << "looking in base: " << baseid;
        if (mng->get_content_view(baseid)->try_set_property(propname, val)) return true;
    }
    return false;
}

/*
smart_blob view_model::get_property(string_view propname) const
{
    smart_blob result;
    if (!try_get_property(propname, result)) {
        blob_result args[] = { string_blob_result(propname) };
        return do_call_method("getProperty", std::span{args});
    }
    return result;
}

void view_model::set_property(string_view propname, blob_result const& val)
{
    if (!try_set_property(propname, val)) {
        blob_result args[] = { string_blob_result(propname), val };
        smart_blob result = do_call_method("setProperty", std::span{args});
        //throw exception("property %1% of %2% is not registered"_fmt % propname % typeid(*this).name());
    }
}
*/

/*
blob_result view_model::get_property(std::string_view propname, bool pin) const
{
    lock_guard guard(props_mtx_);
    if (auto it = properties_.find(propname, hasher{}, string_equal_to{}); it != properties_.end()) {
        blob_result result;
        if (blob_result const *pbr = boost::get<blob_result>(&it->second); pbr) {
            result = *pbr;
        } else if (auto const* pep = boost::get<fn_property_holder>(&it->second); pep) {
            result = (*pep)->get();
        } else {
            THROW_INTERNAL_ERROR("unexpected property nature");
        }
        if (pin && result.need_unpin) {
            if (auto mng = get_manager(); mng) {
                mng->pin(result);
            } else {
                // it's unsafe use a result because the buffer is already released
                throw exception("the assigned manager is obsoleted");
            }
        } else if (!pin && result.need_unpin) {
            result.need_unpin = 0;
        }
        return result;
    }
    return nil_blob_result();
}

void view_model::set_property(std::string_view propname, blob_result && val)
{
    auto mng = get_manager(); if (!mng) throw closed_exception();

    bool updated = false;
    // note: val is supposed to be pinned
    {
        lock_guard guard(props_mtx_);
        auto it = properties_.find(propname, hasher{}, string_equal_to{});
        if (it != properties_.end()) {
            if (blob_result * pbr = boost::get<blob_result>(&it->second); pbr) {
                mng->unpin(*pbr);
                if (val.data || val.type != blob_type::unspecified) {
                    *pbr = val;
                } else {
                    properties_.erase(it);
                }
                updated = true;
            } else if (auto * pep = boost::get<fn_property_holder>(&it->second); pep) {
                updated = (*pep)->set(std::move(val));
            } else {
                THROW_INTERNAL_ERROR("unexpected property nature");
            }

        } else if (val.data || val.type != blob_type::unspecified) {
            properties_.insert(it, std::pair{std::string(propname), val});
        }
    }

    if (updated) {
        on_change(status_type::PROPERTY_CHANGED_ST, string_blob_result(propname));
    }
}
*/

void view_model::on_property_change(string_view propname)
{
    on_change(status_type::PROPERTY_CHANGED_ST, {string_blob_result(propname)});
}

/*
bool view_model::has_method(std::string_view methodname) const
{
    auto it = methods_.find(methodname, hasher{}, string_equal_to{});
    return it != methods_.end();
}

blob_result view_model::invoke(cstring_view methodname, std::span<const blob_result> args)
{
    // a concurrent access is not supposed to be
    auto it = methods_.find(methodname, hasher{}, string_equal_to{});
    if (it != methods_.end()) {
        return (*it->second)(*this, args);
    }
    if (auto mng = get_manager(); mng) {
        return mng->eval_function(*this, methodname, args);
    }
    throw exception("no such method: '%1%'"_fmt % methodname);
}
*/

/*
blob_result view_model::do_call_method(string_view name, std::initializer_list<const blob_result> args) const
{
    return do_call_method(name, std::span{args});
}
*/

smart_blob view_model::do_call_method(std::string_view name, std::span<const blob_result> args) const
{
    if (auto mng = get_manager(); mng) {
        //GLOBAL_LOG_INFO() << "view_model::do_call_method " << name << ", argscount: " << args.size();
        return mng->invoke_callback(id(), name, args);
        /*
            [](void* cookie, blob_result* r, uint32_t count) {
                for (uint32_t i = 0; i < count; ++i) blob_result_pin(r);
                blob_result * result = reinterpret_cast<blob_result*>(cookie);
                if (count == 1) {
                    *result = *r;
                } else if (count > 0) {
                    *result = array_blob_result<blob_result>(std::span{r, (size_t)count});
                    blob_result_allocate(result);
                }
            }
       */
    }
    return {};
}

smart_blob view_model::call_method(std::string_view name, blob_result args) const
{
    if (is_nil(args)) {
        return do_call_method(name, {});
    } else if (!is_array(args) || contains_string(args) || args.type == blob_type::bigint) {
        return do_call_method(name, std::span{&args, 1});
    } else if (args.type == blob_type::tuple) {
        return do_call_method(name, std::span{ data_of<blob_result const>(args), array_size_of<blob_result>(args) });
    } else {
        boost::container::small_vector<blob_result, 16> bargs;
        blob_type_selector(args, [&bargs](auto ident, blob_result const& b) {
            using type = typename decltype(ident)::type;
            if constexpr (std::is_void_v<type> || std::is_same_v<mp::basic_integer_view<invocation_bigint_limb_type>, type>) { bargs.push_back(nil_blob_result()); }
            else {
                using fstype = std::conditional_t<std::is_same_v<type, bool>, uint8_t, type>;
                fstype const* begin_ptr = data_of<fstype>(b);
                for (auto* p = begin_ptr, *e = begin_ptr + array_size_of<fstype>(b); p != e; ++p) {
                    bargs.push_back(particular_blob_result(*begin_ptr));
                }
            }
        });
        return do_call_method(name, std::span{bargs});
    }
}

smart_blob view_model::get_method(std::string_view name) const
{
    if (auto mng = get_manager(); mng) {
        //GLOBAL_LOG_INFO() << "view_model::do_call_method " << name << ", argscount: " << args.size();
        blob_result args[1] = { string_blob_result(name) };
        return mng->invoke_callback(id(), "$"sv, args);
        /*
            [](void* cookie, blob_result* r, uint32_t count) {
                blob_result * result = reinterpret_cast<blob_result*>(cookie);
                if (count == 1) {
                    *result = *r;
                    blob_result_allocate(result);
                } else if (count > 0) {
                    *result = make_blob_result( blob_type::tuple, r, static_cast<uint32_t>(count * sizeof(blob_result)));
                    blob_result_allocate(result);
                }
            }
        */
    }
    return {};
}

void view_model::set_method(std::string_view name, std::string_view propname, blob_result val)
{
    if (auto mng = get_manager(); mng) {
        //GLOBAL_LOG_INFO() << "view_model::do_call_method " << name << ", argscount: " << args.size();
        blob_result args[3] = { string_blob_result(name), string_blob_result(propname), val };
        mng->invoke_callback(id(), "="sv, args);
    }
}

std::string_view view_model::echo_method(std::string_view arg) const
{
    return arg;
}

int view_model::on_change(status_type st, std::initializer_list<const blob_result> args)
{
    if (auto mng = get_manager(); mng) {
        return mng->on_change_callback(id(), st, std::span{args});
    }
    return 1;
}

/*
void view_model::push_event(uint32_t request_id, response_item itm)
{
    unique_lock lck(ev_mtx_);
    ev_resps_.insert(std::pair{ request_id, std::move(itm) });
    ev_cv_.notify_all();
}

bool view_model::wait_for_response(uint32_t request_id, response_item& resp)
{
    unique_lock lck(ev_mtx_);
    ev_cv_.wait(lck, [this, request_id, &resp]() {
        if (cancel_reqs_.load()) throw cancel_exception();
        auto it = ev_resps_.find(request_id);
        if (it != ev_resps_.end()) {
            resp = std::move(it->second);
            ev_resps_.erase(it);
            return true;
        }
        return false;
    });
    return true;
}
*/

void view_model::async_cancel()
{
    do_post(&view_model::cancel);
}

void view_model::cancel()
{
    do_serialised_op([this](bool need_cancel) {
        GLOBAL_LOG_INFO() << "cancel operations";
    });
}

void view_model::final_cancel()
{
    ++cancel_reqs_;
    th_.cancel();
    th_ = {};
    ev_cv_.notify_all();
}

}
