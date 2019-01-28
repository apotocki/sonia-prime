//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "factory.hpp"

#include <aio.h>
#include "sonia/utility/posix/signals.hpp"

#include "sonia/optional.hpp"

//#include "sonia/utility/windows.hpp"
//#include "sonia/utility/scope_exit.hpp"
//#include "sonia/utility/object_pool.hpp"
//#include "sonia/exceptions.hpp"
//#include <memory>
//#include <iostream>
//#include <boost/integer_traits.hpp>
//#include <boost/fiber/mutex.hpp>
//#include <boost/fiber/condition_variable.hpp>

namespace sonia { namespace io {

struct lin_impl_data
{
    friend class factory;
};

#define dataptr reinterpret_cast<lin_impl_data*>(impl_data_)

factory::factory() 
    : impl_data_(nullptr)
{
    
}

factory::~factory()
{
    delete dataptr;
}

void factory::open(uint32_t thr_cnt)
{
    if (impl_data_) {
        throw internal_error("io factory is already initialized");
    }
    
    impl_data_ = new lin_impl_data;// (shared_from_this(), thr_cnt);
    //dataptr->run();
}

void factory::close()
{
    if (impl_data_) {
        //dataptr->stop();
    }
}

std::string factory::name() const
{
    return std::string("linux");
}

void factory::thread_proc() {
   
}

// tcp_socket_factory
tcp_socket factory::create_tcp_socket(string_view address, uint16_t port, tcp_socket_type dt)
{
    //throw exception("can't create socket with address: '%1%' and port: '%2%'"_fmt % address % port);
    throw not_implemented_error("create_tcp_socket");
}

size_t factory::tcp_socket_count(tcp_socket_type) const
{
    throw not_implemented_error("tcp_socket_count");
}

// tcp_socket_service
void factory::tcp_socket_close(void * handle)
{
    throw not_implemented_error("tcp_socket_close");
}

size_t factory::tcp_socket_read_some(void * handle, void * buff, size_t sz)
{
    throw not_implemented_error("tcp_socket_read_some");
}

void factory::tcp_socket_async_read_some(void * soc, void * buff, size_t sz, function<void(std::error_code const&, size_t)> const& ftor)
{
    throw not_implemented_error("tcp_socket_async_read_some");
}

size_t factory::tcp_socket_write_some(void * handle, void const* buff, size_t sz)
{
    throw not_implemented_error("tcp_socket_write_some");
}

tcp_acceptor factory::create_tcp_acceptor(string_view address, uint16_t port, tcp_socket_type dt)
{
    throw not_implemented_error("create_tcp_acceptor");
}

void factory::tcp_acceptor_async_accept_and_read_some(void * handle, void * buff, size_t sz, acceptor_functor const& func)
{
    throw not_implemented_error("tcp_acceptor_async_accept_and_read_some");
}

void factory::tcp_acceptor_close(void * handle) noexcept
{
    throw not_implemented_error("tcp_acceptor_close");
}

file factory::open_file(cstring_view path, file_open_mode fom, file_access_mode fam, file_bufferring_mode fbm)
{
    int flags = O_LARGEFILE | O_NOATIME;
    int fmode = 0;
    switch (fom) {
    case file_open_mode::create:
        flags |= (O_CREAT | O_EXCL);
        break;
    case file_open_mode::create_or_open:
        flags |= O_CREAT;
        break;
    case file_open_mode::open:
        break;
    default:
        throw internal_error("unknown file open mode: %1%, file: %2%"_fmt % (int)fom % path);
    }

    switch (fam) {
    case file_access_mode::write:
        flags |= O_RDWR;
        fmode |= S_IRUSR | S_IWUSR;
        break;
    case file_access_mode::read:
        flags |= O_RDONLY;
        fmode |= S_IRUSR;
        break;
    default:
        throw internal_error("unknown file access mode: %1%, file: %2%"_fmt % (int)fam % path);
    }

    switch (fbm) {
    case file_bufferring_mode::buffered:
        break;
    case file_bufferring_mode::not_buffered:
        flags |= O_SYNC | O_DIRECT;
        break;
    default:
        throw internal_error("unknown file buffering mode: %1%, file: %2%"_fmt % (int)fbm % path);
    }

    int h = ::open(path.c_str(), flags, fmode);
    if (-1 == h) {
        int err = errno;
        throw exception("can't open file %1%, error: %2%"_fmt % path % strerror(err));
    }

    if (file_access_mode::write == fam) {
        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        /* Place a write lock on the file. */
        if (-1 == fcntl(h, F_SETLK, &lock)) {
            int err = errno;
            ::close(h);
            throw exception("can't write lock file %1%, error: %2%"_fmt % path % strerror(err));
        }
    }

    return file_access::open_file(shared_from_this(), h, path);
}

struct file_callback
{
    aiocb cb;
    sonia::posix::user_handler_type h_;

    optional<std::error_code> code;

    fibers::mutex mtx;
    fibers::condition_variable_any cnd;

    template <class BuffT>
    explicit file_callback(uint64_t fileoffset, BuffT buff)
    {
        memset(&cb, 0, sizeof(aiocb));
        cb.aio_offset = fileoffset;
        cb.aio_buf = (void*)buff.begin();
        cb.aio_nbytes = buff.size();
        cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        cb.aio_sigevent.sigev_signo = sonia::posix::get_user_signal();
        cb.aio_sigevent.sigev_value.sival_ptr = &h_;
        h_ = [this] { on_op(); };
    }

    void on_op()
    {
        auto lck = make_unique_lock(mtx);
        int err = aio_error(&cb) ? errno : 0;
        code.emplace(err, std::system_category());
        cnd.notify_one();
    }

    void wait()
    {
        auto lck = make_unique_lock(mtx);
        cnd.wait(lck, [this] { return !!code; });
        if (code && *code) {
            throw exception(code->message());
        }
    }
};

size_t factory::file_read(int handle, uint64_t fileoffset, array_view<char> dest)
{
    file_callback fcb(fileoffset, dest);
    fcb.cb.aio_fildes = handle;
    
    int r = aio_read(&fcb.cb);
    if (-1 == r) {
        int err = errno;
        throw exception("read file error : %1%"_fmt % strerror(err));
    }
    fcb.wait();
    ssize_t rsz = aio_return(&fcb.cb);
    if (rsz < 0) {
        int err = errno;
        throw exception("read file error : %1%"_fmt % strerror(err));
    }

    return (size_t)rsz;
}

size_t factory::file_write(int handle, uint64_t fileoffset, array_view<const char> src)
{
    file_callback fcb(fileoffset, src);
    fcb.cb.aio_fildes = handle;

    int r = aio_write(&fcb.cb);
    if (-1 == r) {
        int err = errno;
        throw exception("write file error : %1%"_fmt % strerror(err));
    }
    fcb.wait();
    ssize_t rsz = aio_return(&fcb.cb);
    if (rsz < 0) {
        int err = errno;
        throw exception("write file error : %1%"_fmt % strerror(err));
    }

    return (size_t)rsz;
}

}}
