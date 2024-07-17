//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "jnifwd.hpp"

#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/info.hpp>
#include <boost/throw_exception.hpp>

#include "sonia/logger/logger.hpp"

namespace sonia::detail {

template <typename T>
class unique_jni_ref_base
{
    unique_jni_ref_base(unique_jni_ref_base&) = delete;
    unique_jni_ref_base& operator=(unique_jni_ref_base&) = delete;

protected:
    inline explicit unique_jni_ref_base(T ref = nullptr) noexcept : ref_{ref} {}
    inline unique_jni_ref_base(unique_jni_ref_base&& rhs) noexcept : ref_{rhs.ref_} { rhs.ref_ = nullptr; }

    inline void operator= (unique_jni_ref_base&& rhs) noexcept
    {
        ref_ = rhs.ref_;
        rhs.ref_ = nullptr;
    }

public:
    virtual ~unique_jni_ref_base() = default;

    inline T operator*() const noexcept { return ref_; }

    inline T get() const noexcept { return ref_; }

    operator T() const noexcept { return ref_; }

    inline T detach() noexcept
    {
        T result = ref_;
        ref_ = 0;
        return result;
    }

    inline void swap(unique_jni_ref_base& rhs) noexcept
    {
        T temp = rhs.ref_;
        rhs.ref_ = ref_;
        ref_ = temp;
    }

    inline explicit operator bool() noexcept { return !!ref_; }

protected:
    T ref_;
};

} // sonia::detail

namespace sonia {

template <typename T>
class unique_jni_ref<T, local_ref_kind> : public detail::unique_jni_ref_base<T>
{
    using base_t = detail::unique_jni_ref_base<T>;

public:
    //unique_jni_ref() : base_t{nullptr}, env_{nullptr} {
    //    throw internal_error("empty unique_jni_ref ctor can't be called");
    //}

    inline explicit unique_jni_ref(JNIEnv * env = nullptr, T ref = nullptr) noexcept : base_t{ref}, env_{env} {}

    template <typename OtherT>
    inline explicit unique_jni_ref(unique_jni_ref<OtherT, local_ref_kind> const& rhs) noexcept : env_{rhs.get_jni_env()}
    {
        if (!*rhs) return;
        this->ref_ = (T)env_->NewLocalRef(*rhs);
    }

    template <typename OtherT>
    inline explicit unique_jni_ref(unique_jni_ref<OtherT, local_ref_kind> && rhs) noexcept : env_{rhs.get_jni_env()}
    {
        this->ref_ = static_cast<T>(rhs.detach());
    }

    //template <typename OtherT>
    //explicit unique_jni_ref(unique_jni_ref<OtherT, global_ref_kind> const& rhs) : env_{nullptr}
    //{
    //    if (!*rhs) return;
    //    env_ = rhs.get_vm()->this_thread_environment();
    //    this->ref_ = env_->NewLocalRef(*rhs);
    //}

    inline unique_jni_ref(unique_jni_ref && rhs) noexcept : base_t{std::move((base_t&)rhs)}, env_{std::move(rhs.env_)} {}

    inline ~unique_jni_ref()
    {
        if (this->ref_) {
            env_->DeleteLocalRef(this->ref_);
        }
    }

    inline unique_jni_ref& operator= (unique_jni_ref && rhs) noexcept
    {
        base_t::operator= (std::move((base_t&)rhs));
        env_ = rhs.env_;
        return *this;
    }

    inline unique_jni_ref& operator= (unique_jni_ref const& rhs) noexcept
    {
        unique_jni_ref temp(rhs);
        swap(temp);
        return *this;
    }

    template <typename OtherT, typename OtherKindT>
    inline void operator= (unique_jni_ref<OtherT, OtherKindT> const& rhs) noexcept
    {
        unique_jni_ref temp(rhs);
        swap(temp);
        return *this;
    }

    inline void swap(unique_jni_ref& rhs) noexcept
    {
        base_t::swap(rhs);
        std::swap(env_, rhs.env_);
    }

    void reset()
    {
        if (**this) {
            env_->DeleteLocalRef(this->ref_);
            this->ref_ = 0;
        }
    }

    //jni_env const& get_jni_env() const { return env_; }

private:
    JNIEnv* env_;
};


template <typename T>
class unique_jni_ref<T, global_ref_kind> : public detail::unique_jni_ref_base<T>
{
    using base_t = detail::unique_jni_ref_base<T>;

    JavaVM* jvm_;

public:
    explicit unique_jni_ref(JavaVM* jvm = nullptr, T ref = nullptr)
        : base_t{ref}, jvm_{ jvm } //jvm_(jni_env(env).get_vm())
    { }

