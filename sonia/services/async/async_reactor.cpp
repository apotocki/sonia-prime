//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "reactor.hpp"


///////////////////
//#include <SDKDDKVer.h>
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
//#include <ioapiset.h>

namespace sonia { namespace services { namespace async {

reactor::reactor(uint32_t thr_cnt) {
    handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, thr_cnt);

    threads_.reserve(thr_cnt);
    for (size_t i = 0; i < thr_cnt; ++i) {
        threads_.push_back(thread([this] { this->thread_proc(); }));
    }
}

reactor::~reactor() {
    for (thread & t : threads_) {
        t.join();
    }
}

void reactor::thread_proc() {
    for (;;)
    {
        DWORD bytes;
        ULONG_PTR key;
        LPOVERLAPPED overlapped;

        if (!GetQueuedCompletionStatus(handle_, &bytes, &key, &overlapped, INFINITE)) {

        }
    }
}

}}}