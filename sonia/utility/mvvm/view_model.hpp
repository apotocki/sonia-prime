//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <atomic>
#include <boost/unordered_map.hpp>
#include <boost/variant.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/span.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/invokation/invokable.hpp"
#include "sonia/services/scheduler/scheduler.hpp"

namespace sonia {

/*
struct response_item
{
    uint16_t buttons;
    std::vector<small_string> args;

    bool has_button(button_type b) const { return !!(buttons & static_cast<uint16_t>(b)); }
};
*/

enum class status_type : int16_t {
    INVALIDATED_ST = 0,
    OBSOLETED_ST = 1,
    READY_ST = 2,
    DATA_CHANGED_ST = 3,
    PROPERTY_CHANGED_ST = 4,
    ASK_ST = 5,
    START_OP = 6,
    FINISH_OP = 7,
    //CLOSE_FORM_OP    = 5,

    CANCEL_ST = -1,
    CONTENT_ERROR_ST = -2,
    OP_ERROR_ST = -3
};

using cancel_flag_type = std::atomic<int>;

class view_model 
    : public invokation::invokable
    , public invokation::registrar<view_model>
{
    friend class invokation::registrar<view_model>;

public:
    class manager
    {
    public:
        virtual ~manager() = default;
        virtual scheduler& get_scheduler() const = 0;

        virtual void invoke_callback(void* cookie, int32_t cvid, string_view method_name, span<const blob_result>, on_invoke_cv_result_setter setter) = 0;
        virtual int on_change_callback(int32_t cvid, status_type, std::span<const blob_result> args) = 0;

        virtual int32_t create_content_view(string_view) = 0;
        virtual int32_t push_content_view(shared_ptr<view_model>) = 0;
        virtual shared_ptr<view_model> get_content_view(int32_t id) = 0;
        virtual void free_view_model(int32_t) = 0;
    };

    explicit view_model(int32_t idval = 0);

    view_model(view_model const&) = delete;

    virtual ~view_model() = default;

    virtual void release() { if (auto mng = get_manager(); mng) mng->free_view_model(id()); }

    virtual shared_ptr<view_model> self_as_content_view_shared() = 0;

    void set(int32_t idval);

    int32_t id() const { return id_; }

    // properties
    //smart_blob get_property(string_view propname) const override;
    //void set_property(string_view propname, blob_result const& val) override;

    template<class Archive>
    void serialize_properties(Archive& ar, std::initializer_list<cstring_view> l);

    // events
    //template <typename ... ArgsT>
    //response_item ask_request(int16_t btns, message_type mt, ArgsT const& ... args);

    //void push_event(uint32_t request_id, response_item);
    //bool wait_for_response(uint32_t request_id, response_item& resp);

    void async_cancel();
    void cancel();

    void final_cancel();

    int on_change(status_type st, std::initializer_list<const blob_result> args);

    template <typename F>
    void post(F&& f);

    template <typename RT, typename T, typename ... FArgsT, typename ... ArgsT>
    void do_post(RT(T::* f)(FArgsT ...), ArgsT && ... args);

    template <typename F>
    void do_serialised_op(F&& f);

    template <typename F>
    void do_serialised_cmd(F&& f);

    virtual shared_ptr<manager> get_manager() const = 0;
    virtual void set(shared_ptr<manager>) = 0;

    // properties routine
    void on_property_change(string_view propname) override;
    
    blob_result call_method(string_view name, blob_result args) const;
    blob_result do_call_method(string_view name, span<const blob_result> args) const;
    blob_result do_call_method(string_view name, std::initializer_list<const blob_result> args) const
    { return do_call_method(name, span{args}); }

    blob_result get_method(string_view name) const;
    void set_method(string_view name, string_view propname, blob_result val);

    // test method
    string_view echo_method(string_view arg) const;

protected:
    // invokation routine
    void do_registration(registrar_type &);

protected:
    fibers::mutex ev_mtx_;
    fibers::condition_variable ev_cv_;
    //boost::unordered_map<uint32_t, response_item> ev_resps_;

    //mutable fibers::mutex props_mtx_;
    //using fn_property_holder = automatic_polymorphic<invokation::fn_property, 4 * sizeof(void*)>;
    //boost::unordered_map<std::string, boost::variant<blob_result, fn_property_holder>, hasher> properties_;

    //  + sizeof(function<void()>)
    //using method_holder = automatic_polymorphic<method, sizeof(void*)>;

    //mutable fibers::mutex methods_mtx_;
    //boost::unordered_map<std::string, method_holder, hasher> methods_;

    scheduler_task_handle th_;
    fibers::mutex opmtx_;
    cancel_flag_type cancel_reqs_ {0};

    uint64_t locked_ : 1;

    int32_t id_;
};

}
