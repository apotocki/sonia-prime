//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "http_digest_authentication_application.hpp"

#include <boost/unordered_map.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "agnostic/std/concepts/integral.hpp"
#include <boost/conversion/hashes/md5.hpp>
#include <boost/conversion/base_xx/base16.hpp>
#include <boost/conversion/push_iterator.hpp>

#include "sonia/utility/functional/hash.hpp"
#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/scope_exit.hpp"
#include "sonia/singleton.hpp"
#include "sonia/services.hpp"


namespace sonia::services {

namespace {

template <class T>
class value_reference : public polymorphic_movable
{
public:
    virtual ~value_reference() = default;
    virtual void set(T&, string_view) const = 0;
};

template <class T>
class flag_value_reference : public value_reference<T>
{
    bool T::* ref_;
public:
    explicit flag_value_reference(bool T::* val) : ref_{val} {}
    void set(T& obj, string_view val) const override final { obj.*ref_ = true; }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(flag_value_reference)
};

template <class T>
class string_value_reference : public value_reference<T>
{
    string_view T::* ref_;
public:
    explicit string_value_reference(string_view T::* val) : ref_{val} {}
    void set(T& obj, string_view val) const override final { obj.*ref_ = val; }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(string_value_reference)
};

template <class T, size_t N>
class base16_value_reference : public value_reference<T>
{
    using ref_t = uint8_t(T::*)[N];
    ref_t ref_;
public:
    explicit base16_value_reference(ref_t val) : ref_{val} {}
    void set(T& obj, const string_view val) const override final
    {
        if (val.size() != 2 * N) return;
        using namespace boost::conversion;
        (cvt_push_iterator{ base16l | int8, static_cast<uint8_t*>(obj.*ref_) } << val).finish();
    }

    SONIA_POLYMORPHIC_MOVABLE_IMPL(base16_value_reference)
};

struct http_digest
{
    bool has_digest{false};
    uint8_t response[16];

    //uint8_t ncnum[4];
    string_view username, nonce, nc, cnonce, realm, opaque, uri, qop;

    bool operator()(string_view n, string_view qv);
};

struct http_digest_map : singleton
{
    using value_ref_t = automatic_polymorphic<value_reference<http_digest>, 4 * sizeof(void*)>;
    boost::unordered_map<std::string, value_ref_t, hasher> map_;
    http_digest_map()
    {
        map_["Digest"] = value_ref_t{in_place_type<flag_value_reference<http_digest>>, &http_digest::has_digest};
        map_["username"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::username};
        map_["realm"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::realm};
        map_["nonce"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::nonce};
        map_["uri"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::uri};
        map_["opaque"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::opaque};
        map_["qop"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::qop};
        map_["nc"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::nc};
        map_["cnonce"] = value_ref_t{in_place_type<string_value_reference<http_digest>>, &http_digest::cnonce};
        map_["response"] = value_ref_t{in_place_type<base16_value_reference<http_digest, 16>>, &http_digest::response};
    }

    bool operator()(http_digest * dig, string_view n, string_view qv) const
    {
        auto it = map_.find(n, hasher{}, string_equal_to{});
        if (it != map_.end()) {
            it->second->set(*dig, qv);
            return true;
        }
        return false;
    }
};

bool http_digest::operator()(string_view n, string_view qv)
{
    if (qv.size() >= 2 && ((qv.front() == '"' && qv.back() == '"') || (qv.front() == '\'' && qv.back() == '\''))) {
        qv.advance_front(1);
        qv.advance_back(-1);
    }
    return as_singleton<http_digest_map>()->operator()(this, n, qv);
}

struct nonce_comparer
{
    bool operator()(string_view key, http_session const& s) const { return key < s.session_id_; }
    bool operator()(http_session const& s, string_view key) const { return s.session_id_ < key; }
};

std::string generate_nonce(size_t sz)
{
    const char symbols[] = "0123456789abcdef";
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, 15);
    std::string result;
    result.reserve(sz);
    for (size_t i = 0; i < sz; ++i) {
        result.push_back(symbols[index_dist(rng)]);
    }
    return std::move(result);
}


} // anonymous

