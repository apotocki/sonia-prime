//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "jnifwd.hpp"

#include <boost/container/small_vector.hpp>

#include "boost/conversion/unicode/utf.hpp"
#include "boost/conversion/push_iterator.hpp"

#include <cstdarg>

namespace sonia {

class vscope : boost::noncopyable {
public:
    vscope() = default;
    vscope(vscope const&) = delete;
    vscope& operator=(vscope const&) = delete;

    ~vscope() { va_end(vl_); }

    va_list& vl() { return vl_; }

private:
    va_list vl_;
};

class jni_env
{
public:
    inline explicit jni_env(JNIEnv* env) noexcept : env_{env} {}
    
    explicit jni_env(JavaVM* jvm);

    struct utf_chars
    {
        JNIEnv* env_;
        jstring js_;
        const char* str_;
        
        inline utf_chars(JNIEnv* env, jstring js, const char* str) noexcept
            : env_{env}, js_{js}, str_{str}
        {}

        utf_chars(utf_chars const&) = delete;
        utf_chars& operator=(utf_chars const&) = delete;

        inline utf_chars(utf_chars && rhs) noexcept
            : env_{ rhs.env_ }, js_{ rhs.js_ }, str_{ rhs.str_ }
        {
            rhs.env_ = nullptr;
        }

        inline utf_chars& operator=(utf_chars&& rhs) noexcept
        {
            if (env_) env_->ReleaseStringUTFChars(js_, str_);
            env_ = rhs.env_;
            js_ = rhs.js_;
            str_ = rhs.str_;
            rhs.env_ = nullptr;
            return* this;
        }

        ~utf_chars()
        {
            if (env_) env_->ReleaseStringUTFChars(js_, str_);
        }

        inline const char* c_str() const noexcept { return str_; }
    };

    //shared_ptr<jni_vm> get_vm() const;

    jobject new_local_ref(jobject ref) const;
    void delete_local_ref(jobject ref) const;

    jobject new_global_ref(jobject ref) const;
    void delete_global_ref(jobject ref) const;

    // exception routine
    void check_exception() const;
    void exception_clear() const;

    // object factory
    unique_jni_ref<jobject, local_ref_kind> new_instance(jclass cls, jmethodID method, va_list vl) const;

    // invokers
    
    //template <typename ResultT>
    //get_jni_t<ResultT> invoke(jobject obj, char const* mname, char const* sig, ...);

    template <typename ResultT>
    get_jni_t<ResultT> invoke(jclass cls, jobject obj, jmethodID method, ...) const;

    unique_jni_ref<jobject, local_ref_kind> invoke(jclass cls, std::type_identity<jobject>, jobject obj, jmethodID method, va_list vl) const;

    void        invoke(jclass cls, std::type_identity<void>, jobject obj, jmethodID method, va_list vl) const;
    jboolean    invoke(jclass cls, std::type_identity<jboolean>, jobject obj, jmethodID method, va_list vl) const;
    jbyte       invoke(jclass cls, std::type_identity<jbyte>, jobject obj, jmethodID method, va_list vl) const;
    jchar       invoke(jclass cls, std::type_identity<jchar>, jobject obj, jmethodID method, va_list vl) const;
    jshort      invoke(jclass cls, std::type_identity<jshort>, jobject obj, jmethodID method, va_list vl) const;
    jint        invoke(jclass cls, std::type_identity<jint>, jobject obj, jmethodID method, va_list vl) const;
    jlong       invoke(jclass cls, std::type_identity<jlong>, jobject obj, jmethodID method, va_list vl) const;
    jfloat      invoke(jclass cls, std::type_identity<jfloat>, jobject obj, jmethodID method, va_list vl) const;
    jdouble     invoke(jclass cls, std::type_identity<jdouble>, jobject obj, jmethodID method, va_list vl) const;

    // field accessors
    template <typename FT>
    get_jni_t<FT> get_static_field_value(jclass cls, jfieldID field) const;

