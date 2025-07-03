//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "jni_vm.hpp"
#include "jni_ref.hpp"

namespace sonia {

jni_vm::jni_vm(JNIEnv* env)
{
    jint opcode = env->GetJavaVM(&jvm_);
    if (JNI_OK != opcode) {
        throw jni_error("can't get java vm, opcode: %1%"_fmt % opcode);
    }
}

unique_jni_ref<jclass, global_ref_kind> const& jni_vm::get_class(JNIEnv* penv, string_view clsnameref)
{
    unique_lock lock(cache_mutex_);
    auto cls_it = cls_cache_.find(clsnameref, hasher{}, string_equal_to{});
    if (cls_it != cls_cache_.end()) return cls_it->second;
    std::string clsname = to_string(clsnameref);
    jclass cls = penv->FindClass(clsname.c_str());
    if (!cls) {
        BOOST_THROW_EXCEPTION(jni_error("can't find class: " + clsname));
    }
    jclass gcls = (jclass)penv->NewGlobalRef(cls);
    penv->DeleteLocalRef(cls);
    if (!gcls) {
        BOOST_THROW_EXCEPTION(jni_error("can't make global ref for class: " + clsname));
    }
    auto it = cls_cache_.insert(cls_it, std::make_pair(std::move(clsname), unique_jni_ref<jclass, global_ref_kind>(jvm_, gcls)));
    return it->second;
}

}
