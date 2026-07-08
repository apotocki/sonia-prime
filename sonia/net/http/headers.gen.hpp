//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

// Auto-generated file. Do not edit manually.
#pragma once


#include <string_view>
#include <stdexcept>
#include <ostream>
#include <algorithm>
#include <utility>

namespace sonia::http {

inline std::string_view to_string_view(header value) noexcept {
    using namespace std::string_view_literals;
    switch (value) {
    case header::A_IM: return "A-IM"sv;
    case header::ALPN: return "ALPN"sv;
    case header::AMP_CACHE_TRANSFORM: return "AMP-Cache-Transform"sv;
    case header::ACCEPT: return "Accept"sv;
    case header::ACCEPT_ADDITIONS: return "Accept-Additions"sv;
    case header::ACCEPT_CHARSET: return "Accept-Charset"sv;
    case header::ACCEPT_DATETIME: return "Accept-Datetime"sv;
    case header::ACCEPT_ENCODING: return "Accept-Encoding"sv;
    case header::ACCEPT_FEATURES: return "Accept-Features"sv;
    case header::ACCEPT_LANGUAGE: return "Accept-Language"sv;
    case header::ACCEPT_PATCH: return "Accept-Patch"sv;
    case header::ACCEPT_POST: return "Accept-Post"sv;
    case header::ACCEPT_RANGES: return "Accept-Ranges"sv;
    case header::ACCESS_CONTROL: return "Access-Control"sv;
    case header::ACCESS_CONTROL_ALLOW_CREDENTIALS: return "Access-Control-Allow-Credentials"sv;
    case header::ACCESS_CONTROL_ALLOW_HEADERS: return "Access-Control-Allow-Headers"sv;
    case header::ACCESS_CONTROL_ALLOW_METHODS: return "Access-Control-Allow-Methods"sv;
    case header::ACCESS_CONTROL_ALLOW_ORIGIN: return "Access-Control-Allow-Origin"sv;
    case header::ACCESS_CONTROL_MAX_AGE: return "Access-Control-Max-Age"sv;
    case header::ACCESS_CONTROL_REQUEST_HEADERS: return "Access-Control-Request-Headers"sv;
    case header::ACCESS_CONTROL_REQUEST_METHOD: return "Access-Control-Request-Method"sv;
    case header::AGE: return "Age"sv;
    case header::ALLOW: return "Allow"sv;
    case header::ALT_SVC: return "Alt-Svc"sv;
    case header::ALT_USED: return "Alt-Used"sv;
    case header::ALTERNATES: return "Alternates"sv;
    case header::APPLY_TO_REDIRECT_REF: return "Apply-To-Redirect-Ref"sv;
    case header::AUTHENTICATION_CONTROL: return "Authentication-Control"sv;
    case header::AUTHENTICATION_INFO: return "Authentication-Info"sv;
    case header::AUTHORIZATION: return "Authorization"sv;
    case header::C_EXT: return "C-Ext"sv;
    case header::C_MAN: return "C-Man"sv;
    case header::C_OPT: return "C-Opt"sv;
    case header::C_PEP: return "C-PEP"sv;
    case header::C_PEP_INFO: return "C-PEP-Info"sv;
    case header::CDN_LOOP: return "CDN-Loop"sv;
    case header::CACHE_CONTROL: return "Cache-Control"sv;
    case header::CAL_MANAGED_ID: return "Cal-Managed-ID"sv;
    case header::CALDAV_TIMEZONES: return "CalDAV-Timezones"sv;
    case header::CACHE_STATUS: return "Cache-Status"sv;
    case header::CLOSE: return "Close"sv;
    case header::COMPLIANCE: return "Compliance"sv;
    case header::CONNECTION: return "Connection"sv;
    case header::CONTENT_BASE: return "Content-Base"sv;
    case header::CONTENT_DISPOSITION: return "Content-Disposition"sv;
    case header::CONTENT_ENCODING: return "Content-Encoding"sv;
    case header::CONTENT_DIGEST: return "Content-Digest"sv;
    case header::CONTENT_ID: return "Content-ID"sv;
    case header::CONTENT_LANGUAGE: return "Content-Language"sv;
    case header::CONTENT_LENGTH: return "Content-Length"sv;
    case header::CONTENT_LOCATION: return "Content-Location"sv;
    case header::CONTENT_MD5: return "Content-MD5"sv;
    case header::CONTENT_RANGE: return "Content-Range"sv;
    case header::CONTENT_SCRIPT_TYPE: return "Content-Script-Type"sv;
    case header::CONTENT_STYLE_TYPE: return "Content-Style-Type"sv;
    case header::CONTENT_TRANSFER_ENCODING: return "Content-Transfer-Encoding"sv;
    case header::CONTENT_TYPE: return "Content-Type"sv;
    case header::CONTENT_VERSION: return "Content-Version"sv;
    case header::COOKIE: return "Cookie"sv;
    case header::COOKIE2: return "Cookie2"sv;
    case header::COST: return "Cost"sv;
    case header::DASL: return "DASL"sv;
    case header::DAV: return "DAV"sv;
    case header::DATE: return "Date"sv;
    case header::DEFAULT_STYLE: return "Default-Style"sv;
    case header::DELTA_BASE: return "Delta-Base"sv;
    case header::DEPTH: return "Depth"sv;
    case header::DERIVED_FROM: return "Derived-From"sv;
    case header::DESTINATION: return "Destination"sv;
    case header::DIFFERENTIAL_ID: return "Differential-ID"sv;
    case header::DIGEST: return "Digest"sv;
    case header::EDIINT_FEATURES: return "EDIINT-Features"sv;
    case header::ETAG: return "ETag"sv;
    case header::EARLY_DATA: return "Early-Data"sv;
    case header::EXPECT: return "Expect"sv;
    case header::EXPECT_CT: return "Expect-CT"sv;
    case header::EXPIRES: return "Expires"sv;
    case header::EXT: return "Ext"sv;
    case header::FORWARDED: return "Forwarded"sv;
    case header::FROM: return "From"sv;
    case header::GETPROFILE: return "GetProfile"sv;
    case header::HTTP2_SETTINGS: return "HTTP2-Settings"sv;
    case header::HOBAREG: return "Hobareg"sv;
    case header::HOST: return "Host"sv;
    case header::IM: return "IM"sv;
    case header::IF: return "If"sv;
    case header::IF_MATCH: return "If-Match"sv;
    case header::IF_MODIFIED_SINCE: return "If-Modified-Since"sv;
    case header::IF_NONE_MATCH: return "If-None-Match"sv;
    case header::IF_RANGE: return "If-Range"sv;
    case header::IF_SCHEDULE_TAG_MATCH: return "If-Schedule-Tag-Match"sv;
    case header::IF_UNMODIFIED_SINCE: return "If-Unmodified-Since"sv;
    case header::INCLUDE_REFERRED_TOKEN_BINDING_ID: return "Include-Referred-Token-Binding-ID"sv;
    case header::KEEP_ALIVE: return "Keep-Alive"sv;
    case header::LABEL: return "Label"sv;
    case header::LAST_MODIFIED: return "Last-Modified"sv;
    case header::LINK: return "Link"sv;
    case header::LOCATION: return "Location"sv;
    case header::LOCK_TOKEN: return "Lock-Token"sv;
    case header::MIME_VERSION: return "MIME-Version"sv;
    case header::MAN: return "Man"sv;
    case header::MAX_FORWARDS: return "Max-Forwards"sv;
    case header::MEMENTO_DATETIME: return "Memento-Datetime"sv;
    case header::MESSAGE_ID: return "Message-ID"sv;
    case header::METER: return "Meter"sv;
    case header::METHOD_CHECK: return "Method-Check"sv;
    case header::METHOD_CHECK_EXPIRES: return "Method-Check-Expires"sv;
    case header::NEGOTIATE: return "Negotiate"sv;
    case header::NON_COMPLIANCE: return "Non-Compliance"sv;
    case header::OSCORE: return "OSCORE"sv;
    case header::OPT: return "Opt"sv;
    case header::OPTIONAL_: return "Optional"sv;
    case header::OPTIONAL_WWW_AUTHENTICATE: return "Optional-WWW-Authenticate"sv;
    case header::ORDERING_TYPE: return "Ordering-Type"sv;
    case header::ORIGIN: return "Origin"sv;
    case header::OVERWRITE: return "Overwrite"sv;
    case header::P3P: return "P3P"sv;
    case header::PEP: return "PEP"sv;
    case header::PICS_LABEL: return "PICS-Label"sv;
    case header::PEP_INFO: return "Pep-Info"sv;
    case header::POSITION: return "Position"sv;
    case header::PRAGMA: return "Pragma"sv;
    case header::PREFER: return "Prefer"sv;
    case header::PREFERENCE_APPLIED: return "Preference-Applied"sv;
    case header::PROFILEOBJECT: return "ProfileObject"sv;
    case header::PROTOCOL: return "Protocol"sv;
    case header::PROTOCOL_INFO: return "Protocol-Info"sv;
    case header::PROTOCOL_QUERY: return "Protocol-Query"sv;
    case header::PROTOCOL_REQUEST: return "Protocol-Request"sv;
    case header::PROXY_AUTHENTICATE: return "Proxy-Authenticate"sv;
    case header::PROXY_AUTHENTICATION_INFO: return "Proxy-Authentication-Info"sv;
    case header::PROXY_AUTHORIZATION: return "Proxy-Authorization"sv;
    case header::PROXY_FEATURES: return "Proxy-Features"sv;
    case header::PROXY_INSTRUCTION: return "Proxy-Instruction"sv;
    case header::PUBLIC: return "Public"sv;
    case header::PUBLIC_KEY_PINS: return "Public-Key-Pins"sv;
    case header::PUBLIC_KEY_PINS_REPORT_ONLY: return "Public-Key-Pins-Report-Only"sv;
    case header::RANGE: return "Range"sv;
    case header::REDIRECT_REF: return "Redirect-Ref"sv;
    case header::REFERER: return "Referer"sv;
    case header::REFERER_ROOT: return "Referer-Root"sv;
    case header::REPLAY_NONCE: return "Replay-Nonce"sv;
    case header::REPR_DIGEST: return "Repr-Digest"sv;
    case header::RESOLUTION_HINT: return "Resolution-Hint"sv;
    case header::RESOLVER_LOCATION: return "Resolver-Location"sv;
    case header::RETRY_AFTER: return "Retry-After"sv;
    case header::SLUG: return "SLUG"sv;
    case header::SAFE: return "Safe"sv;
    case header::SCHEDULE_REPLY: return "Schedule-Reply"sv;
    case header::SCHEDULE_TAG: return "Schedule-Tag"sv;
    case header::SEC_CH_UA: return "Sec-CH-UA"sv;
    case header::SEC_CH_UA_MOBILE: return "Sec-CH-UA-Mobile"sv;
    case header::SEC_CH_UA_PLATFORM: return "Sec-CH-UA-Platform"sv;
    case header::SEC_FETCH_DEST: return "Sec-Fetch-Dest"sv;
    case header::SEC_FETCH_MODE: return "Sec-Fetch-Mode"sv;
    case header::SEC_FETCH_SITE: return "Sec-Fetch-Site"sv;
    case header::SEC_FETCH_USER: return "Sec-Fetch-User"sv;
    case header::SEC_TOKEN_BINDING: return "Sec-Token-Binding"sv;
    case header::SEC_WEBSOCKET_ACCEPT: return "Sec-WebSocket-Accept"sv;
    case header::SEC_WEBSOCKET_EXTENSIONS: return "Sec-WebSocket-Extensions"sv;
    case header::SEC_WEBSOCKET_KEY: return "Sec-WebSocket-Key"sv;
    case header::SEC_WEBSOCKET_PROTOCOL: return "Sec-WebSocket-Protocol"sv;
    case header::SEC_WEBSOCKET_VERSION: return "Sec-WebSocket-Version"sv;
    case header::SECURITY_SCHEME: return "Security-Scheme"sv;
    case header::SIGNATURE: return "Signature"sv;
    case header::SIGNATURE_INPUT: return "Signature-Input"sv;
    case header::SERVER: return "Server"sv;
    case header::SET_COOKIE: return "Set-Cookie"sv;
    case header::SET_COOKIE2: return "Set-Cookie2"sv;
    case header::SETPROFILE: return "SetProfile"sv;
    case header::SOAPACTION: return "SoapAction"sv;
    case header::STATUS_URI: return "Status-URI"sv;
    case header::STRICT_TRANSPORT_SECURITY: return "Strict-Transport-Security"sv;
    case header::SUBOK: return "SubOK"sv;
    case header::SUBST: return "Subst"sv;
    case header::SUNSET: return "Sunset"sv;
    case header::SURROGATE_CAPABILITY: return "Surrogate-Capability"sv;
    case header::SURROGATE_CONTROL: return "Surrogate-Control"sv;
    case header::TCN: return "TCN"sv;
    case header::TE: return "TE"sv;
    case header::TTL: return "TTL"sv;
    case header::TIMEOUT: return "Timeout"sv;
    case header::TIMING_ALLOW_ORIGIN: return "Timing-Allow-Origin"sv;
    case header::TITLE: return "Title"sv;
    case header::TOPIC: return "Topic"sv;
    case header::TRACEPARENT: return "Traceparent"sv;
    case header::TRACESTATE: return "Tracestate"sv;
    case header::TRAILER: return "Trailer"sv;
    case header::TRANSFER_ENCODING: return "Transfer-Encoding"sv;
    case header::UA_COLOR: return "UA-Color"sv;
    case header::UA_MEDIA: return "UA-Media"sv;
    case header::UA_PIXELS: return "UA-Pixels"sv;
    case header::UA_RESOLUTION: return "UA-Resolution"sv;
    case header::UA_WINDOWPIXELS: return "UA-Windowpixels"sv;
    case header::URI: return "URI"sv;
    case header::UPGRADE: return "Upgrade"sv;
    case header::UPGRADE_INSECURE_REQUESTS: return "Upgrade-Insecure-Requests"sv;
    case header::URGENCY: return "Urgency"sv;
    case header::USER_AGENT: return "User-Agent"sv;
    case header::VARIANT_VARY: return "Variant-Vary"sv;
    case header::VARY: return "Vary"sv;
    case header::VERSION: return "Version"sv;
    case header::VIA: return "Via"sv;
    case header::WWW_AUTHENTICATE: return "WWW-Authenticate"sv;
    case header::WANT_DIGEST: return "Want-Digest"sv;
    case header::WARNING: return "Warning"sv;
    case header::X_CACHE: return "X-Cache"sv;
    case header::X_CONTENT_TYPE_OPTIONS: return "X-Content-Type-Options"sv;
    case header::X_DEVICE_ACCEPT: return "X-Device-Accept"sv;
    case header::X_DEVICE_ACCEPT_CHARSET: return "X-Device-Accept-Charset"sv;
    case header::X_DEVICE_ACCEPT_ENCODING: return "X-Device-Accept-Encoding"sv;
    case header::X_DEVICE_ACCEPT_LANGUAGE: return "X-Device-Accept-Language"sv;
    case header::X_DEVICE_USER_AGENT: return "X-Device-User-Agent"sv;
    case header::X_FRAME_OPTIONS: return "X-Frame-Options"sv;
    case header::X_HTTP_METHOD_OVERRIDE: return "X-Http-Method-Override"sv;
    case header::X_POWERED_BY: return "X-Powered-By"sv;
    case header::X_REQUESTED_WITH: return "X-Requested-With"sv;
    case header::X_UA_COMPATIBLE: return "X-UA-Compatible"sv;
    case header::X_UIDH: return "X-UIDH"sv;
    case header::X_WAP_PROFILE: return "X-Wap-Profile"sv;
    case header::X_WEBKIT_CSP: return "X-WebKit-CSP"sv;
    case header::X_XSS_PROTECTION: return "X-XSS-Protection"sv;
    default: return ""sv;
    }
}

inline std::ostream& operator<<(std::ostream& os, header value) {
    return os << to_string_view(value);
}

inline bool is_valid(header value) noexcept {
    switch (value) {
    case header::A_IM:
    case header::ALPN:
    case header::AMP_CACHE_TRANSFORM:
    case header::ACCEPT:
    case header::ACCEPT_ADDITIONS:
    case header::ACCEPT_CHARSET:
    case header::ACCEPT_DATETIME:
    case header::ACCEPT_ENCODING:
    case header::ACCEPT_FEATURES:
    case header::ACCEPT_LANGUAGE:
    case header::ACCEPT_PATCH:
    case header::ACCEPT_POST:
    case header::ACCEPT_RANGES:
    case header::ACCESS_CONTROL:
    case header::ACCESS_CONTROL_ALLOW_CREDENTIALS:
    case header::ACCESS_CONTROL_ALLOW_HEADERS:
    case header::ACCESS_CONTROL_ALLOW_METHODS:
    case header::ACCESS_CONTROL_ALLOW_ORIGIN:
    case header::ACCESS_CONTROL_MAX_AGE:
    case header::ACCESS_CONTROL_REQUEST_HEADERS:
    case header::ACCESS_CONTROL_REQUEST_METHOD:
    case header::AGE:
    case header::ALLOW:
    case header::ALT_SVC:
    case header::ALT_USED:
    case header::ALTERNATES:
    case header::APPLY_TO_REDIRECT_REF:
    case header::AUTHENTICATION_CONTROL:
    case header::AUTHENTICATION_INFO:
    case header::AUTHORIZATION:
    case header::C_EXT:
    case header::C_MAN:
    case header::C_OPT:
    case header::C_PEP:
    case header::C_PEP_INFO:
    case header::CDN_LOOP:
    case header::CACHE_CONTROL:
    case header::CAL_MANAGED_ID:
    case header::CALDAV_TIMEZONES:
    case header::CACHE_STATUS:
    case header::CLOSE:
    case header::COMPLIANCE:
    case header::CONNECTION:
    case header::CONTENT_BASE:
    case header::CONTENT_DISPOSITION:
    case header::CONTENT_ENCODING:
    case header::CONTENT_DIGEST:
    case header::CONTENT_ID:
    case header::CONTENT_LANGUAGE:
    case header::CONTENT_LENGTH:
    case header::CONTENT_LOCATION:
    case header::CONTENT_MD5:
    case header::CONTENT_RANGE:
    case header::CONTENT_SCRIPT_TYPE:
    case header::CONTENT_STYLE_TYPE:
    case header::CONTENT_TRANSFER_ENCODING:
    case header::CONTENT_TYPE:
    case header::CONTENT_VERSION:
    case header::COOKIE:
    case header::COOKIE2:
    case header::COST:
    case header::DASL:
    case header::DAV:
    case header::DATE:
    case header::DEFAULT_STYLE:
    case header::DELTA_BASE:
    case header::DEPTH:
    case header::DERIVED_FROM:
    case header::DESTINATION:
    case header::DIFFERENTIAL_ID:
    case header::DIGEST:
    case header::EDIINT_FEATURES:
    case header::ETAG:
    case header::EARLY_DATA:
    case header::EXPECT:
    case header::EXPECT_CT:
    case header::EXPIRES:
    case header::EXT:
    case header::FORWARDED:
    case header::FROM:
    case header::GETPROFILE:
    case header::HTTP2_SETTINGS:
    case header::HOBAREG:
    case header::HOST:
    case header::IM:
    case header::IF:
    case header::IF_MATCH:
    case header::IF_MODIFIED_SINCE:
    case header::IF_NONE_MATCH:
    case header::IF_RANGE:
    case header::IF_SCHEDULE_TAG_MATCH:
    case header::IF_UNMODIFIED_SINCE:
    case header::INCLUDE_REFERRED_TOKEN_BINDING_ID:
    case header::KEEP_ALIVE:
    case header::LABEL:
    case header::LAST_MODIFIED:
    case header::LINK:
    case header::LOCATION:
    case header::LOCK_TOKEN:
    case header::MIME_VERSION:
    case header::MAN:
    case header::MAX_FORWARDS:
    case header::MEMENTO_DATETIME:
    case header::MESSAGE_ID:
    case header::METER:
    case header::METHOD_CHECK:
    case header::METHOD_CHECK_EXPIRES:
    case header::NEGOTIATE:
    case header::NON_COMPLIANCE:
    case header::OSCORE:
    case header::OPT:
    case header::OPTIONAL_:
    case header::OPTIONAL_WWW_AUTHENTICATE:
    case header::ORDERING_TYPE:
    case header::ORIGIN:
    case header::OVERWRITE:
    case header::P3P:
    case header::PEP:
    case header::PICS_LABEL:
    case header::PEP_INFO:
    case header::POSITION:
    case header::PRAGMA:
    case header::PREFER:
    case header::PREFERENCE_APPLIED:
    case header::PROFILEOBJECT:
    case header::PROTOCOL:
    case header::PROTOCOL_INFO:
    case header::PROTOCOL_QUERY:
    case header::PROTOCOL_REQUEST:
    case header::PROXY_AUTHENTICATE:
    case header::PROXY_AUTHENTICATION_INFO:
    case header::PROXY_AUTHORIZATION:
    case header::PROXY_FEATURES:
    case header::PROXY_INSTRUCTION:
    case header::PUBLIC:
    case header::PUBLIC_KEY_PINS:
    case header::PUBLIC_KEY_PINS_REPORT_ONLY:
    case header::RANGE:
    case header::REDIRECT_REF:
    case header::REFERER:
    case header::REFERER_ROOT:
    case header::REPLAY_NONCE:
    case header::REPR_DIGEST:
    case header::RESOLUTION_HINT:
    case header::RESOLVER_LOCATION:
    case header::RETRY_AFTER:
    case header::SLUG:
    case header::SAFE:
    case header::SCHEDULE_REPLY:
    case header::SCHEDULE_TAG:
    case header::SEC_CH_UA:
    case header::SEC_CH_UA_MOBILE:
    case header::SEC_CH_UA_PLATFORM:
    case header::SEC_FETCH_DEST:
    case header::SEC_FETCH_MODE:
    case header::SEC_FETCH_SITE:
    case header::SEC_FETCH_USER:
    case header::SEC_TOKEN_BINDING:
    case header::SEC_WEBSOCKET_ACCEPT:
    case header::SEC_WEBSOCKET_EXTENSIONS:
    case header::SEC_WEBSOCKET_KEY:
    case header::SEC_WEBSOCKET_PROTOCOL:
    case header::SEC_WEBSOCKET_VERSION:
    case header::SECURITY_SCHEME:
    case header::SIGNATURE:
    case header::SIGNATURE_INPUT:
    case header::SERVER:
    case header::SET_COOKIE:
    case header::SET_COOKIE2:
    case header::SETPROFILE:
    case header::SOAPACTION:
    case header::STATUS_URI:
    case header::STRICT_TRANSPORT_SECURITY:
    case header::SUBOK:
    case header::SUBST:
    case header::SUNSET:
    case header::SURROGATE_CAPABILITY:
    case header::SURROGATE_CONTROL:
    case header::TCN:
    case header::TE:
    case header::TTL:
    case header::TIMEOUT:
    case header::TIMING_ALLOW_ORIGIN:
    case header::TITLE:
    case header::TOPIC:
    case header::TRACEPARENT:
    case header::TRACESTATE:
    case header::TRAILER:
    case header::TRANSFER_ENCODING:
    case header::UA_COLOR:
    case header::UA_MEDIA:
    case header::UA_PIXELS:
    case header::UA_RESOLUTION:
    case header::UA_WINDOWPIXELS:
    case header::URI:
    case header::UPGRADE:
    case header::UPGRADE_INSECURE_REQUESTS:
    case header::URGENCY:
    case header::USER_AGENT:
    case header::VARIANT_VARY:
    case header::VARY:
    case header::VERSION:
    case header::VIA:
    case header::WWW_AUTHENTICATE:
    case header::WANT_DIGEST:
    case header::WARNING:
    case header::X_CACHE:
    case header::X_CONTENT_TYPE_OPTIONS:
    case header::X_DEVICE_ACCEPT:
    case header::X_DEVICE_ACCEPT_CHARSET:
    case header::X_DEVICE_ACCEPT_ENCODING:
    case header::X_DEVICE_ACCEPT_LANGUAGE:
    case header::X_DEVICE_USER_AGENT:
    case header::X_FRAME_OPTIONS:
    case header::X_HTTP_METHOD_OVERRIDE:
    case header::X_POWERED_BY:
    case header::X_REQUESTED_WITH:
    case header::X_UA_COMPATIBLE:
    case header::X_UIDH:
    case header::X_WAP_PROFILE:
    case header::X_WEBKIT_CSP:
    case header::X_XSS_PROTECTION:
        return true;
    default:
        return false;
    }
}

inline header parse_header(std::string_view str) {
    using namespace std::string_view_literals;
    static constexpr auto to_lower_ascii = [](char c) -> char {
        return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;
    };
    std::string lower_str(str.size(), '\0');
    std::transform(str.begin(), str.end(), lower_str.begin(), to_lower_ascii);
    std::string_view key = lower_str;
    static constexpr std::pair<std::string_view, header> table[] = {
        { "a-im"sv, header::A_IM },
        { "accept"sv, header::ACCEPT },
        { "accept-additions"sv, header::ACCEPT_ADDITIONS },
        { "accept-charset"sv, header::ACCEPT_CHARSET },
        { "accept-datetime"sv, header::ACCEPT_DATETIME },
        { "accept-encoding"sv, header::ACCEPT_ENCODING },
        { "accept-features"sv, header::ACCEPT_FEATURES },
        { "accept-language"sv, header::ACCEPT_LANGUAGE },
        { "accept-patch"sv, header::ACCEPT_PATCH },
        { "accept-post"sv, header::ACCEPT_POST },
        { "accept-ranges"sv, header::ACCEPT_RANGES },
        { "access-control"sv, header::ACCESS_CONTROL },
        { "access-control-allow-credentials"sv, header::ACCESS_CONTROL_ALLOW_CREDENTIALS },
        { "access-control-allow-headers"sv, header::ACCESS_CONTROL_ALLOW_HEADERS },
        { "access-control-allow-methods"sv, header::ACCESS_CONTROL_ALLOW_METHODS },
        { "access-control-allow-origin"sv, header::ACCESS_CONTROL_ALLOW_ORIGIN },
        { "access-control-max-age"sv, header::ACCESS_CONTROL_MAX_AGE },
        { "access-control-request-headers"sv, header::ACCESS_CONTROL_REQUEST_HEADERS },
        { "access-control-request-method"sv, header::ACCESS_CONTROL_REQUEST_METHOD },
        { "age"sv, header::AGE },
        { "allow"sv, header::ALLOW },
        { "alpn"sv, header::ALPN },
        { "alt-svc"sv, header::ALT_SVC },
        { "alt-used"sv, header::ALT_USED },
        { "alternates"sv, header::ALTERNATES },
        { "amp-cache-transform"sv, header::AMP_CACHE_TRANSFORM },
        { "apply-to-redirect-ref"sv, header::APPLY_TO_REDIRECT_REF },
        { "authentication-control"sv, header::AUTHENTICATION_CONTROL },
        { "authentication-info"sv, header::AUTHENTICATION_INFO },
        { "authorization"sv, header::AUTHORIZATION },
        { "c-ext"sv, header::C_EXT },
        { "c-man"sv, header::C_MAN },
        { "c-opt"sv, header::C_OPT },
        { "c-pep"sv, header::C_PEP },
        { "c-pep-info"sv, header::C_PEP_INFO },
        { "cache-control"sv, header::CACHE_CONTROL },
        { "cache-status"sv, header::CACHE_STATUS },
        { "cal-managed-id"sv, header::CAL_MANAGED_ID },
        { "caldav-timezones"sv, header::CALDAV_TIMEZONES },
        { "cdn-loop"sv, header::CDN_LOOP },
        { "close"sv, header::CLOSE },
        { "compliance"sv, header::COMPLIANCE },
        { "connection"sv, header::CONNECTION },
        { "content-base"sv, header::CONTENT_BASE },
        { "content-digest"sv, header::CONTENT_DIGEST },
        { "content-disposition"sv, header::CONTENT_DISPOSITION },
        { "content-encoding"sv, header::CONTENT_ENCODING },
        { "content-id"sv, header::CONTENT_ID },
        { "content-language"sv, header::CONTENT_LANGUAGE },
        { "content-length"sv, header::CONTENT_LENGTH },
        { "content-location"sv, header::CONTENT_LOCATION },
        { "content-md5"sv, header::CONTENT_MD5 },
        { "content-range"sv, header::CONTENT_RANGE },
        { "content-script-type"sv, header::CONTENT_SCRIPT_TYPE },
        { "content-style-type"sv, header::CONTENT_STYLE_TYPE },
        { "content-transfer-encoding"sv, header::CONTENT_TRANSFER_ENCODING },
        { "content-type"sv, header::CONTENT_TYPE },
        { "content-version"sv, header::CONTENT_VERSION },
        { "cookie"sv, header::COOKIE },
        { "cookie2"sv, header::COOKIE2 },
        { "cost"sv, header::COST },
        { "dasl"sv, header::DASL },
        { "date"sv, header::DATE },
        { "dav"sv, header::DAV },
        { "default-style"sv, header::DEFAULT_STYLE },
        { "delta-base"sv, header::DELTA_BASE },
        { "depth"sv, header::DEPTH },
        { "derived-from"sv, header::DERIVED_FROM },
        { "destination"sv, header::DESTINATION },
        { "differential-id"sv, header::DIFFERENTIAL_ID },
        { "digest"sv, header::DIGEST },
        { "early-data"sv, header::EARLY_DATA },
        { "ediint-features"sv, header::EDIINT_FEATURES },
        { "etag"sv, header::ETAG },
        { "expect"sv, header::EXPECT },
        { "expect-ct"sv, header::EXPECT_CT },
        { "expires"sv, header::EXPIRES },
        { "ext"sv, header::EXT },
        { "forwarded"sv, header::FORWARDED },
        { "from"sv, header::FROM },
        { "getprofile"sv, header::GETPROFILE },
        { "hobareg"sv, header::HOBAREG },
        { "host"sv, header::HOST },
        { "http2-settings"sv, header::HTTP2_SETTINGS },
        { "if"sv, header::IF },
        { "if-match"sv, header::IF_MATCH },
        { "if-modified-since"sv, header::IF_MODIFIED_SINCE },
        { "if-none-match"sv, header::IF_NONE_MATCH },
        { "if-range"sv, header::IF_RANGE },
        { "if-schedule-tag-match"sv, header::IF_SCHEDULE_TAG_MATCH },
        { "if-unmodified-since"sv, header::IF_UNMODIFIED_SINCE },
        { "im"sv, header::IM },
        { "include-referred-token-binding-id"sv, header::INCLUDE_REFERRED_TOKEN_BINDING_ID },
        { "keep-alive"sv, header::KEEP_ALIVE },
        { "label"sv, header::LABEL },
        { "last-modified"sv, header::LAST_MODIFIED },
        { "link"sv, header::LINK },
        { "location"sv, header::LOCATION },
        { "lock-token"sv, header::LOCK_TOKEN },
        { "man"sv, header::MAN },
        { "max-forwards"sv, header::MAX_FORWARDS },
        { "memento-datetime"sv, header::MEMENTO_DATETIME },
        { "message-id"sv, header::MESSAGE_ID },
        { "meter"sv, header::METER },
        { "method-check"sv, header::METHOD_CHECK },
        { "method-check-expires"sv, header::METHOD_CHECK_EXPIRES },
        { "mime-version"sv, header::MIME_VERSION },
        { "negotiate"sv, header::NEGOTIATE },
        { "non-compliance"sv, header::NON_COMPLIANCE },
        { "opt"sv, header::OPT },
        { "optional"sv, header::OPTIONAL_ },
        { "optional-www-authenticate"sv, header::OPTIONAL_WWW_AUTHENTICATE },
        { "ordering-type"sv, header::ORDERING_TYPE },
        { "origin"sv, header::ORIGIN },
        { "oscore"sv, header::OSCORE },
        { "overwrite"sv, header::OVERWRITE },
        { "p3p"sv, header::P3P },
        { "pep"sv, header::PEP },
        { "pep-info"sv, header::PEP_INFO },
        { "pics-label"sv, header::PICS_LABEL },
        { "position"sv, header::POSITION },
        { "pragma"sv, header::PRAGMA },
        { "prefer"sv, header::PREFER },
        { "preference-applied"sv, header::PREFERENCE_APPLIED },
        { "profileobject"sv, header::PROFILEOBJECT },
        { "protocol"sv, header::PROTOCOL },
        { "protocol-info"sv, header::PROTOCOL_INFO },
        { "protocol-query"sv, header::PROTOCOL_QUERY },
        { "protocol-request"sv, header::PROTOCOL_REQUEST },
        { "proxy-authenticate"sv, header::PROXY_AUTHENTICATE },
        { "proxy-authentication-info"sv, header::PROXY_AUTHENTICATION_INFO },
        { "proxy-authorization"sv, header::PROXY_AUTHORIZATION },
        { "proxy-features"sv, header::PROXY_FEATURES },
        { "proxy-instruction"sv, header::PROXY_INSTRUCTION },
        { "public"sv, header::PUBLIC },
        { "public-key-pins"sv, header::PUBLIC_KEY_PINS },
        { "public-key-pins-report-only"sv, header::PUBLIC_KEY_PINS_REPORT_ONLY },
        { "range"sv, header::RANGE },
        { "redirect-ref"sv, header::REDIRECT_REF },
        { "referer"sv, header::REFERER },
        { "referer-root"sv, header::REFERER_ROOT },
        { "replay-nonce"sv, header::REPLAY_NONCE },
        { "repr-digest"sv, header::REPR_DIGEST },
        { "resolution-hint"sv, header::RESOLUTION_HINT },
        { "resolver-location"sv, header::RESOLVER_LOCATION },
        { "retry-after"sv, header::RETRY_AFTER },
        { "safe"sv, header::SAFE },
        { "schedule-reply"sv, header::SCHEDULE_REPLY },
        { "schedule-tag"sv, header::SCHEDULE_TAG },
        { "sec-ch-ua"sv, header::SEC_CH_UA },
        { "sec-ch-ua-mobile"sv, header::SEC_CH_UA_MOBILE },
        { "sec-ch-ua-platform"sv, header::SEC_CH_UA_PLATFORM },
        { "sec-fetch-dest"sv, header::SEC_FETCH_DEST },
        { "sec-fetch-mode"sv, header::SEC_FETCH_MODE },
        { "sec-fetch-site"sv, header::SEC_FETCH_SITE },
        { "sec-fetch-user"sv, header::SEC_FETCH_USER },
        { "sec-token-binding"sv, header::SEC_TOKEN_BINDING },
        { "sec-websocket-accept"sv, header::SEC_WEBSOCKET_ACCEPT },
        { "sec-websocket-extensions"sv, header::SEC_WEBSOCKET_EXTENSIONS },
        { "sec-websocket-key"sv, header::SEC_WEBSOCKET_KEY },
        { "sec-websocket-protocol"sv, header::SEC_WEBSOCKET_PROTOCOL },
        { "sec-websocket-version"sv, header::SEC_WEBSOCKET_VERSION },
        { "security-scheme"sv, header::SECURITY_SCHEME },
        { "server"sv, header::SERVER },
        { "set-cookie"sv, header::SET_COOKIE },
        { "set-cookie2"sv, header::SET_COOKIE2 },
        { "setprofile"sv, header::SETPROFILE },
        { "signature"sv, header::SIGNATURE },
        { "signature-input"sv, header::SIGNATURE_INPUT },
        { "slug"sv, header::SLUG },
        { "soapaction"sv, header::SOAPACTION },
        { "status-uri"sv, header::STATUS_URI },
        { "strict-transport-security"sv, header::STRICT_TRANSPORT_SECURITY },
        { "subok"sv, header::SUBOK },
        { "subst"sv, header::SUBST },
        { "sunset"sv, header::SUNSET },
        { "surrogate-capability"sv, header::SURROGATE_CAPABILITY },
        { "surrogate-control"sv, header::SURROGATE_CONTROL },
        { "tcn"sv, header::TCN },
        { "te"sv, header::TE },
        { "timeout"sv, header::TIMEOUT },
        { "timing-allow-origin"sv, header::TIMING_ALLOW_ORIGIN },
        { "title"sv, header::TITLE },
        { "topic"sv, header::TOPIC },
        { "traceparent"sv, header::TRACEPARENT },
        { "tracestate"sv, header::TRACESTATE },
        { "trailer"sv, header::TRAILER },
        { "transfer-encoding"sv, header::TRANSFER_ENCODING },
        { "ttl"sv, header::TTL },
        { "ua-color"sv, header::UA_COLOR },
        { "ua-media"sv, header::UA_MEDIA },
        { "ua-pixels"sv, header::UA_PIXELS },
        { "ua-resolution"sv, header::UA_RESOLUTION },
        { "ua-windowpixels"sv, header::UA_WINDOWPIXELS },
        { "upgrade"sv, header::UPGRADE },
        { "upgrade-insecure-requests"sv, header::UPGRADE_INSECURE_REQUESTS },
        { "urgency"sv, header::URGENCY },
        { "uri"sv, header::URI },
        { "user-agent"sv, header::USER_AGENT },
        { "variant-vary"sv, header::VARIANT_VARY },
        { "vary"sv, header::VARY },
        { "version"sv, header::VERSION },
        { "via"sv, header::VIA },
        { "want-digest"sv, header::WANT_DIGEST },
        { "warning"sv, header::WARNING },
        { "www-authenticate"sv, header::WWW_AUTHENTICATE },
        { "x-cache"sv, header::X_CACHE },
        { "x-content-type-options"sv, header::X_CONTENT_TYPE_OPTIONS },
        { "x-device-accept"sv, header::X_DEVICE_ACCEPT },
        { "x-device-accept-charset"sv, header::X_DEVICE_ACCEPT_CHARSET },
        { "x-device-accept-encoding"sv, header::X_DEVICE_ACCEPT_ENCODING },
        { "x-device-accept-language"sv, header::X_DEVICE_ACCEPT_LANGUAGE },
        { "x-device-user-agent"sv, header::X_DEVICE_USER_AGENT },
        { "x-frame-options"sv, header::X_FRAME_OPTIONS },
        { "x-http-method-override"sv, header::X_HTTP_METHOD_OVERRIDE },
        { "x-powered-by"sv, header::X_POWERED_BY },
        { "x-requested-with"sv, header::X_REQUESTED_WITH },
        { "x-ua-compatible"sv, header::X_UA_COMPATIBLE },
        { "x-uidh"sv, header::X_UIDH },
        { "x-wap-profile"sv, header::X_WAP_PROFILE },
        { "x-webkit-csp"sv, header::X_WEBKIT_CSP },
        { "x-xss-protection"sv, header::X_XSS_PROTECTION },
    };
    auto it = std::lower_bound(std::begin(table), std::end(table), key,
        [](const auto& p, std::string_view s) { return p.first < s; });
    if (it != std::end(table) && it->first == key)
        return it->second;
    throw std::invalid_argument("Unknown header value: " + std::string(str));
}

}
