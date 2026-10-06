//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <jni.h>

#include "sonia/utility/invocation/invocation.hpp"

namespace sonia::invocation {

// Native proxies over Java Invocable/Callable objects (see jni_decoder).
// Declared here so that jni_encoder can recognize them and hand back the original Java object.

class jni_callable_proxy
    : public callable
    , public enable_shared_from_this<jni_callable_proxy>
{
    JNIEnv* penv_;
    jint id_;

public:
    inline jni_callable_proxy(JNIEnv* penv, jint id) noexcept : penv_{ penv }, id_{ id } {}

    inline jint java_id() const noexcept { return id_; }

    smart_blob invoke(span<const blob_result> args) override;
};

class jni_invocable_proxy
    : public invocable
    , public enable_shared_from_this<jni_invocable_proxy>
{
    JNIEnv* penv_;
    jint id_;

public:
    inline jni_invocable_proxy(JNIEnv* penv, jint id) noexcept : penv_{ penv }, id_{ id } {}

    inline jint java_id() const noexcept { return id_; }

    shared_ptr<invocable> self_as_invocable_shared() override;

    bool try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept override;
    bool try_set_property(string_view propname, blob_result const& val) override;
    bool try_get_property(string_view propname, smart_blob& result) const override;
};

}