    template <typename FT>
    get_jni_t<FT> get_field_value(jobject obj, jfieldID field) const;

    template <typename FT>
    void set_field_value(jobject obj, jfieldID field, FT value) const;

    template <typename FT>
    void set_static_field_value(jclass obj, jfieldID field, FT value) const;

    // method/field ids
    jmethodID get_jmethod(jclass cls, char const* mname, char const* sig) const;
    jmethodID get_static_jmethod(jclass cls, char const* mname, char const* sig) const;
    jfieldID get_jfield(jclass cls, char const* fname, char const* sig) const;
    jfieldID get_static_jfield(jclass cls, char const* fname, char const* sig) const;

    unique_jni_ref<jclass> get_class(jobject) const;
    unique_jni_ref<jstring> get_class_name(jclass c) const;

    // string operations
    jsize get_string_utf_length(jstring s) const;
    utf_chars get_string_utf_chars(jstring s) const;
    
    unique_jni_ref<jstring> new_string(const char* str) const;
    unique_jni_ref<jstring> new_string(string_view str) const;

    //unique_jni_ref<jobject> new_integer(jint) const;
    //unique_jni_ref<jobject> new_float(jfloat) const;

    template <typename JT>
    unique_jni_ref<jobject> new_boxed_value(JT) const;

    // arrays
    inline jsize get_array_length(jarray arr) const noexcept { return env_->GetArrayLength(arr); }

    void get_array_region(jbooleanArray arr, jsize start, jsize size, jboolean* region) const;
    void get_array_region(jbyteArray arr, jsize start, jsize size, jbyte* region) const;
    void get_array_region(jcharArray arr, jsize start, jsize size, jchar* region) const;
    void get_array_region(jshortArray arr, jsize start, jsize size, jshort* region) const;
    void get_array_region(jintArray arr, jsize start, jsize size, jint* region) const;
    void get_array_region(jlongArray arr, jsize start, jsize size, jlong* region) const;
    void get_array_region(jfloatArray arr, jsize start, jsize size, jfloat* region) const;
    void get_array_region(jdoubleArray arr, jsize start, jsize size, jdouble* region) const;
    void get_array_region(jobjectArray arr, jsize start, jsize size, unique_jni_ref<jobject>* region) const;

    void set_array_region(jbooleanArray arr, jsize start, jsize size, jboolean const* region) const;
    void set_array_region(jbyteArray arr, jsize start, jsize size, jbyte const* region) const;
    void set_array_region(jcharArray arr, jsize start, jsize size, jchar const* region) const;
    void set_array_region(jshortArray arr, jsize start, jsize size, jshort const* region) const;
    void set_array_region(jintArray arr, jsize start, jsize size, jint const* region) const;
    void set_array_region(jlongArray arr, jsize start, jsize size, jlong const* region) const;
    void set_array_region(jfloatArray arr, jsize start, jsize size, jfloat const* region) const;
    void set_array_region(jdoubleArray arr, jsize start, jsize size, jdouble const* region) const;
    void set_array_region(jobjectArray arr, jsize start, jsize size, unique_jni_ref<jobject> const* region) const;

    template <typename T>
    unique_jni_ref<jobjectArray> new_object_array(std::type_identity<T>, jsize sz) const;

    unique_jni_ref<jbooleanArray> new_array(std::type_identity<jboolean>, jsize sz) const;
    unique_jni_ref<jbyteArray> new_array(std::type_identity<jbyte>, jsize sz) const;
    unique_jni_ref<jcharArray> new_array(std::type_identity<jchar>, jsize sz) const;
    unique_jni_ref<jshortArray> new_array(std::type_identity<jshort>, jsize sz) const;
    unique_jni_ref<jintArray> new_array(std::type_identity<jint>, jsize sz) const;
    unique_jni_ref<jlongArray> new_array(std::type_identity<jlong>, jsize sz) const;
    unique_jni_ref<jfloatArray> new_array(std::type_identity<jfloat>, jsize sz) const;
    unique_jni_ref<jdoubleArray> new_array(std::type_identity<jdouble>, jsize sz) const;
    unique_jni_ref<jobjectArray> new_object_array(jsize sz, jclass cls, jobject initvalue) const;

