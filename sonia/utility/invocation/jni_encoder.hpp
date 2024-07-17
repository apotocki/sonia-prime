//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/span.hpp"
#include "sonia/string.hpp"
#include "sonia/singleton.hpp"
#include "sonia/utility/invocation/invocation.hpp"

#include <jni.h>

namespace sonia::invocation {

class jni_encoder : public singleton
{
public:
	jni_encoder();
	[[nodiscard]] jobject encode(JNIEnv*, blob_result const&) const;
};

}
