//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <jni.h>

#include <mutex>

#include "sonia/exceptions.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/string.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/exception/error_info.hpp>

namespace sonia {

DECLARE_EXCEPTION(jni_error, "jni_error", exception)
DECLARE_EXCEPTION(jni_out_of_memory_error, "java out of memory", jni_error)

typedef boost::error_info<struct jni_error_code_t, jint> jni_error_code;
typedef boost::error_info<struct class_name_t, std::string> jni_class_name;

// forward declarations
class jni_vm;
class jni_env;

struct local_ref_kind {};
struct global_ref_kind {};
struct weak_ref_kind {};

template <typename T, typename KindT = local_ref_kind> class unique_jni_ref;
template <typename T, typename KindT = local_ref_kind> class jni_array;


template <typename JTypeT, typename KindT = local_ref_kind>
struct get_jni { using type = JTypeT; };

template <typename KindT>
struct get_jni<jobject, KindT> { using type = unique_jni_ref<jobject, KindT> ; };

template <typename JTypeT>
using get_jni_t = typename get_jni<JTypeT>::type;


template <typename T>
struct is_jobject : std::is_base_of<remove_pointer_t<jobject>, remove_pointer_t<T>> {};

template <typename T>
constexpr bool is_jobject_v = is_jobject<T>::value;


template <typename JT>
struct jni_traits {};

template <>
struct jni_traits<jboolean>
{
    using array_type = jbooleanArray;
    inline static const char* class_name = "java/lang/Boolean";
    inline static const char* ctor_sig = "(Z)V";
};

template <>
struct jni_traits<jbyte>
{
    using array_type = jbyteArray;
    inline static const char* class_name = "java/lang/Byte";
    inline static const char* ctor_sig = "(B)V";
};

template <>
struct jni_traits<jchar>
{
    using array_type = jcharArray;
    inline static const char* class_name = "java/lang/Character";
    inline static const char* ctor_sig = "(C)V";
};

template <>
struct jni_traits<jshort>
{
    using array_type = jshortArray;
    inline static const char* class_name = "java/lang/Short";
    inline static const char* ctor_sig = "(S)V";
};

template <>
struct jni_traits<jint>
{
    using array_type = jintArray;
    inline static const char* class_name = "java/lang/Integer";
    inline static const char* ctor_sig = "(I)V";
};

template <>
struct jni_traits<jlong>
{
    using array_type = jlongArray;
    inline static const char* class_name = "java/lang/Long";
    inline static const char* ctor_sig = "(J)V";
};

template <>
struct jni_traits<jfloat>
{
    using array_type = jfloatArray;
    inline static const char* class_name = "java/lang/Float";
    inline static const char* ctor_sig = "(F)V";
};

template <>
struct jni_traits<jdouble>
{
    using array_type = jdoubleArray;
    inline static const char* class_name = "java/lang/Double";
    inline static const char* ctor_sig = "(D)V";
};

/*
class jvm_registry {
public:
    virtual shared_ptr<jni_vm> instance(JNIEnv * env) = 0;
    virtual void put_instance(JNIEnv *) = 0;
    virtual void put_instance(shared_ptr<jni_vm>) = 0;
    virtual void clear_instance(jni_vm *) = 0;

    //virtual JNIEnv * this_thread_environment(shared_ptr<jni_vm>) = 0;

    virtual ~jvm_registry() {}
};

class jni_method_registry
{
public:
    virtual void do_registration(jni_vm& jvm, JNIEnv *env) const = 0;
};

*/

template <typename DerivedT>
class jni_object
{
public:
    static unique_jni_ref<jclass, global_ref_kind> const& get_class(JNIEnv * env);
    static unique_jni_ref<jclass, global_ref_kind> const& get_class(jni_env const& env);

private:
    static void init_class(JNIEnv * env);

    static std::once_flag once_cls_flag_;
    static unique_jni_ref<jclass, global_ref_kind> const* pcls_;
};

}
