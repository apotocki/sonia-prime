//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "jni_decoder.hpp"
#include "sonia/java/jni_env.hpp"
#include "sonia/java/jni_ref.hpp"
//#include "jni_encoder.hpp"
#include "jni_invoker.hpp"
#include "jni_proxy.hpp"
#include "jni_native_handle.hpp"

namespace sonia::invocation {

jni_object_ref::jni_object_ref(jni_invoker& inv, JNIEnv* penv, jobject obj, bool weak)
    : invoker_{ &inv }
    , ref_{ inv.new_ref(penv, obj, weak) }
    , weak_{ weak }
{}

jni_object_ref::~jni_object_ref()
{
    invoker_->delete_ref(ref_, weak_);
}

smart_blob jni_callable_proxy::invoke(span<const blob_result> args)
{
    return obj_.invoker().call_invoke(obj_.get(), args);
}

shared_ptr<invocable> jni_invocable_proxy::self_as_invocable_shared()
{
    return shared_from_this();
}

bool jni_invocable_proxy::has_method(string_view methodname) const
{
    try {
        return obj_.invoker().has_method(obj_.get(), methodname);
    } catch (...) {
        return false;
    }
}

bool jni_invocable_proxy::try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept
{
    try {
        result = obj_.invoker().invoke(obj_.get(), methodname, args);
        return true;
    } catch (...) {
        return false;
    }
}

bool jni_invocable_proxy::try_set_property(string_view propname, blob_result const& val)
{
    // GLOBAL_LOG_INFO() << "setting property '" << propname << "' with value: " << val;
    try {
        obj_.invoker().set_property(obj_.get(), propname, val);
        return true;
    }
    catch (...) {
        GLOBAL_LOG_ERROR() << "failed to set property '" << propname << "', error: " << boost::current_exception_diagnostic_information();
        return false;
    }
}

bool jni_invocable_proxy::try_get_property(string_view propname, smart_blob& result) const
{
    try {
        result = obj_.invoker().get_property(obj_.get(), propname);
        GLOBAL_LOG_INFO() << "got property '" << propname << "' with value: " << result;
        return true;
    } catch (...) {
        GLOBAL_LOG_ERROR() << "failed to get property '" << propname << "', error: " << boost::current_exception_diagnostic_information();
        return false;
    }
}

blob_result jni_decoder::decode(JNIEnv* penv, jobject obj)
{
    return as_singleton<jni_decoder>(penv)->do_decode(penv, obj);
}

blob_result jni_decoder::do_decode(JNIEnv* penv, jobject obj) const
{
    if (!obj) return nil_blob_result();
    
    jni_env env{ penv };
    auto objcls = env.get_class(obj);
    auto jclsname = env.get_class_name(*objcls);
    auto chars = env.get_string_utf_chars(*jclsname);

    if (auto it = type_handlers_.find(string_view{ chars.c_str() }); it != type_handlers_.end()) {
        return it->second(this, penv, obj);
    }
    for (auto const& ph : polymorphic_type_handlers_) {
        if (env->IsInstanceOf(obj, ph.first)) {
            return ph.second(this, penv, obj);
        }
    }
    throw exception("jni_decoder::decode: unknown type: %1%"_fmt % chars.c_str());
}

jni_decoder::jni_decoder(JNIEnv* penv)
{
    jni_env env{ penv };
    
    jni_invoker& inv = *as_singleton<jni_invoker>(penv);

    {
        auto boolean_cls = env.get_class(jni_traits<jboolean>::class_name);
        boolean_booleanValue_ = env.get_jmethod(*boolean_cls, "booleanValue", "()Z");
    }
    {
        auto byte_cls = env.get_class(jni_traits<jshort>::class_name);
        byte_byteValue_ = env.get_jmethod(*byte_cls, "byteValue", "()B");
    }
    {
        auto short_cls = env.get_class(jni_traits<jshort>::class_name);
        short_shortValue_ = env.get_jmethod(*short_cls, "shortValue", "()S");
    }
    {
        auto int_cls = env.get_class(jni_traits<jint>::class_name);
        int_intValue_ = env.get_jmethod(*int_cls, "intValue", "()I");
    }
    {
        auto long_cls = env.get_class(jni_traits<jlong>::class_name);
        long_longValue_ = env.get_jmethod(*long_cls, "longValue", "()J");
    }
    {
        auto float_cls = env.get_class(jni_traits<jfloat>::class_name);
        float_floatValue_ = env.get_jmethod(*float_cls, "floatValue", "()F");
    }
    {
        auto double_cls = env.get_class(jni_traits<jdouble>::class_name);
        double_doubleValue_ = env.get_jmethod(*double_cls, "doubleValue", "()D");
    }
    weak_invocable_target_fld_ = env.get_jfield(*inv.weak_invocable_cls, "target", "Lcom/sonia/invocation/Invocable;");

    type_handlers_["java.lang.String"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        auto chars = env.get_string_utf_chars((jstring)obj);
        blob_result result = string_blob_result(chars.c_str());
        blob_result_allocate(&result);
        return result;
    };
    type_handlers_["java.lang.Boolean"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jboolean value = env.invoke<jboolean>(nullptr, obj, p->boolean_booleanValue_);
        return bool_blob_result(value);
    };
    type_handlers_["java.lang.Byte"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jbyte value = env.invoke<jbyte>(nullptr, obj, p->byte_byteValue_);
        return i8_blob_result(value);
    };
    type_handlers_["java.lang.Short"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jshort value = env.invoke<jshort>(nullptr, obj, p->short_shortValue_);
        return i16_blob_result(value);
    };
    type_handlers_["java.lang.Integer"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jint value = env.invoke<jint>(nullptr, obj, p->int_intValue_);
        return i32_blob_result(value);
    };
    type_handlers_["java.lang.Long"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jlong value = env.invoke<jlong>(nullptr, obj, p->long_longValue_);
        return i64_blob_result(value);
    };
    type_handlers_["java.lang.Float"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jfloat value = env.invoke<jfloat>(nullptr, obj, p->float_floatValue_);
        return f32_blob_result(value);
    };
    type_handlers_["java.lang.Double"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jni_env env{ penv };
        jdouble value = env.invoke<jdouble>(nullptr, obj, p->double_doubleValue_);
        return f64_blob_result(value);
    };

    // an Invocable that native should reference weakly, see com.sonia.invocation.WeakInvocable
    type_handlers_["com.sonia.invocation.WeakInvocable"] = [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
        if (!obj) return nil_blob_result();
        jobject target = penv->GetObjectField(obj, p->weak_invocable_target_fld_);
        if (!target) return nil_blob_result();
        using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
        blob_result result = object_blob_result<wrapper_object_t>(make_shared<jni_invocable_proxy>(*as_singleton<jni_invoker>(penv), penv, target, true));
        penv->DeleteLocalRef(target);
        return result;
    };

    get_native_handle_ = env.get_jmethod(*inv.native_invocable_cls, "getHandle", "()J"); // declared in NativeObject

    // must precede the generic Invocable/Callable handlers below: NativeInvocable/NativeCallable implement
    // those interfaces too, but wrap a native object that is handed back as is instead of being proxied again
    polymorphic_type_handlers_.emplace_back(
        *inv.native_invocable_cls,
        [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
            jni_env env{ penv };
            jlong handle = env.invoke<jlong>(nullptr, obj, p->get_native_handle_);
            using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
            return object_blob_result<wrapper_object_t>(native_handle_ref<invocable>(handle));
        });

    polymorphic_type_handlers_.emplace_back(
        *inv.native_callable_cls,
        [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
            jni_env env{ penv };
            jlong handle = env.invoke<jlong>(nullptr, obj, p->get_native_handle_);
            using wrapper_object_t = wrapper_object<shared_ptr<callable>>;
            return object_blob_result<wrapper_object_t>(native_handle_ref<callable>(handle));
        });

    polymorphic_type_handlers_.emplace_back(
        *inv.invocable_cls,
        [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
            if (!obj) return nil_blob_result();
            using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
            return object_blob_result<wrapper_object_t>(make_shared<jni_invocable_proxy>(*as_singleton<jni_invoker>(penv), penv, obj, false));
        });

    polymorphic_type_handlers_.emplace_back(
        *inv.callable_cls,
        [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
            if (!obj) return nil_blob_result();
            using wrapper_object_t = wrapper_object<shared_ptr<callable>>;
            return object_blob_result<wrapper_object_t>(make_shared<jni_callable_proxy>(*as_singleton<jni_invoker>(penv), penv, obj));
        });

    // a Java exception passed as a value (e.g. CallbackBean reporting a failed call) becomes an error blob
    throwable_toString_ = env.get_jmethod(*inv.throwable_cls, "toString", "()Ljava/lang/String;");
    polymorphic_type_handlers_.emplace_back(
        *inv.throwable_cls,
        [](jni_decoder const* p, JNIEnv* penv, jobject obj) -> blob_result {
            if (!obj) return nil_blob_result();
            jni_env env{ penv };
            auto text = env.invoke<jobject>(nullptr, obj, p->throwable_toString_);
            if (!*text) return error_blob_result("java exception"sv, true);
            auto chars = env.get_string_utf_chars((jstring)*text);
            return error_blob_result(string_view{ chars.c_str() }, true);
        });
}

}
