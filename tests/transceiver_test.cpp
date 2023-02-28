//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <vector>

#include "sonia/filesystem.hpp"
#include "sonia/services.hpp"

#include "applied/scoped_services.hpp"

#include "sonia/utility/marshaling/marshaling.hpp"
#include "sonia/utility/marshaling/service.hpp"
#include "sonia/utility/marshaling/string.hpp"

#include "sonia/services/diagnostic.hpp"
#include "sonia/services/transceiver.hpp"
#include "sonia/services/scheduler/scheduler.hpp"
#include "sonia/utility/streaming/vector.hpp"

#include "sonia/utility/serialization/service.hpp"
#include "sonia/utility/serialization/pair.hpp"
#include "sonia/utility/serialization/string.hpp"
#include "sonia/utility/serialization/vector.hpp"
#include <boost/exception/diagnostic_information.hpp>

#include "applied/sonia_test.hpp"

namespace fs = sonia::fs;

#define TEST_FOLDER "cmd_transceiver_test"

using namespace std::chrono_literals;
using namespace sonia;
namespace serv = sonia::services;

namespace {

class itest_service : public service
{
public:
    itest_service() = default;

    virtual void empty_method() const = 0;
    virtual void exception_method() const = 0;
    virtual std::pair<std::string, std::vector<std::string> > some_method(int ival, std::string const& str, std::vector<std::string> & result) = 0;
    virtual void message_method(int val) = 0;
    virtual int get_value_method() const = 0;
};

SONIA_DECLARE_BINDING_TAG(ts_empty_method, &itest_service::empty_method);
SONIA_DECLARE_BINDING_TAG(ts_exception_method, &itest_service::exception_method);
SONIA_DECLARE_BINDING_TAG(ts_some_method, &itest_service::some_method);
SONIA_DECLARE_BINDING_TAG(ts_message_method, &itest_service::message_method);
SONIA_DECLARE_BINDING_TAG(ts_get_value_method, &itest_service::get_value_method);

class test_service : public itest_service
{
public:
    test_service() : val_(0) {}

    std::pair<std::string, std::vector<std::string> > some_method(int ival, std::string const& str, std::vector<std::string> & result) override
    {
        std::pair<std::string, std::vector<std::string> > mainresult(str, result);
        result.push_back(std::to_string(ival));
        return mainresult;
    }

    void empty_method() const override
    {

    }

    void exception_method() const override
    {
        throw std::runtime_error("exception_method");
    }

    void message_method(int val) override
    {
        val_.fetch_add(val);
    }

    int get_value_method() const override
    {
        return val_.load();
    }

private:
    std::atomic<int> val_;
};

class test_service_proxy : public itest_service
{
public:
    void open() override
    {
        transmitter_ = sonia::services::locate<sonia::services::transceiver>("transceiver.serv");
        tcpaddr_ = "tcp://localhost:2222";
    }

    void empty_method() const override
    {
        return serv::transmit_end_receive<ts_empty_method>(transmitter_, tcpaddr_, *this);
    }

    void exception_method() const override
    {
        return serv::transmit_end_receive<ts_exception_method>(transmitter_, tcpaddr_, *this);
    }

    std::pair<std::string, std::vector<std::string> > some_method(int ival, std::string const& str, std::vector<std::string> & result) override
    {
        return serv::transmit_end_receive<ts_some_method>(transmitter_, tcpaddr_, *this, ival, str, result);
    }

    void message_method(int val) override
    {
        return serv::transmit_end_receive<ts_message_method>(transmitter_, tcpaddr_, *this, val);
    }

