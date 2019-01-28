//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "sonia/services.hpp"

#include "applied/scoped_services.hpp"

#include "sonia/utility/type_durable_id.hpp"
#include "sonia/utility/bind_command.hpp"
#include "sonia/services/transceiver.hpp"

#include "sonia/utility/serialization/service.hpp"
#include "sonia/utility/serialization/string.hpp"
#include "sonia/utility/serialization/vector.hpp"

namespace fs = boost::filesystem;

#define TEST_FOLDER "cmd_transceiver_test"

using namespace sonia;
namespace serv = sonia::services;

namespace {

class itest_service : public service
{
public:
    itest_service() { }

    virtual void empty_method() const = 0;
    virtual void exception_method() const = 0;
    virtual std::pair<std::string, std::vector<std::string> > some_method(int ival, std::string const& str, std::vector<std::string> & result) = 0;
    virtual void message_method(int val) = 0;
    virtual int get_value_method() const = 0;
};

SONIA_DECLARE_BINDING_TAG(ts_empty_method, &itest_service::empty_method)
SONIA_DECLARE_BINDING_TAG(ts_exception_method, &itest_service::exception_method)
SONIA_DECLARE_BINDING_TAG(ts_some_method, &itest_service::some_method)
SONIA_DECLARE_BINDING_TAG(ts_message_method, &itest_service::message_method)
SONIA_DECLARE_BINDING_TAG(ts_get_value_method, &itest_service::get_value_method)

class test_service : public itest_service
{
public:
    test_service() : val_(0) {}

    std::pair<std::string, std::vector<std::string> > some_method(int ival, std::string const& str, std::vector<std::string> & result) override
    {
        std::pair<std::string, std::vector<std::string> > mainresult(str, result);
        result.push_back(boost::lexical_cast<std::string>(ival));
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
        tcpaddr_ = "localhost";
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

void get_configuration(std::ostream & os) {
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
        "              socket-factory: 'io.serv'"
        "           }"
        "       },"
        "       io.serv: {"
        "           factory: 'io-factory',"
        "           layer: 16,"
        "           parameters: {"
        "               threads: 8"
        "           }"
        "       },"
        "       net.serv: {"
        "           factory: 'net-server-factory',"
        "           layer: 16,"
        "           parameters: {"
        "               acceptor-factory: 'io.serv',"
        "               scheduler: 'scheduler.serv',"
        "               listeners: ["
        "                   {"
        "                       connector: 'transceiver.serv',"
        "                       address: '0.0.0.0',"
        "                       port: 2222,"
        "                       type: 'tcp'"
        "                   }"
        "               ]"
        "           }"
        "       },"
        "       scheduler.serv: {"
        "           factory: 'scheduler-factory',"
        "           layer: 16,"
        "           parameters: {"
        "               threads: 8,"
        "               fibers: 8"
        "           }"
        "       },"
        "       transceiver-factory: {"
        "           factory: 'net',"
        "           layer: 8,"
        "           parameters: {name: 'transceiver'}"
        "       },"
        "       net-server-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'net-server' }"
        "       },"
        "       io-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'io' }"
        "       },"
        "       scheduler-factory: {"
        "           factory: 'prime',"
        "           layer: 0,"
        "           parameters: { name: 'scheduler' }"
        "       }"
        "   },"
        "   bundles: {"
        "       prime: {lib: 'sonia-prime'},"
        "       net: {lib: 'sonia-net'}"
        "   }"
        "}";
}

}

BOOST_AUTO_TEST_CASE (cmd_transceiver_test)
{
    using namespace sonia;

    fs::remove_all(TEST_FOLDER);

    try {
        scoped_services ss;
        SONIA_REGISTER_BINDING_TAG(ts_empty_method, "ts_empty_method", "prime");
        SONIA_REGISTER_BINDING_TAG(ts_exception_method, "ts_exception_method", "prime");
        SONIA_REGISTER_BINDING_TAG(ts_some_method, "ts_some_method", "prime");
        SONIA_REGISTER_BINDING_TAG(ts_message_method, "ts_message_method", "prime");
        SONIA_REGISTER_BINDING_TAG(ts_get_value_method, "ts_get_value_method", "prime");

        services::register_service_factory("test_service", []() -> service_descriptor {
            shared_ptr<service> serv;
            if (services::get_host()->get_name() == "server") {
                serv = make_shared<test_service>();
            } else {
                serv = make_shared<test_service_proxy>();
            }
            return {serv, 0};
        });

        std::stringstream cfgss;
        get_configuration(cfgss);
        services::load_configuration(cfgss);
        
        this_thread::attach_host("client");

        auto ctl_proxy = services::locate<itest_service>("test_service");

        try {
            ctl_proxy->exception_method();
            BOOST_CHECK (false);
        } catch (std::exception const& e) {
            if (e.what() != std::string("exception_method")) {
                GLOBAL_LOG_DEBUG() << e.what();
                BOOST_CHECK(e.what() == std::string("exception_method"));
            }
        }
        ctl_proxy->empty_method();

        /*
        type::durable_id v0 = type::durable_id::get<std::string>("stl-string");
        type::durable_id v1 = type::durable_id::get<int>("int");
        type::durable_id v2 = type::durable_id::get<std::string>();
        std::cout << v0 << ", " << v1 << "\n";
        */
        /*
        auto p = services::locate<itest_service>("test_service");
        BOOST_CHECK_EQUAL("job0", p->do_job());

        try {
            services::register_service_factory("asd", []() -> service_descriptor { throw 1; });
            BOOST_CHECK(false);
        } catch (internal_error const&) {}

        services::load_configuration("host.json");

        auto s0 = services::locate("scheduler.serv");
        */
    } catch (closed_exception const& e) {
        std::cout << e.what() << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << "\n";
        BOOST_REQUIRE(false);
    }
}