    //explicit unique_jni_ref(jni_env const& env, T ref = nullptr) : base_t(ref), env_{ env } //jvm_(env.get_vm())
    //{ }

    //explicit unique_jni_ref(shared_ptr<jni_vm> vm, T ref = nullptr) : base_t(ref), jvm_(vm)
    //{ }

    unique_jni_ref(unique_jni_ref const& rhs);

    //template <typename OtherT>
    //explicit unique_jni_ref(unique_jni_ref<OtherT, local_ref_kind> const& rhs) : jvm_(rhs.get_jni_env().get_vm())
    //{
    //    if (!rhs) return;
    //    this->ref_ = (T)rhs.get_jni_env().new_global_ref(*rhs);
    //}

    //template <typename OtherT>
    //explicit unique_jni_ref(unique_jni_ref<OtherT, global_ref_kind> const& rhs) : jvm_(rhs.get_vm())
    //{
    //    if (!rhs) return;
    //    this->ref_ = (T)jni_env(jvm_->this_thread_environment()).new_global_ref(*rhs);
    //}

    unique_jni_ref(unique_jni_ref && rhs) : base_t(std::move((base_t&)rhs)), jvm_{ rhs.jvm_ }
    {
        rhs.jvm_ = nullptr;
    }

    ~unique_jni_ref();

    void operator= (unique_jni_ref && rhs) noexcept
    {
        base_t::operator= (std::move((base_t&)rhs));
        jvm_ = std::move(rhs.jvm_);
        rhs.jvm_ = nullptr;
    }

    void operator= (unique_jni_ref const& rhs) noexcept
    {
        unique_jni_ref temp{rhs};
        swap(temp);
    }

    template <typename OtherT, typename OtherKindT>
    void operator= (unique_jni_ref<OtherT, OtherKindT> const& rhs) noexcept
    {
        unique_jni_ref temp{rhs};
        swap(temp);
    }

    void swap(unique_jni_ref& rhs) noexcept
    {
        base_t::swap(rhs);
        std::swap(jvm_, rhs.jvm_);
    }
};


/*
template <typename T>
class unique_jni_ref<T, weak_ref_kind> : public detail::unique_jni_ref_base<T>
{
    typedef detail::unique_jni_ref_base<T> base_t;

protected:
    unique_jni_ref(JNIEnv * env, T ref) : base_t(ref), jvm_(0)
    {
        jint opcode = env->GetJavaVM(&jvm_);
        if (JNI_OK != opcode)
            BOOST_THROW_EXCEPTION (jni_error("can't get JavaVM by JNIEnv") << jni_error_code(opcode));
    }

public:
    unique_jni_ref(unique_jni_ref && rhs) : base_t(std::move((base_t&)rhs)), jvm_(rhs.jvm_)
    { }

    ~unique_jni_ref()
    {
        if (!this->ref_) return;
        JNIEnv *env;
        try {
            env = get_JNIEnv();
        } catch (boost::exception const& ex) {
            GLOBAL_LOG_ERROR() << boost::diagnostic_information(ex);
            return;
        }
        env->DeleteWeakGlobalRef(this->ref_);
    }

    void operator= (unique_jni_ref const& rhs) {
        unique_jni_ref temp(rhs);
        swap(temp);
    }

    JNIEnv * get_JNIEnv() {
        JNIEnv *env;
        jint opcode = jvm_->AttachCurrentThread((void **)&env, 0);
        if (JNI_OK != opcode)
            BOOST_THROW_EXCEPTION (jni_error("can't attach java thread") << jni_error_code(opcode));
        return env;
    }

private:
    JavaVM *jvm_;
};
*/

}

#include "jni_env.hpp"

namespace sonia {

template <typename T>
unique_jni_ref<T, global_ref_kind>::unique_jni_ref(unique_jni_ref const& rhs)
    : jvm_(rhs.jvm_) //jvm_(rhs.jvm_)
{
    if (!rhs || !jvm_) return;
    jni_env env{ jvm_ };
    this->ref_ = (T)env.new_global_ref(rhs);
}

template <typename T>
unique_jni_ref<T, global_ref_kind>::~unique_jni_ref()
{
    if (!this->ref_) return;
    try {
        jni_env env{ jvm_ };
        env.delete_global_ref(**this);
        //jvm_->this_thread_environment()->DeleteGlobalRef(**this);
    } catch (std::exception const& ex) {
        GLOBAL_LOG_ERROR() << boost::diagnostic_information(ex);
        return;
    }
}

}
