//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/filesystem.hpp"
#include "sonia/utility/persister.hpp"

namespace sonia {

class file_persister : public persister
{
public:
    explicit file_persister(fs::path fname);

    ~file_persister() override;

    bool read(function<void(input_iterator)> const& ftor) const override;
    void write(function<void(output_iterator)> const& ftor) override;

private:
    fs::path fname_;
};

}
