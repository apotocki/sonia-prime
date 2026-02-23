//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "jni_env.hpp"
#include "jni_ref.hpp"

#define SONIA_JNI_INLINE

#include "sonia/logger/logger.hpp"

namespace sonia {

jni_env::jni_env(JavaVM* jvm)
{
    int res = jvm->GetEnv((void**)&env_, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED) {
        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_6; // choose your JNI version
        args.name = nullptr; // you might want to give the java thread a name
        args.group = nullptr; // you might want to assign the java thread to a ThreadGroup
#ifdef __ANDROID__
        int optres = jvm->AttachCurrentThread(&env_, &args);
#else
        int optres = jvm->AttachCurrentThread((void**)&env_, &args);
#endif
        if (optres != JNI_OK) {
            GLOBAL_LOG_INFO() << "can't attach thread, res: " << optres;
            throw jni_error("can't attach current thread, error: %1%"_fmt % optres);
        }
        GLOBAL_LOG_INFO() << "AttachCurrentThread, env_: " << (uint64_t)env_;

    } else if (res == JNI_EVERSION) {
        throw jni_error("unsupported jni version");
    } else if (res != JNI_OK) {
        throw jni_error("can't retrieve environment");
    }
}

SONIA_JNI_INLINE jobject jni_env::new_local_ref(jobject ref) const
{
    if (!ref) return nullptr;
    jobject result = env_->NewLocalRef(ref);
    if (!result) {
        env_->ExceptionClear();
        throw jni_out_of_memory_error("can't get local reference");
    }
    return result;
}

SONIA_JNI_INLINE jobject jni_env::new_global_ref(jobject ref) const
{
    if (!ref) return nullptr;
    jobject result = env_->NewGlobalRef(ref);
    if (!result) {
        env_->ExceptionClear();
        throw jni_out_of_memory_error("can't get global reference");
    }
    return result;
}

SONIA_JNI_INLINE void jni_env::delete_global_ref(jobject ref) const
{
    if (ref) env_->DeleteGlobalRef(ref);
}

SONIA_JNI_INLINE unique_jni_ref<jclass> jni_env::get_class(jobject obj) const
{
    jclass clsObj = env_->GetObjectClass(obj);
    jni_env::check_exception();
    return unique_jni_ref<jclass>{env_, clsObj};
}

SONIA_JNI_INLINE unique_jni_ref<jstring> jni_env::get_class_name(jclass c) const
{
    unique_jni_ref<jclass> clsOfCls = get_class(c);
    jmethodID tostringmthd = env_->GetMethodID(*clsOfCls, "getName", "()Ljava/lang/String;");
    jni_env::check_exception();
    
    unique_jni_ref<jstring> result{env_, (jstring)env_->CallObjectMethod(c, tostringmthd)};
    jni_env::check_exception();
    return result;
}

SONIA_JNI_INLINE jsize jni_env::get_string_utf_length(jstring s) const
{
    jsize result = env_->GetStringUTFLength(s);
    jni_env::check_exception();
    return result;
}

SONIA_JNI_INLINE jni_env::utf_chars jni_env::get_string_utf_chars(jstring s) const
{
    const char* result = env_->GetStringUTFChars(s, nullptr);
    if (!result) {
        env_->ExceptionClear(); // clear OutOfMemoryError
        BOOST_THROW_EXCEPTION(jni_error("OutOfMemoryError"));
    }
    return utf_chars{env_, s, result};
}

SONIA_JNI_INLINE unique_jni_ref<jstring> jni_env::new_string(string_view str) const
{
    using namespace dataforge;
    boost::container::small_vector<jchar, 64> buff;
    (quark_push_iterator{ utf8 | utf16, std::back_inserter(buff) } << str).flush();
    unique_jni_ref<jstring> result{env_, env_->NewString(buff.data(), (jsize)buff.size())};
    jni_env::check_exception();
    return result;
}

SONIA_JNI_INLINE unique_jni_ref<jstring> jni_env::new_string(const char* str) const
{
    unique_jni_ref<jstring> result(env_, env_->NewStringUTF(str));
    jni_env::check_exception();
    return result;
}

SONIA_JNI_INLINE unique_jni_ref<jobject> jni_env::get_object_array_element(jobjectArray arr, jsize pos) const
{
    unique_jni_ref<jobject> result(env_, env_->GetObjectArrayElement(arr, pos));
    jni_env::check_exception();
    return result;
}

SONIA_JNI_INLINE void jni_env::check_exception() const
{
    if (!env_->ExceptionCheck()) return;
    unique_jni_ref<jthrowable, local_ref_kind> throwable(env_, env_->ExceptionOccurred());
    env_->ExceptionClear();
    if (!throwable) {
        BOOST_THROW_EXCEPTION(jni_error("some java exception (can't retrieve)"));
    }

    for (;;) {
        unique_jni_ref<jclass, local_ref_kind> throwablecls(env_, env_->FindClass("java/lang/Throwable"));
        if (!throwablecls) break;

        unique_jni_ref<jclass, local_ref_kind> strwrcls(env_, env_->FindClass("java/io/StringWriter"));
        if (!strwrcls) break;

        unique_jni_ref<jclass, local_ref_kind> pwrcls(env_, env_->FindClass("java/io/PrintWriter"));
        if (!pwrcls) break;

        jmethodID strwrctor = env_->GetMethodID(*strwrcls, "<init>", "()V");
        if (!strwrctor || env_->ExceptionCheck()) break;
        unique_jni_ref<jobject, local_ref_kind> strwr(env_, env_->NewObject(*strwrcls, strwrctor));
        if (!strwr) break;

        jmethodID prwrctor = env_->GetMethodID(*pwrcls, "<init>", "(Ljava/io/Writer;)V");
        if (!prwrctor || env_->ExceptionCheck()) break;
        unique_jni_ref<jobject, local_ref_kind> pwr(env_, env_->NewObject(*pwrcls, prwrctor, *strwr));
        if (!pwr) break;

        jmethodID printmthd = env_->GetMethodID(*throwablecls, "printStackTrace", "(Ljava/io/PrintWriter;)V");
        if (!printmthd || env_->ExceptionCheck()) break;
        env_->CallVoidMethod(*throwable, printmthd, *pwr);
        if (env_->ExceptionCheck()) break;
        pwr.reset();
        throwable.reset();
        throwablecls.reset();

        jmethodID tostringmthd = env_->GetMethodID(*strwrcls, "toString", "()Ljava/lang/String;");
        if (!tostringmthd || env_->ExceptionCheck()) break;
        unique_jni_ref<jstring, local_ref_kind> exstr(env_, (jstring)env_->CallObjectMethod(*strwr, tostringmthd));
        if (!exstr || env_->ExceptionCheck()) break;

        jsize sz = env_->GetStringUTFLength(*exstr);
        jboolean false_val = JNI_FALSE;
        const char* buff = env_->GetStringUTFChars(*exstr, &false_val);
        if (!buff) break;
        std::string result;
        try {
            result = std::move(std::string(buff, (size_t)sz));
        } catch (...) {
            env_->ReleaseStringUTFChars(*exstr, buff);
            break;
        }
        env_->ReleaseStringUTFChars(*exstr, buff);
        while (!result.empty() && (result.back() == '\r' || result.back() == '\n')) {
            result.pop_back();
        }
        BOOST_THROW_EXCEPTION(jni_error(result));
    }

    env_->ExceptionClear();
    BOOST_THROW_EXCEPTION(jni_error("some java exception (can't retrieve)"));
}

// method invokation
//template <typename ResultT>
//get_jni_t<ResultT> jni_env::invoke(jclass cls, jobject obj, char const* mname, char const* sig, ...)
//{
//    vscope scope;
//    va_start(scope.vl(), sig);
//    return invoke<ResultT>(cls, std::type_identity<ResultT>(), obj, mname, get_jmethod(cls, mname, sig), scope.vl());
//}
//
//
//template <typename ResultT>
//get_jni_t<ResultT> jni_env::invoke(jclass cls, jobject obj, char const* mname, char const* sig, va_list vl)
//{
//    return invoke(cls, std::type_identity<ResultT>(), obj, get_jmethod(cls, mname, sig), vl);
//}



SONIA_JNI_INLINE unique_jni_ref<jobject, local_ref_kind> jni_env::invoke(jclass cls, std::type_identity<jobject>, jobject obj, jmethodID method, va_list vl) const
{
    unique_jni_ref<jobject, local_ref_kind> result(env_, obj ? env_->CallObjectMethodV(obj, method, vl) : env_->CallStaticObjectMethodV(cls, method, vl));
    check_exception();
    return result;
}

SONIA_JNI_INLINE void jni_env::invoke(jclass cls, std::type_identity<void>, jobject obj, jmethodID method, va_list vl) const
{
    obj ? env_->CallVoidMethodV(obj, method, vl) : env_->CallStaticVoidMethodV(cls, method, vl);
    check_exception();
}

SONIA_JNI_INLINE jboolean jni_env::invoke(jclass cls, std::type_identity<jboolean>, jobject obj, jmethodID method, va_list vl) const
{
    jboolean result = obj ? env_->CallBooleanMethodV(obj, method, vl) : env_->CallStaticBooleanMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jbyte jni_env::invoke(jclass cls, std::type_identity<jbyte>, jobject obj, jmethodID method, va_list vl) const
{
    jbyte result = obj ? env_->CallByteMethodV(obj, method, vl) : env_->CallStaticByteMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jchar jni_env::invoke(jclass cls, std::type_identity<jchar>, jobject obj, jmethodID method, va_list vl) const
{
    jchar result = obj ? env_->CallCharMethodV(obj, method, vl) : env_->CallStaticCharMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jshort jni_env::invoke(jclass cls, std::type_identity<jshort>, jobject obj, jmethodID method, va_list vl) const
{
    jshort result = obj ? env_->CallShortMethodV(obj, method, vl) : env_->CallStaticShortMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jint jni_env::invoke(jclass cls, std::type_identity<jint>, jobject obj, jmethodID method, va_list vl) const
{
    jint result = obj ? env_->CallIntMethodV(obj, method, vl) : env_->CallStaticIntMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jlong jni_env::invoke(jclass cls, std::type_identity<jlong>, jobject obj, jmethodID method, va_list vl) const
{
    jlong result = obj ? env_->CallLongMethodV(obj, method, vl) : env_->CallStaticLongMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jfloat jni_env::invoke(jclass cls, std::type_identity<jfloat>, jobject obj, jmethodID method, va_list vl) const
{
    jfloat result = obj ? env_->CallFloatMethodV(obj, method, vl) : env_->CallStaticFloatMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jdouble jni_env::invoke(jclass cls, std::type_identity<jdouble>, jobject obj, jmethodID method, va_list vl) const
{
    jdouble result = obj ? env_->CallDoubleMethodV(obj, method, vl) : env_->CallStaticDoubleMethodV(cls, method, vl);
    check_exception();
    return result;
}

SONIA_JNI_INLINE jmethodID jni_env::get_jmethod(jclass cls, char const* mname, char const* sig) const
{
    jmethodID method = env_->GetMethodID(cls, mname, sig);
    if (!method) {
        env_->ExceptionClear();
        auto clsnamejstring = get_class_name(cls);
        std::ostringstream errss;
        errss << "error: can't find method `" << mname << "` with signature " << sig << " for class " << get_string_utf_chars(clsnamejstring.get()).c_str();
        throw jni_error(errss.str());
    }
    return method;
}

SONIA_JNI_INLINE jmethodID jni_env::get_static_jmethod(jclass cls, char const* mname, char const* sig) const
{
    jmethodID method = env_->GetStaticMethodID(cls, mname, sig);
    if (!method) {
        env_->ExceptionClear();
        auto clsnamejstring = get_class_name(cls);
        std::ostringstream errss;
        errss << "error: can't find static method `" << mname << "` with signature " << sig << " for class " << get_string_utf_chars(clsnamejstring.get()).c_str();
        throw jni_error(errss.str());
    }
    return method;
}

jfieldID jni_env::get_jfield(jclass cls, char const* fname, char const* sig) const
{
    jfieldID sfield = env_->GetFieldID(cls, fname, sig);
    if (!sfield) {
        env_->ExceptionClear();
        throw jni_error("can't find instance field %1% with signature %2% for class %3%"_fmt % fname % sig % get_class_name(cls));
    }
    return sfield;
}

jfieldID jni_env::get_static_jfield(jclass cls, char const* fname, char const* sig) const
{
    jfieldID sfield = env_->GetStaticFieldID(cls, fname, sig);
    if (!sfield) {
        env_->ExceptionClear();
        throw jni_error("can't find static field %1% with signature %2% for class %3%"_fmt % fname % sig % get_class_name(cls));
    }
    return sfield;
}

SONIA_JNI_INLINE unique_jni_ref<jbooleanArray> jni_env::new_array(std::type_identity<jboolean>, jsize sz) const
{
    unique_jni_ref<jbooleanArray> result(env_, env_->NewBooleanArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jbyteArray> jni_env::new_array(std::type_identity<jbyte>, jsize sz) const
{
    unique_jni_ref<jbyteArray> result(env_, env_->NewByteArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jcharArray> jni_env::new_array(std::type_identity<jchar>, jsize sz) const
{
    unique_jni_ref<jcharArray> result(env_, env_->NewCharArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jshortArray> jni_env::new_array(std::type_identity<jshort>, jsize sz) const
{
    unique_jni_ref<jshortArray> result(env_, env_->NewShortArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jintArray> jni_env::new_array(std::type_identity<jint>, jsize sz) const
{
    unique_jni_ref<jintArray> result(env_, env_->NewIntArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jlongArray> jni_env::new_array(std::type_identity<jlong>, jsize sz) const
{
    unique_jni_ref<jlongArray> result(env_, env_->NewLongArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jfloatArray> jni_env::new_array(std::type_identity<jfloat>, jsize sz) const
{
    unique_jni_ref<jfloatArray> result(env_, env_->NewFloatArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jdoubleArray> jni_env::new_array(std::type_identity<jdouble>, jsize sz) const
{
    unique_jni_ref<jdoubleArray> result(env_, env_->NewDoubleArray(sz));
    jni_env::check_exception();
    return std::move(result);
}

SONIA_JNI_INLINE unique_jni_ref<jobjectArray> jni_env::new_object_array(jsize sz, jclass cl, jobject initvalue) const
{
    unique_jni_ref<jobjectArray> result(env_, env_->NewObjectArray(sz, cl, initvalue));
    jni_env::check_exception();
    return result;
}

//SONIA_JNI_INLINE unique_jni_ref<jobject> jni_env::new_integer(jint value) const
//{
//    jclass cls = env_->FindClass("java/lang/Integer");
//    jmethodID midInit = env_->GetMethodID(cls, "<init>", "(I)V");
//    unique_jni_ref<jobject> result {env_, env_->NewObject(cls, midInit, value)};
//    env_->DeleteLocalRef(cls);
//    return result;
//}
//
//SONIA_JNI_INLINE unique_jni_ref<jobject> jni_env::new_float(jfloat value) const
//{
//    jclass cls = env_->FindClass("java/lang/Float");
//    jmethodID midInit = env_->GetMethodID(cls, "<init>", "(F)V");
//    unique_jni_ref<jobject> result{ env_, env_->NewObject(cls, midInit, value) };
//    env_->DeleteLocalRef(cls);
//    return result;
//}

SONIA_JNI_INLINE unique_jni_ref<jclass, global_ref_kind> jni_env::get_class(cstring_view clsname) const
{
    JavaVM* jvm;
    if (env_->GetJavaVM(&jvm)) [[unlikely]] {
        throw jni_error("can't retrieve jvm");
    }
    jclass cls = env_->FindClass(clsname.c_str());
    unique_jni_ref<jclass, global_ref_kind> result{ jvm, (jclass)new_global_ref(cls) };
    env_->DeleteLocalRef(cls);
    return result;
}

SONIA_JNI_INLINE jint jni_env::throw_new(jclass cl, char const* msg) const
{
    return env_->ThrowNew(cl, msg);
}

}
