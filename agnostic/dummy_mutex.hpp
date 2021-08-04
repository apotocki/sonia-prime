/// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace agnostic {

struct dummy_mutex
{
    void lock() {}
    void unlock() {}
    void lock_shared() {}
    void unlock_shared() {}
};

}
