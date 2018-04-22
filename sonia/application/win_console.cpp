/*=============================================================================
    Copyright (c) 2017 Avicomp
    Author: Alexander A Potocki
==============================================================================*/
#include "margot/config.hpp"

#include <windows.h>

#include <boost/exception/diagnostic_information.hpp>

#include "margot/applications/host.hpp"
#include "margot/exceptions/exception.hpp"

#include MARGOT_APPLIED_FACADE_INCLUDE

BOOL WINAPI TermhandlerRoutine(DWORD ctrl_type)
{
    switch (ctrl_type)
    {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_SHUTDOWN_EVENT: {
            margot::applications::terminate();
            return TRUE;
        }
        default:
            return FALSE;
    }
}

using namespace margot;

void terminate_impl() {
    std::cerr << "margot terminate called\n";
    abort();
}

void unexpected_impl() {
    std::cerr << "margot unexpected called\n";
    throw;
}

int main(int argc, char const* argv[])
{
    std::set_terminate(terminate_impl);
    std::set_unexpected(unexpected_impl);


    try {
        SetConsoleCtrlHandler(TermhandlerRoutine, TRUE);

        margot::applications::add_listener(
            margot::applications::host_phase::BEFORE_START, 
            [&argc, &argv]() {
                typedef MARGOT_APPLIED_FACADE facade_type;
                auto main_facade = margot::make_shared<facade_type>();
                int result = main_facade->load(argc, argv, nullptr, margot::applications::get_host_configuration_for_modification());
                if (result) throw margot::silence_exception();
                return true; // permanent listener
            }
        );

        if (margot::applications::start_host()) {
            margot::applications::event_loop();
        }
    } catch (std::exception const& e) {
        std::cerr << boost::diagnostic_information(e);
    } catch (...) {
        std::cerr << "unspecified error : " << boost::current_exception_diagnostic_information();
    }
    return 0;
}
