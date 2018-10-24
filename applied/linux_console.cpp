//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <atomic>

#include <boost/exception/diagnostic_information.hpp>

#include "applied/scoped_services.hpp"
#include "sonia/utility/posix/signals.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/thread.hpp"
#include "sonia/exceptions.hpp"

std::atomic<long> barrier_(0);
std::atomic<scoped_services*> serv_ = nullptr;

void interuption_handler(int signum) {
    if (0 == barrier_.fetch_add(1)) {
        SCOPE_EXIT([]() { --barrier_; });
        scoped_services* pservs = serv_.load();
        if (pservs) {
            try {
                pservs->stop();
            } catch (...) {
                std::cerr << boost::current_exception_diagnostic_information();
            }
        }
    }
}

void terminate_impl() {
    std::cerr << "unexpected fatal error (terminate)\n";
    abort();
}


int main(int argc, char const* argv[]) {
    std::set_terminate(terminate_impl);

    try {
        scoped_services s(argc, argv);

        SCOPE_EXIT([]() {
            while (0 != barrier_.fetch_add(1)) {
                --barrier_;
                sonia::this_thread::sleep_for(boost::chrono::milliseconds(10));
            }
            serv_.store(nullptr);
        });

        serv_.store(&s);

        sonia::posix::set_interruption_handler(&interuption_handler);

        s.run();
    } catch (sonia::shutdown_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (sonia::exception const& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (std::exception const& e) {
        std::cerr << boost::diagnostic_information(e);
        return 1;
    } catch (...) {
        std::cerr << "unspecified error : " << boost::current_exception_diagnostic_information();
        return 1;
    }

    return 0;
}
