//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <jni.h>

#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

class jni_invoker;

// Owns a JNI global (strong) or weak global reference to a Java object.
// Strong keeps the Java object alive while native holds the proxy; weak lets it be collected
// (e.g. an invoker that itself owns the native object, see com.sonia.invocation.WeakInvocable).
// The reference may be released on any thread, see jni_invoker::delete_ref.
class jni_object_ref
{
    jni_invoker* invoker_;
    jobject ref_;
    bool weak_;

public:
    jni_object_ref(jni_invoker&, JNIEnv*, jobject obj, bool weak);
    jni_object_ref(jni_object_ref const&) = delete;
    jni_object_ref& operator=(jni_object_ref const&) = delete;
    ~jni_object_ref();

    inline jni_invoker& invoker() const noexcept { return *invoker_; }
    inline jobject get() const noexcept { return ref_; }
    inline bool is_weak() const noexcept { return weak_; }
};

// Native proxies over Java Invocable/Callable objects (see jni_decoder).
// Declared here so that jni_encoder can recognize them and hand back the original Java object.

class jni_callable_proxy
    : public callable
    , public enable_shared_from_this<jni_callable_proxy>
{
    jni_object_ref obj_;

public:
    inline jni_callable_proxy(jni_invoker& inv, JNIEnv* penv, jobject obj) : obj_{ inv, penv, obj, false } {}

    inline jobject java_object() const noexcept { return obj_.get(); }

    smart_blob invoke(span<const blob_result> args) override;
};

class jni_invocable_proxy
    : public invocable
    , public enable_shared_from_this<jni_invocable_proxy>
{
    jni_object_ref obj_;

public:
    inline jni_invocable_proxy(jni_invoker& inv, JNIEnv* penv, jobject obj, bool weak) : obj_{ inv, penv, obj, weak } {}

    inline jobject java_object() const noexcept { return obj_.get(); }

    shared_ptr<invocable> self_as_invocable_shared() override;

    bool has_method(string_view methodname) const override;
    bool try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept override;
    bool try_set_property(string_view propname, blob_result const& val) override;
    bool try_get_property(string_view propname, smart_blob& result) const override;
};

}