    int get_value_method() const override {
        return serv::transmit_end_receive<ts_get_value_method>(transmitter_, tcpaddr_, *this);
    }

private:
    std::string tcpaddr_;
    shared_ptr<sonia::services::transceiver> transmitter_;
};

void get_configuration(std::ostream & os)
{
     os << 
        "{"
        "   hosts: ["
        "       {"
        "           name: 'server',"
        "           services: ['test_service', 'net.serv']"
        "       },"
        "       {"
        "           name: 'client',"
        "           services: ['test_service']"
        "       }"
        "   ],"
        "   services: {"
        "       transceiver.serv: {"
        "           factory: 'transceiver-factory',"
        "           layer: 16,"
        "           parameters: {"
        "              socket-factory: 'io-cache.serv'"
        "           }"
        "       },"
        "       io.serv: {"
        "           factory: 'io-factory',"
        "           layer: 8,"
        "           parameters: { scheduler: 'scheduler.serv', threads: 1 }"
        "       },"
        "       io-cache.serv: {"
        "           factory: 'io-cache-factory',"
        "           layer: 12,"
        "           parameters: { tcp-socket-factory: 'io.serv', max-connection-count: 128, per-route-max-connection-count: 128 }"
        "       },"
        "       net.serv: {"
        "           factory: 'net-server-factory',"
        "           layer: 14,"
        "           parameters: {"
        "               tcp-server-socket-factory: 'io.serv',"
        "               scheduler: 'scheduler.serv',"
        "               listeners: ["
        "                   { connector: 'transceiver.serv', workers : 128, buffer-size: 65536, address : '0.0.0.0', port : 2222, type : 'tcp'}"
        "               ]"
        "           }"
        "       },"
        "       scheduler.serv: {"
        "           factory: 'scheduler-factory',"
        "           layer: 4,"
        "           parameters: { threads: 20, fibers: 8 }"
        "       },"
        "       transceiver-factory: { factory: 'prime', layer: 0, parameters: {name: 'transceiver'} },"
        "       net-server-factory: { factory: 'prime', layer: 0, parameters: { name: 'net-server' } },"
        "       io-factory: { factory: 'prime', layer: 0, parameters: { name: 'io' } },"
        "       io-cache-factory: { factory: 'prime', layer: 0, parameters: { name: 'io-cache' } },"
        "       scheduler-factory: { factory: 'prime', layer: 0, parameters: { name: 'scheduler' } }"
        "   },"
        "   bundles: {"
        "       prime: {lib: 'sonia-prime'}"
        "   }"
        "}";
}

#ifdef BOOST_WINDOWS
struct pause_alloc_hook {
    _CRT_ALLOC_HOOK newhook_;
    pause_alloc_hook() : newhook_(_CrtSetAllocHook(0)) { }

    ~pause_alloc_hook() {
        _CrtSetAllocHook(newhook_);
    }
};

int AllocHook(int allocType, void* userData, size_t size, int
    blockType, long requestNumber, const unsigned char* filename, int
    lineNumber)
{
    /*
    if (size == 131072) {
        pause_alloc_hook guard_;
        std::cout << size << "\n";
    }
    
    if (!size) return TRUE;
    pause_alloc_hook guard_;
    std::cout << size << "\n";
    */
    return TRUE;
}
#endif
}

