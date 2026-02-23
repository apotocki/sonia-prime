//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <unordered_map>

#include "sonia/java/jni_ref.hpp"

#include "sonia/singleton.hpp"
#include "sonia/concurrency.hpp"

#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

class jni_invoker : public singleton
{
    JavaVM* jvm;
    jmethodID invoke_;
    jmethodID invoke_set_;
    jmethodID invoke_get_;
    // com.sonia.invocation.CallbackBean fields
    jfieldID cbcl_id_fld;
    jfieldID cbcl_invId_fld;
    jfieldID cbcl_name_fld;
    jfieldID cbcl_arguments_fld;

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
        jint invid;
        string_view name;
        sonia::span<const blob_result> args;
        shared_ptr<sync_t> sync;
    };

    std::list<caller_bean> caller_queue;
    threads::mutex caller_queue_mtx;
    threads::condition_variable cvar;

    std::unordered_map<jlong, caller_bean> results;
    threads::mutex results_mtx;

public:
    unique_jni_ref<jclass, global_ref_kind> obj_cls;
    unique_jni_ref<jclass, global_ref_kind> invocable_cls;
    unique_jni_ref<jclass, global_ref_kind> invocable_registry_cls;
    unique_jni_ref<jclass, global_ref_kind> cbcl;

    explicit jni_invoker(JNIEnv* penv);

    smart_blob invoke(jint invid, string_view name, sonia::span<const blob_result> args);
    void set_property(jint invid, string_view name, blob_result const& value);
    smart_blob get_property(jint invid, string_view name);

    void poll(JNIEnv* penv, jobject cb) noexcept;
    void push_result(JNIEnv* penv, jlong cbid, jobject result) noexcept;

    unique_jni_ref<jobjectArray> encode_arguments(jni_env& env, sonia::span<const blob_result> args);
};

}
