//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//

#include "sonia/config.hpp"
#include "jni_invoker.hpp"
#include "sonia/logger/logger.hpp"
#include "sonia/java/jni_env.hpp"
#include "sonia/utility/invocation/jni_encoder.hpp"
#include "sonia/utility/invocation/jni_decoder.hpp"

#include <boost/exception/diagnostic_information.hpp>

namespace sonia::invocation {

namespace {

JNIEnv* attached_env(JavaVM* jvm) noexcept
{
	JNIEnv* penv = nullptr;
	return jvm->GetEnv((void**)&penv, JNI_VERSION_1_6) == JNI_OK ? penv : nullptr;
}

// A weak global reference must be promoted to a strong local one before use;
// for a strong global reference this is just an extra local reference.
class target_lock
{
	jni_env& env_;
	jobject obj_;

public:
	target_lock(jni_env& env, jobject target)
		: env_{ env }, obj_{ env->NewLocalRef(target) }
	{
		if (!obj_) {
			throw exception("jni_invoker: the target Java object has been collected");
		}
	}

	target_lock(target_lock const&) = delete;
	target_lock& operator=(target_lock const&) = delete;

	~target_lock() { env_->DeleteLocalRef(obj_); }

	inline jobject get() const noexcept { return obj_; }
};

}

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
    throwable_cls = env.get_class("java/lang/Throwable");
    cbcl = env.get_class("com/sonia/invocation/CallbackBean");

	invocable_cls = env.get_class("com/sonia/invocation/Invocable");
    callable_cls = env.get_class("com/sonia/invocation/Callable");
    weak_invocable_cls = env.get_class("com/sonia/invocation/WeakInvocable");
    native_invocable_cls = env.get_class("com/sonia/invocation/NativeInvocable");
    native_callable_cls = env.get_class("com/sonia/invocation/NativeCallable");

