//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "jni_encoder.hpp"
#include "jni_invoker.hpp"

#include "sonia/java/jni_env.hpp"
#include "sonia/java/jni_ref.hpp"

namespace sonia::invocation {

template <typename CT> struct cxxjni{ using jtype = void; };
template <> struct cxxjni<bool> { using jtype = jboolean; };
template <> struct cxxjni<int8_t> { using jtype = jbyte; };
template <> struct cxxjni<uint8_t> { using jtype = jbyte; };
template <> struct cxxjni<int16_t> { using jtype = jshort; };
template <> struct cxxjni<uint16_t> { using jtype = jshort; };
template <> struct cxxjni<int32_t> { using jtype = jint; };
template <> struct cxxjni<uint32_t> { using jtype = jint; };
template <> struct cxxjni<float> { using jtype = jfloat; };
template <> struct cxxjni<double_t> { using jtype = jdouble; };
template <> struct cxxjni<blob_result> { using jtype = jobject; };
template <> struct cxxjni<sonia::invocation::object> { using jtype = jobject; };

template <typename IT, typename JElementT>
[[nodiscard]]
jobject create_array(std::type_identity<JElementT>, jni_encoder const&, jni_env& env, blob_result const& b)
{
    using type_t = IT;
    using jtype_t = JElementT;
    jsize sz = static_cast<jsize>(array_size_of<type_t>(b));
    auto arr = env.new_array(std::type_identity<jtype_t>{}, sz);
    jtype_t* body = env.GetArrayElements<jtype_t>(arr);
    type_t const* begin_ptr = data_of<type_t>(b);
    type_t const* end_ptr = begin_ptr + sz;
    for (size_t index = 0; begin_ptr != end_ptr; ++begin_ptr, ++index) {
        body[index] = (jtype_t)*begin_ptr;
    }
    env.ReleaseArrayElements<jtype_t>(arr, body, 0);
    return arr.detach();
}

template <typename IT>
[[nodiscard]]
jobject create_array(std::type_identity<jobject>, jni_encoder const& enc, jni_env& env, blob_result const& b)
{
    if constexpr (std::is_same_v<blob_result, IT>) {
        GLOBAL_LOG_INFO() << "jni_encoder::encode: encoding array of blob_result, blob: " << b;
        jsize sz = static_cast<jsize>(array_size_of<blob_result>(b));
        
        // create array of Invokeable objects if all elements of the array are invocable objects, otherwise create array of Objects
        bool all_invocables = true;
        blob_result const* begin_ptr = data_of<blob_result>(b);
        blob_result const* end_ptr = begin_ptr + sz;
        for (; begin_ptr != end_ptr; ++begin_ptr) {
            if (begin_ptr->type != blob_type::object) {
                all_invocables = false;
                break;
            }
        }
        
        auto jobjarr = env.new_object_array(sz, all_invocables ? enc.invocable_cls_ : enc.obj_cls_, nullptr);
        
        begin_ptr = data_of<blob_result>(b);
        for (jsize index = 0; begin_ptr != end_ptr; ++begin_ptr, ++index) {
            jobject arg = jni_encoder::encode(env.get(), *begin_ptr);
            if (arg) {
                env->SetObjectArrayElement(*jobjarr, index, arg);
                env->DeleteLocalRef(arg);
            }
        }
        return jobjarr.detach();
    } else if constexpr (std::is_same_v<sonia::invocation::object, IT>) {
        GLOBAL_LOG_INFO() << "jni_encoder::encode: encoding array of invocable objects, blob: " << b;
        using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
        jsize sz = static_cast<jsize>(array_size_of<wrapper_object_t>(b));
        auto jobjarr = env.new_object_array(sz, enc.invocable_cls_, nullptr);
        wrapper_object_t const* begin_ptr = data_of<wrapper_object_t>(b);
        wrapper_object_t const* end_ptr = begin_ptr + sz;
        for (jsize index = 0; begin_ptr != end_ptr; ++begin_ptr, ++index) {
            smart_blob java_id_res;
            if (!begin_ptr->value->try_get_property("java_id"sv, java_id_res)) {
                THROW_INTERNAL_ERROR("jni_encoder::encode: can't get java_id property of object %1%"_fmt % begin_ptr->value.get());
            }
            jint java_id = java_id_res.as<jint>();
            GLOBAL_LOG_INFO() << "jni_encoder::encode: encoding object array element, java_id: " << java_id;
            auto arg = env.invoke<jobject>(enc.invocable_registry_cls_, nullptr, enc.get_invocable_, java_id);
            if (arg) {
                env->SetObjectArrayElement(*jobjarr, index, *arg);
            }
        }
        env.invoke<void>(enc.invocable_registry_cls_, nullptr, enc.debug_method_, *jobjarr);
        return jobjarr.detach();
    } else {
        static_assert(sonia::dependent_false<IT>);
        return nullptr;
    }
}

template <typename IT>
jobject create_array(std::type_identity<void>, jni_encoder const&, jni_env& env, blob_result const& b)
{
    THROW_NOT_IMPLEMENTED_ERROR("jni_encoder::encode: can't encode array %1%"_fmt % b);
}

jobject jni_encoder::do_encode(JNIEnv* penv, blob_result const& b) const
{
    if (is_nil(b)) return nullptr;
    
    jni_env env{ penv };
    if (b.type == blob_type::object) { // object is considered ad an array type (array of bytes that represents an object)
        using wrapper_object_t = wrapper_object<shared_ptr<invocable>>;
        shared_ptr<invocable> invk = as<wrapper_object_t>(b).value;
        smart_blob java_id_res;
        if (!invk->try_get_property("java_id", java_id_res)) {
            GLOBAL_LOG_ERROR() << "jni_encoder::encode: failed to get java_id property of object invokable" << b;
            return nullptr;
        }
        return env.invoke<jobject>(invocable_registry_cls_, nullptr, get_invocable_, java_id_res.as<jint>()).detach();
    }
    if (is_array(b) && !contains_string(b)) {
        return blob_type_selector(b, [&env, this](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using jtype = typename cxxjni<type>::jtype;
            GLOBAL_LOG_INFO() << "jni_encoder::encode: encoding array of type " << typeid(type).name() << ", blob: " << b;
            return create_array<type>(std::type_identity<jtype>{}, *this, env, b);
        });
    }
    switch (b.type) {
    case blob_type::boolean:
        return env.new_boxed_value<jboolean>((jint)b.data.bp.i8value).detach();
    case blob_type::i8:
        return env.new_boxed_value<jbyte>((jbyte)b.data.bp.i8value).detach();
    case blob_type::ui8:
        return env.new_boxed_value<jshort>((jshort)b.data.bp.ui8value).detach();
    case blob_type::i16:
        return env.new_boxed_value<jshort>((jshort)b.data.bp.i16value).detach();
    case blob_type::ui16:
        return env.new_boxed_value<jint>((jint)b.data.bp.ui16value).detach();
    case blob_type::i32:
        return env.new_boxed_value<jint>((jint)b.data.bp.i32value).detach();
    case blob_type::ui32:
        return env.new_boxed_value<jlong>((jlong)b.data.bp.i32value).detach();
    case blob_type::i64:
        return env.new_boxed_value<jlong>((jlong)b.data.bp.i64value).detach();
    case blob_type::ui64:
        return env.new_boxed_value<jlong>((jlong)b.data.bp.ui64value).detach();
    case blob_type::flt16:
        return env.new_boxed_value<jfloat>((jfloat)b.data.bp.f16value).detach();
    case blob_type::flt32:
        return env.new_boxed_value<jfloat>((jfloat)b.data.bp.f32value).detach();
    case blob_type::flt64:
        return env.new_boxed_value<jdouble>((jdouble)b.data.bp.f64value).detach();
    case blob_type::string:
    {
        string_view s{ data_of<char>(b), array_size_of<char>(b) };
        return env.new_string(s).detach();
    }
    default:
        THROW_NOT_IMPLEMENTED_ERROR("jni_encoder::encode: can't encode %1%"_fmt % b);
    }
}

jni_encoder::jni_encoder(JNIEnv* penv)
{
    jni_invoker const& inv = *as_singleton<jni_invoker>(penv);
    obj_cls_ = *inv.obj_cls;
    invocable_cls_ = *inv.invocable_cls;
    invocable_registry_cls_ = *inv.invocable_registry_cls;

    jni_env env{ penv };
    get_invocable_ = env.get_static_jmethod(invocable_registry_cls_, "get", "(I)Lcom/sonia/invocation/Invocable;");
    debug_method_ = env.get_static_jmethod(invocable_registry_cls_, "debug", "(Ljava/lang/Object;)V");
}

jobject jni_encoder::encode(JNIEnv* penv, blob_result const& br)
{
    return as_singleton<jni_encoder>(penv)->do_encode(penv, br);
}

}
