//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services.hpp"

#include <sys/types.h>
#include <dlfcn.h>
#include <signal.h>
#include <unistd.h>

namespace sonia { namespace sal {

void set_thread_name(sonia::thread::id tid, string_view name) {
    // do nothing
}

void set_thread_name(sonia::thread::id tid, std::string const& name) {
    // do nothing
}

typedef sonia::services::bundle*(get_bundle_fn)();

shared_ptr<sonia::services::bundle> load_bundle(string_view name) {
    std::string libname = "lib" + to_string(name) + ".so";
    void * handle = dlopen(
#ifndef __ANDROID__
        libname.c_str(),
#else 
        (bundles_path + libname).c_str(), // Android requires fullpath
#endif
        RTLD_LAZY
    );

    if (!handle) {
        throw internal_error("Cannot load bundle: %1%\n%2%"_fmt % libname % dlerror());
    }

    // reset errors
    dlerror();

    get_bundle_fn* fn_handle = (get_bundle_fn*)dlsym(handle, "get_bundle");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        throw internal_error("Cannot load symbol 'get_bundle' in %1% module\n%2%"_fmt % libname % dlsym_error);
    }
    shared_ptr<sonia::services::bundle> result = shared_ptr<sonia::services::bundle>(fn_handle());
    result->set_handle(handle);
    return std::move(result);
}

uint64_t file_size(int h) {
    struct stat filest;
    if (-1 == fstat(h, &filest)) {
        int err = errno;
        throw exception("get file size error : ", strerror(err));
    }
    return filest.st_size;
}

void delete_file(cstring_view path) {
    if (-1 == unlink(path.c_str())) {
        int err = errno;
        throw exception("can't delete file %1%, error : %2%"_fmt % path % strerror(err));
    }
}

}}
