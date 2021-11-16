//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#include "sonia/config.hpp"

#include <windows.h>

#include <iostream>
#include <exception>

#include <boost/exception/diagnostic_information.hpp>

#include "applied/scoped_services.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/exceptions.hpp"

std::atomic<long> barrier_(0);
std::atomic<scoped_services*> serv_ = nullptr;

BOOL WINAPI TermhandlerRoutine(DWORD ctrl_type)
{
    switch (ctrl_type)
    {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_SHUTDOWN_EVENT: {
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
            return TRUE;
        }
        default:
            return FALSE;
    }
}

void terminate_impl()
{
    std::cerr << "unexpected fatal error (terminate)\n";
    abort();
}

void unexpected_impl()
{
    std::cerr << "unexpected error (unexpected), trying to retrow\n";
    throw;
}

int main(int argc, char const* argv[])
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode (_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile (_CRT_WARN, _CRTDBG_FILE_STDOUT);
    
    SetConsoleCtrlHandler(TermhandlerRoutine, TRUE);
    SetConsoleOutputCP(65001);

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
        s.run();
    } catch (sonia::closed_exception const& e) {
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
