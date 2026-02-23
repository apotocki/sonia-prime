//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <unordered_map>

#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/singleton.hpp"
#include "sonia/utility/invocation/invocation.hpp"

#include <jni.h>

namespace sonia::invocation {

class jni_decoder : public singleton
{
	friend class jni_invocable_proxy;

public:
	explicit jni_decoder(JNIEnv* penv);
	[[nodiscard]] static blob_result decode(JNIEnv*, jobject);

private:
	jclass invocable_registry_cls_;

	jmethodID boolean_booleanValue_;
	jmethodID byte_byteValue_;
	jmethodID short_shortValue_;
	jmethodID int_intValue_;
	jmethodID long_longValue_;
	jmethodID float_floatValue_;
	jmethodID double_doubleValue_;
	jmethodID get_invocable_id_;

	blob_result do_decode(JNIEnv*, jobject) const;

	using value_handler = blob_result(*)(jni_decoder const*, JNIEnv*, jobject);
	std::unordered_map<std::string, value_handler, hasher, string_equal_to> type_handlers_;
	std::vector<std::pair<jclass, value_handler>> polymorphic_type_handlers_;
};

}
