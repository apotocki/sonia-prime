//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <chrono>

#include <boost/intrusive/set.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/global_fun.hpp>

#include "sonia/concurrency.hpp"
#include "sonia/services/service.hpp"
#include "sonia/services/scheduler/scheduler.hpp"
#include "sonia/services/auth/authentication.hpp"
#include "sonia/net/http/application.hpp"
#include "sonia/utility/object_pool.hpp"
#include "http_digest_authentication_application_configuration.hpp"

namespace sonia::services {

struct http_session
{
    using hook_type = boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link> >;

    hook_type session_hook_;
    hook_type lifetime_hook_;
    hook_type user_hook_;

    std::string session_id_;
    std::string user;
    std::string digest;
    std::chrono::steady_clock::time_point start;

    //scheduler_task_handle timeout_handler_;

    explicit http_session(std::string session_id) 
        : session_id_{std::move(session_id)}
        , start{std::chrono::steady_clock::now()}
    {}

    auto lifetime(std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now()) const
    {
        return tp - start;
    }

    struct session_compare_type
    {
        bool operator()(http_session const& lhs, http_session const& rhs) const
        {
            return lhs.session_id_ < rhs.session_id_;
        }
    };

    struct lifetime_compare_type
    {
        bool operator()(http_session const& lhs, http_session const& rhs) const
        {
            return lhs.start < rhs.start;
        }
    };

    struct user_compare_type
    {
        bool operator()(http_session const& lhs, http_session const& rhs) const
        {
            return lhs.user < rhs.user;
        }
    };
};


struct http_digest_authentication_application
    : public service
    , public http::application
    , public authentication::authenticator
{
public:
    explicit http_digest_authentication_application(http_digest_authentication_application_configuration const& cfg);
    ~http_digest_authentication_application() override;

    void close() noexcept override final;
    void handle(http::request & req, http::response & resp) override final;

    std::string get_digest_for(string_view, string_view password) const override final;
    string_view get_realm() const override final;
    void clear_sessions() override final;

private:
    std::string get_nonce();
    bool remove_nonce(string_view nval);

private:
    http_digest_authentication_application_configuration cfg_;
    shared_ptr<scheduler> scheduler_;
    shared_ptr<authentication::digest_provider> digest_provider_;
    std::string digest_opaque_;
    mutable fibers::mutex routes_mutex_;
    fibers::mutex closing_mtx_;
    bool closing_{false};

    using sessions_type = boost::intrusive::set<
        http_session,
        boost::intrusive::member_hook<
              http_session
            , http_session::hook_type
            , &http_session::session_hook_
        >,
        boost::intrusive::compare<http_session::session_compare_type>
    >;
    sessions_type sessions_;
    fibers::mutex session_mutex_;

    using lifetimes_type = boost::intrusive::multiset<
        http_session,
        boost::intrusive::member_hook<
              http_session
            , http_session::hook_type
            , &http_session::lifetime_hook_
        >,
        boost::intrusive::compare<http_session::lifetime_compare_type>
    >;
    lifetimes_type lifetimes_;

    using users_type = boost::intrusive::multiset<
        http_session,
        boost::intrusive::member_hook<
              http_session
            , http_session::hook_type
            , &http_session::user_hook_
        >,
        boost::intrusive::compare<http_session::user_compare_type>
    >;
    users_type users_;

    using nonce_decriptor_t = std::tuple<std::string, std::chrono::steady_clock::time_point>;
    static std::string const& get_nonce(nonce_decriptor_t const& nd) { return std::get<0>(nd); }
    static std::chrono::steady_clock::time_point get_time_point(nonce_decriptor_t const& nd) { return std::get<1>(nd); }

    using nonces_t = boost::multi_index::multi_index_container<
        nonce_decriptor_t,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::global_fun<nonce_decriptor_t const&, std::string const&, &get_nonce>,
                hasher
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::global_fun<nonce_decriptor_t const&, std::chrono::steady_clock::time_point, &get_time_point>
            >
        >
    >;
      
    nonces_t nonces_, approved_nonces_;
    fibers::mutex nonce_mutex_;

    object_pool<http_session> session_pool_;
};

}
