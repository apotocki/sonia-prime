//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <atomic>
#include <unordered_map>

#include "sonia/java/jni_ref.hpp"

#include "sonia/singleton.hpp"
#include "sonia/concurrency.hpp"

#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

// Calls into Java Invocable/Callable objects referenced by JNI global (or weak global) references,
// see jni_invocable_proxy/jni_callable_proxy. Calls made on a thread that is not attached to the JVM
// are marshalled to the Java callback thread (com.sonia.invocation.CallbackBean.callbackProc).
class jni_invoker : public singleton
{
    JavaVM* jvm;
    jmethodID invocable_invoke_;
    jmethodID invocable_set_property_;
    jmethodID invocable_get_property_;
    jmethodID invocable_has_method_;
    jmethodID callable_invoke_;
    // com.sonia.invocation.CallbackBean fields
    jfieldID cbcl_id_fld;
    jfieldID cbcl_target_fld;
    jfieldID cbcl_name_fld;
    jfieldID cbcl_arguments_fld;

    // a warning is logged each time the number of live references held by proxies crosses a multiple of this value
    static constexpr int live_refs_warning_step = 5000;
    std::atomic<int> live_refs_{ 0 };

	struct sync_t
	{
		threads::mutex mtx;
		threads::condition_variable var;
		smart_blob result;

		int progress = 0;

		void wait_for_start_invokation();
		void wait_for_result();
        void set_start_invokation_stage();
        void set_result_stage();
	};

    struct caller_bean
    {
        jobject target; // global or weak global reference, owned by the calling proxy
        bool is_callable;
        string_view name;
        sonia::span<const blob_result> args;
        shared_ptr<sync_t> sync;
    };

    std::list<caller_bean> caller_queue;
    threads::mutex caller_queue_mtx;
    threads::condition_variable cvar;

    std::unordered_map<jlong, caller_bean> results;
    threads::mutex results_mtx;

    smart_blob enqueue(caller_bean&&);

public:
    unique_jni_ref<jclass, global_ref_kind> obj_cls;
    unique_jni_ref<jclass, global_ref_kind> throwable_cls;
    unique_jni_ref<jclass, global_ref_kind> invocable_cls;
    unique_jni_ref<jclass, global_ref_kind> callable_cls;
    unique_jni_ref<jclass, global_ref_kind> weak_invocable_cls;
    unique_jni_ref<jclass, global_ref_kind> native_invocable_cls;
    unique_jni_ref<jclass, global_ref_kind> native_callable_cls;
    unique_jni_ref<jclass, global_ref_kind> cbcl;

    explicit jni_invoker(JNIEnv* penv);

    // reference management for proxies; delete_ref may be called on any thread
    jobject new_ref(JNIEnv* penv, jobject obj, bool weak);
    void delete_ref(jobject ref, bool weak) noexcept;

    smart_blob invoke(jobject target, string_view name, sonia::span<const blob_result> args);
    smart_blob call_invoke(jobject target, sonia::span<const blob_result> args);
    void set_property(jobject target, string_view name, blob_result const& value);
    smart_blob get_property(jobject target, string_view name);
    bool has_method(jobject target, string_view name);

    void poll(JNIEnv* penv, jobject cb) noexcept;
    void push_result(JNIEnv* penv, jlong cbid, jobject result) noexcept;

    unique_jni_ref<jobjectArray> encode_arguments(jni_env& env, sonia::span<const blob_result> args);
};

}