	invocable_invoke_ = env.get_jmethod(*invocable_cls, "invoke", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;");
	invocable_set_property_ = env.get_jmethod(*invocable_cls, "setProperty", "(Ljava/lang/String;Ljava/lang/Object;)V");
	invocable_get_property_ = env.get_jmethod(*invocable_cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
	invocable_has_method_ = env.get_jmethod(*invocable_cls, "hasMethod", "(Ljava/lang/String;)Z");
    callable_invoke_ = env.get_jmethod(*callable_cls, "invoke", "([Ljava/lang/Object;)Ljava/lang/Object;");

    cbcl_id_fld = env.get_jfield(*cbcl, "id", "J");
    cbcl_target_fld = env.get_jfield(*cbcl, "target", "Ljava/lang/Object;");
    cbcl_name_fld = env.get_jfield(*cbcl, "name", "Ljava/lang/String;");
    cbcl_arguments_fld = env.get_jfield(*cbcl, "arguments", "[Ljava/lang/Object;");
}

jobject jni_invoker::new_ref(JNIEnv* penv, jobject obj, bool weak)
{
	jobject ref = weak ? penv->NewWeakGlobalRef(obj) : penv->NewGlobalRef(obj);
	if (!ref) {
		penv->ExceptionClear();
		throw exception("jni_invoker: can't create a global reference");
	}
	int count = live_refs_.fetch_add(1, std::memory_order_relaxed) + 1;
	if (count % live_refs_warning_step == 0) {
		GLOBAL_LOG_WARN() << "jni_invoker: " << count << " live Java object references are held by native proxies, possible leak";
	}
	return ref;
}

void jni_invoker::delete_ref(jobject ref, bool weak) noexcept
{
	if (!ref) return;
	JNIEnv* penv = nullptr;
	bool attached = false;
	jint res = jvm->GetEnv((void**)&penv, JNI_VERSION_1_6);
	if (res == JNI_EDETACHED) {
		if (jvm->AttachCurrentThread(&penv, nullptr) != JNI_OK) {
			GLOBAL_LOG_ERROR() << "jni_invoker: can't attach the thread to release a Java object reference";
			return;
		}
		attached = true;
	} else if (res != JNI_OK) {
		GLOBAL_LOG_ERROR() << "jni_invoker: can't get JNI environment to release a Java object reference";
		return;
	}

	if (weak) {
		penv->DeleteWeakGlobalRef(ref);
	} else {
		penv->DeleteGlobalRef(ref);
	}
	live_refs_.fetch_sub(1, std::memory_order_relaxed);

	if (attached) {
		jvm->DetachCurrentThread();
	}
}

smart_blob jni_invoker::enqueue(caller_bean&& bean)
{
	shared_ptr<sync_t> s = bean.sync;
	{
		lock_guard guard(caller_queue_mtx);
		caller_queue.emplace_back(std::move(bean));
		cvar.notify_one();
	}

	// wait for to java types encoding
	s->wait_for_start_invokation();
	s->wait_for_result();
	blob_result br = s->result.detach();
	return smart_blob{ std::move(br) };
}

smart_blob jni_invoker::invoke(jobject target, string_view name, sonia::span<const blob_result> args)
{
	if (JNIEnv* penv = attached_env(jvm)) {
		//GLOBAL_LOG_INFO() << "jni_invoker::invoke " << name;
		jni_env env{ penv };
		target_lock obj{ env, target };
		auto jname = env.new_string(name);
		auto jargs = encode_arguments(env, args);
		auto res = env.invoke<jobject>(nullptr, obj.get(), invocable_invoke_, *jname, *jargs);
		blob_result br = jni_decoder::decode(penv, *res);
		//GLOBAL_LOG_INFO() << "jni_invoker::invoke " << name << ", result: " << br;
		return smart_blob{ std::move(br) };
	}
	return enqueue(caller_bean{ .target = target, .is_callable = false, .name = name, .args = args, .sync = make_shared<sync_t>() });
}

smart_blob jni_invoker::call_invoke(jobject target, sonia::span<const blob_result> args)
{
	if (JNIEnv* penv = attached_env(jvm)) {
		jni_env env{ penv };
		target_lock obj{ env, target };
		auto jargs = encode_arguments(env, args);
		auto res = env.invoke<jobject>(nullptr, obj.get(), callable_invoke_, *jargs);
		blob_result br = jni_decoder::decode(penv, *res);
		return smart_blob{ std::move(br) };
	}
	return enqueue(caller_bean{ .target = target, .is_callable = true, .name = {}, .args = args, .sync = make_shared<sync_t>() });
}

void jni_invoker::set_property(jobject target, string_view name, blob_result const& value)
{
	JNIEnv* penv = attached_env(jvm);
	if (!penv) {
		THROW_NOT_IMPLEMENTED_ERROR("jni_invoker::set_property is not implemented for non-JNI threads");
	}
	//GLOBAL_LOG_INFO() << "jni_invoker::set_property " << name << ", with value: " << value;
	jni_env env{ penv };
	target_lock obj{ env, target };
	auto jname = env.new_string(name);
	jobject arg = jni_encoder::encode(penv, value);
	try {
		env.invoke<void>(nullptr, obj.get(), invocable_set_property_, *jname, arg);
	} catch (...) {
		if (arg) env->DeleteLocalRef(arg);
		throw;
	}
	if (arg) env->DeleteLocalRef(arg);
}

smart_blob jni_invoker::get_property(jobject target, string_view name)
{
	JNIEnv* penv = attached_env(jvm);
	if (!penv) {
		THROW_NOT_IMPLEMENTED_ERROR("jni_invoker::get_property is not implemented for non-JNI threads");
	}
	jni_env env{ penv };
	target_lock obj{ env, target };
	auto jname = env.new_string(name);
	auto res = env.invoke<jobject>(nullptr, obj.get(), invocable_get_property_, *jname);
	blob_result br = jni_decoder::decode(penv, *res);
	//GLOBAL_LOG_INFO() << "jni_invoker::get_property " << name << ", result: " << br;
	return smart_blob{ std::move(br) };
}

bool jni_invoker::has_method(jobject target, string_view name)
{
	JNIEnv* penv = attached_env(jvm);
	if (!penv) {
		THROW_NOT_IMPLEMENTED_ERROR("jni_invoker::has_method is not implemented for non-JNI threads");
	}
	jni_env env{ penv };
	target_lock obj{ env, target };
	auto jname = env.new_string(name);
	return !!env.invoke<jboolean>(nullptr, obj.get(), invocable_has_method_, *jname);
}

unique_jni_ref<jobjectArray> jni_invoker::encode_arguments(jni_env & env, sonia::span<const blob_result> args)
{
	// always a real array, even for no arguments: Java implementations index into it without null checks
	unique_jni_ref<jobjectArray> jobjarr = env.new_object_array(static_cast<jsize>(args.size()), *obj_cls, nullptr);
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
	// null for a collected weakly referenced target, CallbackBean reports it as an error result
	jobject target = env->NewLocalRef(cbean->target);
	jstring jname = cbean->is_callable ? nullptr : env.new_string(cbean->name).detach();
	auto jargs = encode_arguments(env, cbean->args);
	//GLOBAL_LOG_INFO() << "poll: before set_start_invokation_stage";
	cbean->sync->set_start_invokation_stage();
	//GLOBAL_LOG_INFO() << "poll: after set_start_invokation_stage";
	env.set_field_value(cb, cbcl_target_fld, target);
	env.set_field_value(cb, cbcl_name_fld, jname);
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
	// the caller must be released in any case, an undecodable result becomes an error
	try {
		cbean->sync->result = sonia::invocation::jni_decoder::decode(penv, result);
	} catch (...) {
		cbean->sync->result = smart_blob{ error_blob_result(boost::current_exception_diagnostic_information(), true) };
	}
	cbean->sync->set_result_stage();
}

extern "C" JNIEXPORT void JNICALL Java_com_sonia_invocation_CallbackBean_pollCallback(JNIEnv* penv, jclass, jobject cb)
{
	//GLOBAL_LOG_INFO() << "in pollCallback";
	as_singleton<invocation::jni_invoker>(penv)->poll(penv, cb);
	//GLOBAL_LOG_INFO() << "out of pollCallback";
}

extern "C" JNIEXPORT void JNICALL Java_com_sonia_invocation_CallbackBean_callbackResult(JNIEnv* penv, jclass, jlong cbid, jobject result)
{
	//GLOBAL_LOG_INFO() << "in callbackResult";
	as_singleton<invocation::jni_invoker>(penv)->push_result(penv, cbid, result);
	//GLOBAL_LOG_INFO() << "out callbackResult";
}

}
