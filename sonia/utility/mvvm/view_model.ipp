//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "view_model.hpp"
#include <boost/serialization/nvp.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/invocation/serialization.hpp"

namespace sonia {

template <typename F>
void view_model::post(F&& f)
{
    th_.cancel();
    if (auto mng = get_manager(); mng) {
        mng->get_scheduler().post(std::forward<F>(f));
    }
}

template <typename RT, typename T, typename ... FArgsT, typename ... ArgsT>
void view_model::do_post(RT(T::* f)(FArgsT ...), ArgsT && ... args)
{
    th_.cancel();
    if (auto mng = get_manager(); mng) {
        mng->get_scheduler().post([self = shared_self(), f, tpl = std::tuple(std::forward<ArgsT>(args) ...)] {
            std::apply(f, std::tuple_cat(std::tuple{dynamic_pointer_cast<T>(self)}, std::move(tpl)));
        });
    }
}

template <typename F>
void view_model::do_serialised_op(F&& f)
{
    bool has_cancel_req = false;
    if (!opmtx_.try_lock()) {
        ++cancel_reqs_;
        ev_cv_.notify_all();
        has_cancel_req = true;
        opmtx_.lock();
    }
    SCOPE_EXIT([this] { opmtx_.unlock(); });
    try {
        f(has_cancel_req && cancel_reqs_.fetch_sub(1) > 1);
    } catch (cancel_exception const& ex) {
        // do nothing
        GLOBAL_LOG_INFO() << "cancel_exception: " << ex.what();
    } catch (...) {
        GLOBAL_LOG_ERROR() << boost::current_exception_diagnostic_information();
    }
}

template <typename F>
void view_model::do_serialised_cmd(F&& cmd)
{
    do_serialised_op([this, &cmd](bool need_cancel) {
        if (need_cancel) return;
        // start operation
        on_change(status_type::START_OP, {});
        try {
            cmd();
        } catch (std::exception const& e) {
            on_change(status_type::OP_ERROR_ST, {string_blob_result(string_view(e.what()))});
            return;
        }
        on_change(status_type::FINISH_OP, {});
    });
}

template<class Archive>
void view_model::serialize_properties(Archive& ar, std::initializer_list<cstring_view> l)
{
    using namespace boost::serialization;
    if constexpr (Archive::is_saving::value) {
        for (cstring_view key : l) {
            ar & make_nvp(key.c_str(), *get_property(key));
        }
    } else {
        //blob_result_serialization_helper& hlp =
        //    ar.template get_helper<blob_result_serialization_helper>(blob_result_serialization_helper::id);
        for (cstring_view key : l) {
            blob_result b = nil_blob_result();
            ar & make_nvp(key.c_str(), b);
            set_property(key, std::move(b));
        }
    }
}

}
