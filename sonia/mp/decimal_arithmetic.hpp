//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "basic_decimal.hpp"

namespace sonia::mp {

//void numeric_round(decimal &);
//void ceil(decimal &);
//void floor(decimal &);
    /*
    void numeric_round(decimal& val)
    {
        if (auto exp = val.raw_exp(); exp < 0) {
            double tmpval = std::round(val.get<double>());
            if (exp == -1 && val < 0 && (-val.raw_value()) % 10 <= 5) {
                tmpval += 1;
            }
            val = tmpval;
        }
    }

    void ceil(decimal& val)
    {
        if (auto exp = val.raw_exp(); exp < 0) {
            val = std::ceil(val.get<double>());
        }
    }

    void floor(decimal& val)
    {
        if (auto exp = val.raw_exp(); exp < 0) {
            val = std::floor(val.get<double>());
        }
    }
    */
}