    unique_jni_ref<jobject> get_object_array_element(jobjectArray arr, jsize pos) const;

    template <typename JT>
    JT* GetArrayElements(typename jni_traits<JT>::array_type arr) const;

    template <typename JT>
    void ReleaseArrayElements(typename jni_traits<JT>::array_type arr, JT* body, jint mode) const;

    // class routine
    unique_jni_ref<jclass, global_ref_kind> get_class(cstring_view clsname) const;

    jint throw_new(jclass cl, char const* msg) const;

    // lifecircle
    void register_natives(jclass, JNINativeMethod* mthds, jint sz) const;

    inline JNIEnv* get() const noexcept { return env_; }
    inline JNIEnv* operator-> () const noexcept { return env_; }
    inline explicit operator bool() const noexcept { return !!env_; }

    inline jni_env& operator= (JNIEnv* env) noexcept { env_ = env; return *this; }

    inline void swap(jni_env& rhs) noexcept
    {
        JNIEnv* temp = rhs.env_;
        rhs.env_ = env_;
        env_ = temp;
    }

private:
    JNIEnv* env_;
};

}

#include "jni_ref.hpp"

namespace sonia {

template <typename ResultT>
get_jni_t<ResultT> jni_env::invoke(jclass cls, jobject obj, jmethodID method, ...) const
{
    vscope scope;
    va_start(scope.vl(), method);
    return invoke(cls, std::type_identity<ResultT>(), obj, method, scope.vl());
}

template <typename JT>
unique_jni_ref<jobject> jni_env::new_boxed_value(JT value) const
{
    jclass cls = env_->FindClass(jni_traits<JT>::class_name);
    jmethodID midInit = env_->GetMethodID(cls, "<init>", jni_traits<JT>::ctor_sig);
    unique_jni_ref<jobject> result{ env_, env_->NewObject(cls, midInit, value) };
    env_->DeleteLocalRef(cls);
    return result;
}

template <typename FT>
get_jni_t<FT> jni_env::get_field_value(jobject obj, jfieldID field) const
{
    get_jni_t<FT> result;
    if constexpr (std::is_same_v<FT, jboolean>) {
        result = env_->GetBooleanField(obj, field);
    } else if constexpr (std::is_same_v<FT, jbyte>) {
        result = env_->GetByteField(obj, field);
    } else if constexpr (std::is_same_v<FT, jchar>) {
        result = env_->GetCharField(obj, field);
    } else if constexpr (std::is_same_v<FT, jshort>) {
        result = env_->GetShortField(obj, field);
    } else if constexpr (std::is_same_v<FT, jint>) {
        result = env_->GetIntField(obj, field);
    } else if constexpr (std::is_same_v<FT, jlong>) {
        result = env_->GetLongField(obj, field);
    } else if constexpr (std::is_same_v<FT, jfloat>) {
        result = env_->GetFloatField(obj, field);
    } else if constexpr (std::is_same_v<FT, jdouble>) {
        result = env_->GetDoubleField(obj, field);
    } else if constexpr (is_jobject_v<FT>) {
        result = env_->GetObjectField(obj, field);
    } else {
        static_assert(dependent_false<FT>);
    }
    check_exception();
    return result;
}

template <typename FT>
get_jni_t<FT> jni_env::get_static_field_value(jclass cls, jfieldID field) const
{
    get_jni_t<FT> result;
    if constexpr (std::is_same_v<FT, jboolean>) {
        result = env_->GetStaticBooleanField(cls, field);
    } else if constexpr (std::is_same_v<FT, jbyte>) {
        result = env_->GetStaticByteField(cls, field);
    } else if constexpr (std::is_same_v<FT, jchar>) {
        result = env_->GetStaticCharField(cls, field);
    } else if constexpr (std::is_same_v<FT, jshort>) {
        result = env_->GetStaticShortField(cls, field);
    } else if constexpr (std::is_same_v<FT, jint>) {
        result = env_->GetStaticIntField(cls, field);
    } else if constexpr (std::is_same_v<FT, jlong>) {
        result = env_->GetStaticLongField(cls, field);
    } else if constexpr (std::is_same_v<FT, jfloat>) {
        result = env_->GetStaticFloatField(cls, field);
    } else if constexpr (std::is_same_v<FT, jdouble>) {
        result = env_->GetStaticDoubleField(cls, field);
    } else if constexpr (is_jobject_v<FT>) {
        result = env_->GetStaticObjectField(cls, field);
    } else {
        static_assert(dependent_false<FT>);
    }
    check_exception();
    return result;
}

template <typename FT>
void jni_env::set_field_value(jobject obj, jfieldID field, FT value) const
{
    if constexpr (std::is_same_v<FT, jboolean>) {
        env_->SetBooleanField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jbyte>) {
        env_->SetByteField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jchar>) {
        env_->SetCharField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jshort>) {
        env_->SetShortField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jint>) {
        env_->SetIntField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jlong>) {
        env_->SetLongField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jfloat>) {
        env_->SetFloatField(obj, field, value);
    } else if constexpr (std::is_same_v<FT, jdouble>) {
        env_->SetDoubleField(obj, field, value);
    } else if constexpr (is_jobject_v<FT>) {
        env_->SetObjectField(obj, field, value);
    } else {
        static_assert(dependent_false<FT>);
    }
    check_exception();
}

template <typename FT>
void jni_env::set_static_field_value(jclass cls, jfieldID field, FT value) const
{
    if constexpr (std::is_same_v<FT, jboolean>) {
        env_->SetStaticBooleanField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jbyte>) {
        env_->SetStaticByteField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jchar>) {
        env_->SetStaticCharField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jshort>) {
        env_->SetStaticShortField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jint>) {
        env_->SetStaticIntField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jlong>) {
        env_->SetStaticLongField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jfloat>) {
        env_->SetStaticFloatField(cls, field, value);
    } else if constexpr (std::is_same_v<FT, jdouble>) {
        env_->SetStaticDoubleField(cls, field, value);
    } else if constexpr (is_jobject_v<FT>) {
        env_->SetStaticObjectField(cls, field, value);
    } else {
        static_assert(dependent_false<FT>);
    }
    check_exception();
}

template <typename JT>
JT* jni_env::GetArrayElements(typename jni_traits<JT>::array_type arr) const
{
    if constexpr (std::is_same_v<JT, jboolean>) {
        return env_->GetBooleanArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jbyte>) {
        return env_->GetByteArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jchar>) {
        return env_->GetCharArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jshort>) {
        return env_->GetShortArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jint>) {
        return env_->GetIntArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jlong>) {
        return env_->GetLongArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jfloat>) {
        return env_->GetFloatArrayElements(arr, nullptr);
    } else if constexpr (std::is_same_v<JT, jdouble>) {
        return env_->GetDoubleArrayElements(arr, nullptr);
    } else {
        static_assert(dependent_false<JT>);
    }
}

template <typename JT>
void jni_env::ReleaseArrayElements(typename jni_traits<JT>::array_type arr, JT* body, jint mode) const
{
    if constexpr (std::is_same_v<JT, jboolean>) {
        env_->ReleaseBooleanArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jbyte>) {
        env_->ReleaseByteArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jchar>) {
        env_->ReleaseCharArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jshort>) {
        env_->ReleaseShortArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jint>) {
        env_->ReleaseIntArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jlong>) {
        env_->ReleaseLongArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jfloat>) {
        env_->ReleaseFloatArrayElements(arr, body, mode);
    } else if constexpr (std::is_same_v<JT, jdouble>) {
        env_->ReleaseDoubleArrayElements(arr, body, mode);
    } else {
        static_assert(dependent_false<JT>);
    }
}

}