void cmd_transceiver_test()
{
    //_CrtSetAllocHook(AllocHook);
    //_CrtSetBreakAlloc(10282);

    using namespace sonia;
    fs::remove_all(TEST_FOLDER);

    //std::cout << typeid(ts_some_method::stub_tuple_t).name();
    //return;
    try {
        scoped_services ss{"base-path=" TEST_FOLDER "/"};
#if 1
        SONIA_REGISTER_BINDING_TAG(ts_empty_method, "ts_empty_method", "test_service");
        SONIA_REGISTER_BINDING_TAG(ts_exception_method, "ts_exception_method", "test_service");
        SONIA_REGISTER_BINDING_TAG(ts_some_method, "ts_some_method", "test_service");
        SONIA_REGISTER_BINDING_TAG(ts_message_method, "ts_message_method", "test_service");
        SONIA_REGISTER_BINDING_TAG(ts_get_value_method, "ts_get_value_method", "test_service");

        services::register_service_factory("test_service", []() -> shared_ptr<service> {
            shared_ptr<service> serv;
            if (services::get_host()->get_name() == "server") {
                services::register_transceiver_invoker<ts_empty_method>();
                services::register_transceiver_invoker<ts_exception_method>();
                services::register_transceiver_invoker<ts_some_method>();
                services::register_transceiver_invoker<ts_message_method>();
                services::register_transceiver_invoker<ts_get_value_method>();
                serv = make_shared<test_service>();
            } else {
                serv = make_shared<test_service_proxy>();
            }
            return serv;
        });

        std::stringstream cfgss;
        get_configuration(cfgss);
        services::load_configuration(cfgss);

        this_thread::attach_host("server");
        shared_ptr<diagnostic> serv_iodiag = services::locate<diagnostic>("io.serv");

        this_thread::attach_host("client");
        auto ctl_proxy = services::locate<itest_service>("test_service");
        //this_thread::attach_host("server");
#endif
#if 1
        try {
            ctl_proxy->exception_method();
            BOOST_CHECK (false);
        } catch (std::exception const& e) {
            if (e.what() != std::string("exception_method")) {
                GLOBAL_LOG_DEBUG() << boost::diagnostic_information(e);
                BOOST_CHECK(e.what() == std::string("exception_method"));
            }
        }

        //boost::this_thread::sleep(boost::posix_time::milliseconds(300));

        ctl_proxy->empty_method();
#endif
        ///*
        int ival = 10;
        std::string str = "asd";
        std::vector<std::string> result;
        result.push_back("123"); result.push_back("234"); result.push_back("345");
        std::vector<std::string> maincopy = result;
#if 1
        for (int i = 0; i < 50; ++i)
        {
            std::vector<std::string> copy = result;
            auto main_result = ctl_proxy->some_method(ival, str, result);
            BOOST_CHECK_EQUAL (main_result.second, copy);
            BOOST_CHECK_EQUAL (result.back(), std::to_string(ival));
            BOOST_CHECK_EQUAL (main_result.first,  str);
            result.push_back(to_string("xxx%1%"_fmt % i));
        }
#endif
#if 1
#ifdef _DEBUG
    const int calls_count = 300;
    const size_t max_pass_count = 32;
#else
    const int calls_count = 1;
    const size_t max_pass_count = 32;
#endif

        shared_ptr<diagnostic> iodiag = services::locate<diagnostic>("io.serv");

        shared_ptr<scheduler> async = services::locate<scheduler>("scheduler.serv");
        std::atomic<long> tasks(0);
        std::unique_ptr<int[]> curs(new int[max_pass_count]);
        std::fill(curs.get(), curs.get() + max_pass_count, -1);

        for (int i = 0; i < max_pass_count; ++i) {
            ++tasks;
            async->post([tnum = i, &tasks, &maincopy, &ival, &str, ctl_proxy, calls_count, &curs](){
                try {
                    std::vector<std::string> result = maincopy;
                    for (int i = 0; i < calls_count; ++i)
                    {
                        //curs[tnum] = i;
                        std::vector<std::string> copy = result;
                        //    GLOBAL_LOG_TRACE() << "client " << tnum << ", before call " << i;
                        auto main_result = ctl_proxy->some_method(ival, str, result);
                        //    GLOBAL_LOG_TRACE() << "client " << tnum << ", after call " << i;
                        BOOST_ASSERT (main_result.second == copy);
                        BOOST_ASSERT (result.back() == std::to_string(ival));
                        BOOST_ASSERT (main_result.first == str);
                        std::ostringstream ss;
                        ss << "xxx" << i;
                        result.push_back(ss.str());
                        curs[tnum] = i + 1;
                    }
                    --tasks;
                } catch (...) {
                    --tasks;
                    throw;
                }
                GLOBAL_LOG_TRACE() << "finished: " << tnum;
            });
        }

        while (tasks.load() > 0) {
            boost::thread::sleep(boost::posix_time::microsec_clock::universal_time() + boost::posix_time::microseconds(2000000));
            // print diagnostic
            //*
            std::ostringstream oss;
            for (int i = 0; i < max_pass_count; ++i) {
                oss << "task: " << i << ", cursor: " << curs[i] << ", from: " << calls_count << "\n";
            }
            oss << "client: " << iodiag->get_diagnostic_info() << "\n";
            oss << "server: " << serv_iodiag->get_diagnostic_info();
            GLOBAL_LOG_TRACE() << "DIAGNOSTIC:\n" << oss.str();
            //*/
        }
#endif
    } catch (closed_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << boost::diagnostic_information(e) << "\n";
        BOOST_REQUIRE(false);
    }
}

void transceiver_test_registrar()
{
    register_test(BOOST_TEST_CASE(&cmd_transceiver_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(transceiver_test_registrar)
#endif
