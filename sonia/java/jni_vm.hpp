//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/concurrency.hpp"

#include "jnifwd.hpp"
#include "jni_ref.hpp"

namespace sonia {

class jni_vm : public enable_shared_from_this<jni_vm>
{
public:
    explicit jni_vm(JavaVM* jvm = nullptr) : jvm_{ jvm } {}
    explicit jni_vm(JNIEnv* env);

    JavaVM* get_JavaVM() { return jvm_; }

    unique_jni_ref<jclass, global_ref_kind> const& get_class(JNIEnv* penv, string_view clsname);

    virtual JNIEnv* this_thread_environment() = 0;
    virtual void detach_current_thread() = 0;

    virtual ~jni_vm() = default;

    struct jni_thread {
        JNIEnv* env_;
        shared_ptr<jni_vm> vm_;

        ~jni_thread();
    };

    boost::thread_specific_ptr<jni_thread> perthread_env_;

protected:
    JavaVM* jvm_;
    mutable threads::mutex cache_mutex_;
    boost::unordered_map<std::string, unique_jni_ref<jclass, global_ref_kind> > cls_cache_;
};

}
