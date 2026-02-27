//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//

#include "sonia/config.hpp"
#include "jni_invoker.hpp"

#include "sonia/java/jni_env.hpp"
#include "sonia/utility/invocation/jni_encoder.hpp"
#include "sonia/utility/invocation/jni_decoder.hpp"

namespace sonia::invocation {

inline void jni_invoker::sync_t::wait_for_start_invokation()
{
	unique_lock lck(mtx);
	var.wait(lck, [this] { return !!progress; });
}

inline void jni_invoker::sync_t::wait_for_result()
{
	unique_lock lck(mtx);
	var.wait(lck, [this] { return progress == 2; });
}

inline void jni_invoker::sync_t::set_start_invokation_stage()
{
	lock_guard guard(mtx);
	progress = 1;
	var.notify_one();
}

inline void jni_invoker::sync_t::set_result_stage()
{
	lock_guard guard(mtx);
	progress = 2;
	var.notify_one();
}

jni_invoker::jni_invoker(JNIEnv* penv)
{
    if (penv->GetJavaVM(&jvm)) [[unlikely]] {
        throw jni_error("can't retrieve jvm");
    }

    jni_env env{ penv };
    obj_cls = env.get_class("java/lang/Object");
    cbcl = env.get_class("com/sonia/invocation/CallbackBean");

	invocable_cls = env.get_class("com/sonia/invocation/Invocable");
    invocable_registry_cls = env.get_class("com/sonia/invocation/InvocableRegistry");
    invoke_ = env.get_static_jmethod(*invocable_registry_cls, "invoke", "(ILjava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;");
	invoke_set_ = env.get_static_jmethod(*invocable_registry_cls, "setProperty", "(ILjava/lang/String;Ljava/lang/Object;)V");
	invoke_get_ = env.get_static_jmethod(*invocable_registry_cls, "getProperty", "(ILjava/lang/String;)Ljava/lang/Object;");

    cbcl_id_fld = env.get_jfield(*cbcl, "id", "J");
    cbcl_invId_fld = env.get_jfield(*cbcl, "invId", "I");
    cbcl_name_fld = env.get_jfield(*cbcl, "name", "Ljava/lang/String;");
    cbcl_arguments_fld = env.get_jfield(*cbcl, "arguments", "[Ljava/lang/Object;");
}

smart_blob jni_invoker::invoke(jint invid, string_view name, sonia::span<const blob_result> args)
{
    JNIEnv* penv;
    int res = jvm->GetEnv((void**)&penv, JNI_VERSION_1_6);
    if (res == JNI_OK) {
		GLOBAL_LOG_INFO() << "jni_invoker::invoke " << name;
        jni_env env{ penv };
		auto jname = env.new_string(name);
		auto jargs = encode_arguments(env, args);
		auto res = env.invoke<jobject>(invocable_registry_cls, nullptr, invoke_, invid, jname.detach(), (jobject)jargs.detach());
		blob_result br = sonia::invocation::jni_decoder::decode(env.get(), *res);
		GLOBAL_LOG_INFO() << "jni_invoker::invoke " << name << ", result: " << br;
		return smart_blob{ std::move(br) };
    }
	auto s = make_shared<sync_t>();
	{
		lock_guard guard(caller_queue_mtx);
		caller_queue.emplace_back(caller_bean{ .invid = invid, .name = name, .args = args, .sync = s });
		cvar.notify_one();
	}

	// wait for to java types encoding
	s->wait_for_start_invokation();
	s->wait_for_result();
	blob_result br = s->result.detach();
	return smart_blob{ std::move(br) };
}

void jni_invoker::set_property(jint invid, string_view name, blob_result const& value)
{
	JNIEnv* penv;
	int res = jvm->GetEnv((void**)&penv, JNI_VERSION_1_6);
	if (res == JNI_OK) {
		GLOBAL_LOG_INFO() << "jni_invoker::set_property " << name << ", with value: " << value;
		jni_env env{ penv };
		auto jname = env.new_string(name);
		jobject arg = jni_encoder::encode(env.get(), value);
		env.invoke<void>(invocable_registry_cls, nullptr, invoke_set_, invid, jname.detach(), arg);
		if (arg) {
			env->DeleteLocalRef(arg);
		}
		return;
	}
    THROW_NOT_IMPLEMENTED_ERROR("jni_invoker::set_property is not implemented for non-JNI threads");
}

smart_blob jni_invoker::get_property(jint invid, string_view name)
{
	JNIEnv* penv;
	int res = jvm->GetEnv((void**)&penv, JNI_VERSION_1_6);
	if (res == JNI_OK) {
		GLOBAL_LOG_INFO() << "jni_invoker::get_property " << name;
		jni_env env{ penv };
		auto jname = env.new_string(name);
		auto res = env.invoke<jobject>(invocable_registry_cls, nullptr, invoke_get_, invid, jname.detach());
		blob_result br = jni_decoder::decode(env.get(), *res);
		GLOBAL_LOG_INFO() << "jni_invoker::get_property " << name << ", result: " << br;
		return smart_blob{ std::move(br) };
	}
	THROW_NOT_IMPLEMENTED_ERROR("jni_invoker::get_property is not implemented for non-JNI threads");
}

unique_jni_ref<jobjectArray> jni_invoker::encode_arguments(jni_env & env, sonia::span<const blob_result> args)
{
	unique_jni_ref<jobjectArray> jobjarr;
	if (!args.empty()) {
		jobjarr = env.new_object_array(static_cast<jsize>(args.size()), *obj_cls, nullptr);
	}
	jsize index = 0;
	for (blob_result const& br : args) {
		//GLOBAL_LOG_INFO() << "argument: " << br;
		jobject arg = jni_encoder::encode(env.get(), br);
		if (arg) {
			env->SetObjectArrayElement(*jobjarr, index, arg);
			env->DeleteLocalRef(arg);
		}
		++index;
	}
	return jobjarr;
}

void jni_invoker::poll(JNIEnv* penv, jobject cb) noexcept
{
	jni_env env(penv);
	optional<caller_bean> cbean;
	{
		//GLOBAL_LOG_INFO() << "poll: waiting for invoke request";
		std::unique_lock lck{ caller_queue_mtx };
		cvar.wait(lck, [this] { return !caller_queue.empty(); });
		GLOBAL_LOG_INFO() << "poll: got an invoke request";
		cbean.emplace(std::move(caller_queue.front()));
		caller_queue.pop_front();
	}
	auto jname = env.new_string(cbean->name);
	auto jargs = encode_arguments(env, cbean->args);
	//GLOBAL_LOG_INFO() << "poll: before set_start_invokation_stage";
	cbean->sync->set_start_invokation_stage();
	//GLOBAL_LOG_INFO() << "poll: after set_start_invokation_stage";
	env.set_field_value(cb, cbcl_invId_fld, cbean->invid);
	env.set_field_value(cb, cbcl_name_fld, jname.detach());
	env.set_field_value(cb, cbcl_arguments_fld, jargs.detach());
	
	jlong id = env.get_field_value<jlong>(cb, cbcl_id_fld);
	lock_guard guard(results_mtx);
	results[id] = std::move(*cbean);
}

void jni_invoker::push_result(JNIEnv* penv, jlong cbid, jobject result) noexcept
{
	optional<caller_bean> cbean;
	{
		lock_guard guard(results_mtx);
		auto it = results.find(cbid);
		if (it == results.end()) return;
		cbean = std::move(it->second);
		results.erase(it);
	}
	cbean->sync->result = sonia::invocation::jni_decoder::decode(penv, result);
	cbean->sync->set_result_stage();
}

extern "C" JNIEXPORT void JNICALL Java_com_sonia_invocation_InvocableRegistry_pollCallback(JNIEnv* penv, jclass, jobject cb)
{
	//GLOBAL_LOG_INFO() << "in pollCallback";
	as_singleton<invocation::jni_invoker>(penv)->poll(penv, cb);
	//GLOBAL_LOG_INFO() << "out of pollCallback";
}

extern "C" JNIEXPORT void JNICALL Java_com_sonia_invocation_InvocableRegistry_callbackResult(JNIEnv* penv, jclass, jlong cbid, jobject result)
{
	//GLOBAL_LOG_INFO() << "in callbackResult";
	as_singleton<invocation::jni_invoker>(penv)->push_result(penv, cbid, result);
	//GLOBAL_LOG_INFO() << "out callbackResult";
}

}
