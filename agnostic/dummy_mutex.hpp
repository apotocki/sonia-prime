/// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_DUMMY_MUTEX_HPP
#define AGNOSTIC_DUMMY_MUTEX_HPP

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

#endif // AGNOSTIC_DUMMY_MUTEX_HPP