http_digest_authentication_application::http_digest_authentication_application(http_digest_authentication_application_configuration const& cfg)
    : cfg_{cfg}, session_pool_{16}
{
    set_log_attribute("Type", "http-digest-authenticator");

    digest_opaque_ = generate_nonce(32);
}

http_digest_authentication_application::~http_digest_authentication_application()
{
    while (!lifetimes_.empty()) {
        auto lit = lifetimes_.begin();
        http_session * s = &*lit;
        LOG_TRACE(logger()) << "session: " << s->session_id_ << " has been closed";
        sessions_.erase(*s);
        lifetimes_.erase(lit);
        users_.erase(*s);
        session_pool_.delete_object(s);
    }
}

void http_digest_authentication_application::close() noexcept
{
    lock_guard guard(closing_mtx_);
    closing_ = true;
    digest_provider_.reset();
}

void http_digest_authentication_application::handle(http::request & req, http::response & resp)
{
    shared_ptr<http::application> app;
    cstring_view uri = req.get_relative_uri();
    for (auto const& r : cfg_.routes) {
        if (regex_match(uri.c_str(), r.pathre)) {
            if (lock_guard guard(routes_mutex_); !r.application) {
                locate(r.application_name, r.application);
            }
            app = r.application;
            break;
        }
    }

    if (!app && cfg_.page404_application) {
        app = cfg_.page404_application;
    } else if (!app) {
        resp.make404(cfg_.page404_message);
        return;
    }

    shared_ptr<authentication::digest_provider> dp;

    if (lock_guard guard(closing_mtx_); !closing_) {
        if (!digest_provider_) {
            locate(cfg_.digest_app, digest_provider_);
        }
        dp = digest_provider_;
    }

    if (!dp) {
        throw closed_exception();
    }

    auto now = std::chrono::steady_clock::now();

    // handle expired sessions
    {
        lock_guard session_guard(session_mutex_);
        while (!lifetimes_.empty()) {
            auto lit = lifetimes_.begin();
            http_session * s = &*lit;
            if (s->lifetime(now) < cfg_.session_idle_lifetime) break;
            LOG_TRACE(logger()) << "session: " << s->session_id_ << " expired";
            sessions_.erase(*s);
            lifetimes_.erase(lit);
            users_.erase(*s);
            session_pool_.delete_object(s);
        }
    }

    http_digest dig;
    req.tokenize_header(http::header::AUTHORIZATION, [this, &dig](string_view n, string_view v, char d) {
        if (!dig(n, v)) {
            LOG_WARN(logger()) << "unparsed auth token: " << n << "=" << v;
        }
        return true;
    });

    while (dig.has_digest) {
        if (dig.realm != cfg_.digest_realm) {
            LOG_TRACE(logger()) << "wrong client realm value: " << dig.realm;
        }
        if (dig.opaque != digest_opaque_) {
            LOG_WARN(logger()) << "wrong client opaque value: " << dig.opaque;
            break;
        }

        std::string ha1;
        ha1.reserve(32);

        unique_lock session_lck(session_mutex_);
        auto it = sessions_.find(dig.nonce, nonce_comparer());
        if (it != sessions_.end()) {
            ha1 = it->digest;
            session_lck.unlock();
        } else { // authenticate
            session_lck.unlock();
            if (!remove_nonce(dig.nonce)) {
                LOG_TRACE(logger()) << "wrong digest nonce";
                break;
            }

            auto opt_digest = dp->get_digest_for(dig.username);
            if (!opt_digest) {
                LOG_WARN(logger()) << "unknown user " << dig.username;
                break;
            }
            ha1 = std::move(*opt_digest);
        }

        using namespace boost::conversion;

        std::array<uint8_t, 32> ha2str;
        auto ha2_it = cvt_push_iterator{ int8 | md5 | base16l, ha2str.begin() };
        ha2_it << to_string(req.method) << ':' << dig.uri;
        ha2_it.finish();

        std::array<uint8_t, 16> response;
        auto resp_it = cvt_push_iterator{ int8 | md5, response.begin() };
        resp_it << ha1 << ':' << dig.nonce << ':';
        if (dig.qop == "auth" || dig.qop == "auth-int") {
            resp_it << dig.nc << ':' << dig.cnonce << ':' << dig.qop << ':';
        }
        resp_it << ha2str;
        resp_it.finish();

        bool eq = std::equal(dig.response, dig.response + sizeof(dig.response), response.begin());
        if (!eq) {
            LOG_WARN(logger()) << "wrong digest response";
            break;
        }

        session_lck.lock();
        it = sessions_.find(dig.nonce, nonce_comparer());
        if (it != sessions_.end()) {
            lifetimes_.erase(*it);
            it->start = now;
            lifetimes_.insert(*it);
        } else {
            http_session * ps = session_pool_.new_object(to_string(dig.nonce));
            http_session * ps_pin = ps;
            SCOPE_EXIT([this, &ps_pin]{ if (ps_pin) session_pool_.delete_object(ps_pin); } );
            ps->digest = std::move(ha1);
            ps->user = to_string(dig.username);
            sessions_.insert(*ps);
            http_session * ps_pin2 = ps;
            SCOPE_EXIT([this, &ps_pin2]{ if (ps_pin2) sessions_.erase(*ps_pin2); } );
            lifetimes_.insert(*ps);
            http_session * ps_pin3 = ps;
            SCOPE_EXIT([this, &ps_pin3]{ if (ps_pin3) lifetimes_.erase(*ps_pin3); } );
            users_.insert(*ps);
            ps_pin3 = ps_pin2 = ps_pin = nullptr;
            size_t scnt = users_.count(*ps);
            if (scnt > cfg_.max_sessions_per_user) {
                LOG_WARN(logger()) << to_string("allowed sessions exceeded, user: '%1%', sessions: %2%"_fmt % dig.username % scnt);
                //break;
                //removing the most old session
                for (auto lit = lifetimes_.begin(); lit != lifetimes_.end(); ++lit) {
                    http_session* s = &*lit;
                    if (s->user != dig.username) continue;
                    sessions_.erase(*s);
                    lifetimes_.erase(lit);
                    users_.erase(*s);
                    session_pool_.delete_object(s);
                    break;
                }
            }
        }
        session_lck.unlock();

        req.properties.insert(std::pair{"user", json_value{dig.username}});
        req.properties.insert(std::pair{"session", json_value{dig.nonce}});
        
        app->handle(req, resp);

        return;
    }

    // skip request body if exists
    //req.input.close();
    for (; !req.input.empty(); ++req.input);
    //req.keep_alive = false;

    std::string nonce = get_nonce();
    resp.meet_request(req);
    resp.make401("Digest", cfg_.digest_realm, digest_opaque_, nonce);
}

