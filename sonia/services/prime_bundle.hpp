//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "bundle.hpp"

namespace sonia::services {

class prime_bundle : public bundle
{
public:
	~prime_bundle() override = default;

	string_view build_id() const noexcept override;

	void init() override;
};

}