std::string http_digest_authentication_application::get_nonce()
{
    std::string nonce = generate_nonce(32);
    
    auto now = std::chrono::steady_clock::now();

    lock_guard guard(nonce_mutex_);
    
    // remove expired nonces
    while (!nonces_.empty() && now - std::get<1>(*nonces_.get<1>().begin()) > cfg_.digest_nonce_timeout)
    {
        nonces_.get<1>().erase(nonces_.get<1>().begin());
    }

    BOOST_ASSERT (nonces_.insert(nonces_t::value_type{nonce, now}).second);

    return std::move(nonce);
}

bool http_digest_authentication_application::remove_nonce(string_view nval)
{
    lock_guard guard(nonce_mutex_);
    auto it = nonces_.find(nval, hasher(), string_equal_to());
    if (it != nonces_.end()) {
        nonces_.erase(it);
        return true;
    }
    return false;
}

std::string http_digest_authentication_application::get_digest_for(string_view user, string_view password) const
{
    using namespace boost::conversion;
    std::string result;
    result.reserve(32);
    auto cvt_it = cvt_push_iterator{ int8 | md5 | base16l, std::back_inserter(result) };
    (cvt_it << user << ':' << cfg_.digest_realm << ':' << password).finish();
    return result;
}

string_view http_digest_authentication_application::get_realm() const
{
    return cfg_.digest_realm;
}

void http_digest_authentication_application::clear_sessions()
{
    lock_guard session_guard(session_mutex_);
    while (!lifetimes_.empty()) {
        auto lit = lifetimes_.begin();
        http_session* s = &*lit;
        LOG_TRACE(logger()) << "session: " << s->session_id_ << " has been closed";
        sessions_.erase(*s);
        lifetimes_.erase(lit);
        users_.erase(*s);
        session_pool_.delete_object(s);
    }
}

}
