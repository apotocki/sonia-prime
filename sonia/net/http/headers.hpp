//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HTTP_HEADERS_HPP
#define SONIA_HTTP_HEADERS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include "sonia/string.hpp"

namespace sonia::http {

// windows defines OPTIONAL
#define OPTIONAL_FIX OPTIONAL_

#define SONIA_HTTP_HEADERS_SEQ                                                  \
    ((A_IM, "A-IM"))                                                            \
    ((ALPN, "ALPN"))                                                            \
    ((AMP_CACHE_TRANSFORM, "AMP-Cache-Transform"))                              \
    ((ACCEPT, "Accept"))                                                        \
    ((ACCEPT_ADDITIONS, "Accept-Additions"))                                    \
    ((ACCEPT_CHARSET, "Accept-Charset"))                                        \
    ((ACCEPT_DATETIME, "Accept-Datetime"))                                      \
    ((ACCEPT_ENCODING, "Accept-Encoding"))                                      \
    ((ACCEPT_FEATURES, "Accept-Features"))                                      \
    ((ACCEPT_LANGUAGE, "Accept-Language"))                                      \
    ((ACCEPT_PATCH, "Accept-Patch"))                                            \
    ((ACCEPT_POST, "Accept-Post"))                                              \
    ((ACCEPT_RANGES, "Accept-Ranges"))                                          \
    ((ACCESS_CONTROL, "Access-Control"))                                        \
    ((ACCESS_CONTROL_ALLOW_CREDENTIALS, "Access-Control-Allow-Credentials"))    \
    ((ACCESS_CONTROL_ALLOW_HEADERS, "Access-Control-Allow-Headers"))            \
    ((ACCESS_CONTROL_ALLOW_METHODS, "Access-Control-Allow-Methods"))            \
    ((ACCESS_CONTROL_ALLOW_ORIGIN, "Access-Control-Allow-Origin"))              \
    ((ACCESS_CONTROL_MAX_AGE, "Access-Control-Max-Age"))                        \
    ((ACCESS_CONTROL_REQUEST_HEADERS, "Access-Control-Request-Headers"))        \
    ((ACCESS_CONTROL_REQUEST_METHOD, "Access-Control-Request-Method"))          \
    ((AGE, "Age"))                                                              \
    ((ALLOW, "Allow"))                                                          \
    ((ALT_SVC, "Alt-Svc"))                                                      \
    ((ALT_USED, "Alt-Used"))                                                    \
    ((ALTERNATES, "Alternates"))                                                \
    ((APPLY_TO_REDIRECT_REF, "Apply-To-Redirect-Ref"))                          \
    ((AUTHENTICATION_CONTROL, "Authentication-Control"))                        \
    ((AUTHENTICATION_INFO, "Authentication-Info"))                              \
    ((AUTHORIZATION, "Authorization"))                                          \
    ((C_EXT, "C-Ext"))                                                          \
    ((C_MAN, "C-Man"))                                                          \
    ((C_OPT, "C-Opt"))                                                          \
    ((C_PEP, "C-PEP"))                                                          \
    ((C_PEP_INFO, "C-PEP-Info"))                                                \
    ((CDN_LOOP, "CDN-Loop"))                                                    \
    ((CACHE_CONTROL, "Cache-Control"))                                          \
    ((CAL_MANAGED_ID, "Cal-Managed-ID"))                                        \
    ((CALDAV_TIMEZONES, "CalDAV-Timezones"))                                    \
    ((CLOSE, "Close"))                                                          \
    ((COMPLIANCE, "Compliance"))                                                \
    ((CONNECTION, "Connection"))                                                \
    ((CONTENT_BASE, "Content-Base"))                                            \
    ((CONTENT_DISPOSITION, "Content-Disposition"))                              \
    ((CONTENT_ENCODING, "Content-Encoding"))                                    \
    ((CONTENT_ID, "Content-ID"))                                                \
    ((CONTENT_LANGUAGE, "Content-Language"))                                    \
    ((CONTENT_LENGTH, "Content-Length"))                                        \
    ((CONTENT_LOCATION, "Content-Location"))                                    \
    ((CONTENT_MD5, "Content-MD5"))                                              \
    ((CONTENT_RANGE, "Content-Range"))                                          \
    ((CONTENT_SCRIPT_TYPE, "Content-Script-Type"))                              \
    ((CONTENT_STYLE_TYPE, "Content-Style-Type"))                                \
    ((CONTENT_TRANSFER_ENCODING, "Content-Transfer-Encoding"))                  \
    ((CONTENT_TYPE, "Content-Type"))                                            \
    ((CONTENT_VERSION, "Content-Version"))                                      \
    ((COOKIE, "Cookie"))                                                        \
    ((COOKIE2, "Cookie2"))                                                      \
    ((COST, "Cost"))                                                            \
    ((DASL, "DASL"))                                                            \
    ((DAV, "DAV"))                                                              \
    ((DATE, "Date"))                                                            \
    ((DEFAULT_STYLE, "Default-Style"))                                          \
    ((DELTA_BASE, "Delta-Base"))                                                \
    ((DEPTH, "Depth"))                                                          \
    ((DERIVED_FROM, "Derived-From"))                                            \
    ((DESTINATION, "Destination"))                                              \
    ((DIFFERENTIAL_ID, "Differential-ID"))                                      \
    ((DIGEST, "Digest"))                                                        \
    ((EDIINT_FEATURES, "EDIINT-Features"))                                      \
    ((ETAG, "ETag"))                                                            \
    ((EARLY_DATA, "Early-Data"))                                                \
    ((EXPECT, "Expect"))                                                        \
    ((EXPECT_CT, "Expect-CT"))                                                  \
    ((EXPIRES, "Expires"))                                                      \
    ((EXT, "Ext"))                                                              \
    ((FORWARDED, "Forwarded"))                                                  \
    ((FROM, "From"))                                                            \
    ((GETPROFILE, "GetProfile"))                                                \
    ((HTTP2_SETTINGS, "HTTP2-Settings"))                                        \
    ((HOBAREG, "Hobareg"))                                                      \
    ((HOST, "Host"))                                                            \
    ((IM, "IM"))                                                                \
    ((IF, "If"))                                                                \
    ((IF_MATCH, "If-Match"))                                                    \
    ((IF_MODIFIED_SINCE, "If-Modified-Since"))                                  \
    ((IF_NONE_MATCH, "If-None-Match"))                                          \
    ((IF_RANGE, "If-Range"))                                                    \
    ((IF_SCHEDULE_TAG_MATCH, "If-Schedule-Tag-Match"))                          \
    ((IF_UNMODIFIED_SINCE, "If-Unmodified-Since"))                              \
    ((INCLUDE_REFERRED_TOKEN_BINDING_ID, "Include-Referred-Token-Binding-ID"))  \
    ((KEEP_ALIVE, "Keep-Alive"))                                                \
    ((LABEL, "Label"))                                                          \
    ((LAST_MODIFIED, "Last-Modified"))                                          \
    ((LINK, "Link"))                                                            \
    ((LOCATION, "Location"))                                                    \
    ((LOCK_TOKEN, "Lock-Token"))                                                \
    ((MIME_VERSION, "MIME-Version"))                                            \
    ((MAN, "Man"))                                                              \
    ((MAX_FORWARDS, "Max-Forwards"))                                            \
    ((MEMENTO_DATETIME, "Memento-Datetime"))                                    \
    ((MESSAGE_ID, "Message-ID"))                                                \
    ((METER, "Meter"))                                                          \
    ((METHOD_CHECK, "Method-Check"))                                            \
    ((METHOD_CHECK_EXPIRES, "Method-Check-Expires"))                            \
    ((NEGOTIATE, "Negotiate"))                                                  \
    ((NON_COMPLIANCE, "Non-Compliance"))                                        \
    ((OSCORE, "OSCORE"))                                                        \
    ((OPT, "Opt"))                                                              \
    ((OPTIONAL_FIX, "Optional"))                                                \
    ((OPTIONAL_WWW_AUTHENTICATE, "Optional-WWW-Authenticate"))                  \
    ((ORDERING_TYPE, "Ordering-Type"))                                          \
    ((ORIGIN, "Origin"))                                                        \
    ((OVERWRITE, "Overwrite"))                                                  \
    ((P3P, "P3P"))                                                              \
    ((PEP, "PEP"))                                                              \
    ((PICS_LABEL, "PICS-Label"))                                                \
    ((PEP_INFO, "Pep-Info"))                                                    \
    ((POSITION, "Position"))                                                    \
    ((PRAGMA, "Pragma"))                                                        \
    ((PREFER, "Prefer"))                                                        \
    ((PREFERENCE_APPLIED, "Preference-Applied"))                                \
    ((PROFILEOBJECT, "ProfileObject"))                                          \
    ((PROTOCOL, "Protocol"))                                                    \
    ((PROTOCOL_INFO, "Protocol-Info"))                                          \
    ((PROTOCOL_QUERY, "Protocol-Query"))                                        \
    ((PROTOCOL_REQUEST, "Protocol-Request"))                                    \
    ((PROXY_AUTHENTICATE, "Proxy-Authenticate"))                                \
    ((PROXY_AUTHENTICATION_INFO, "Proxy-Authentication-Info"))                  \
    ((PROXY_AUTHORIZATION, "Proxy-Authorization"))                              \
    ((PROXY_FEATURES, "Proxy-Features"))                                        \
    ((PROXY_INSTRUCTION, "Proxy-Instruction"))                                  \
    ((PUBLIC, "Public"))                                                        \
    ((PUBLIC_KEY_PINS, "Public-Key-Pins"))                                      \
    ((PUBLIC_KEY_PINS_REPORT_ONLY, "Public-Key-Pins-Report-Only"))              \
    ((RANGE, "Range"))                                                          \
    ((REDIRECT_REF, "Redirect-Ref"))                                            \
    ((REFERER, "Referer"))                                                      \
    ((REFERER_ROOT, "Referer-Root"))                                            \
    ((REPLAY_NONCE, "Replay-Nonce"))                                            \
    ((RESOLUTION_HINT, "Resolution-Hint"))                                      \
    ((RESOLVER_LOCATION, "Resolver-Location"))                                  \
    ((RETRY_AFTER, "Retry-After"))                                              \
    ((SLUG, "SLUG"))                                                            \
    ((SAFE, "Safe"))                                                            \
    ((SCHEDULE_REPLY, "Schedule-Reply"))                                        \
    ((SCHEDULE_TAG, "Schedule-Tag"))                                            \
    ((SEC_TOKEN_BINDING, "Sec-Token-Binding"))                                  \
    ((SEC_WEBSOCKET_ACCEPT, "Sec-WebSocket-Accept"))                            \
    ((SEC_WEBSOCKET_EXTENSIONS, "Sec-WebSocket-Extensions"))                    \
    ((SEC_WEBSOCKET_KEY, "Sec-WebSocket-Key"))                                  \
    ((SEC_WEBSOCKET_PROTOCOL, "Sec-WebSocket-Protocol"))                        \
    ((SEC_WEBSOCKET_VERSION, "Sec-WebSocket-Version"))                          \
    ((SECURITY_SCHEME, "Security-Scheme"))                                      \
    ((SERVER, "Server"))                                                        \
    ((SET_COOKIE, "Set-Cookie"))                                                \
    ((SET_COOKIE2, "Set-Cookie2"))                                              \
    ((SETPROFILE, "SetProfile"))                                                \
    ((SOAPACTION, "SoapAction"))                                                \
    ((STATUS_URI, "Status-URI"))                                                \
    ((STRICT_TRANSPORT_SECURITY, "Strict-Transport-Security"))                  \
    ((SUBOK, "SubOK"))                                                          \
    ((SUBST, "Subst"))                                                          \
    ((SUNSET, "Sunset"))                                                        \
    ((SURROGATE_CAPABILITY, "Surrogate-Capability"))                            \
    ((SURROGATE_CONTROL, "Surrogate-Control"))                                  \
    ((TCN, "TCN"))                                                              \
    ((TE, "TE"))                                                                \
    ((TTL, "TTL"))                                                              \
    ((TIMEOUT, "Timeout"))                                                      \
    ((TIMING_ALLOW_ORIGIN, "Timing-Allow-Origin"))                              \
    ((TITLE, "Title"))                                                          \
    ((TOPIC, "Topic"))                                                          \
    ((TRACEPARENT, "Traceparent"))                                              \
    ((TRACESTATE, "Tracestate"))                                                \
    ((TRAILER, "Trailer"))                                                      \
    ((TRANSFER_ENCODING, "Transfer-Encoding"))                                  \
    ((UA_COLOR, "UA-Color"))                                                    \
    ((UA_MEDIA, "UA-Media"))                                                    \
    ((UA_PIXELS, "UA-Pixels"))                                                  \
    ((UA_RESOLUTION, "UA-Resolution"))                                          \
    ((UA_WINDOWPIXELS, "UA-Windowpixels"))                                      \
    ((URI, "URI"))                                                              \
    ((UPGRADE, "Upgrade"))                                                      \
    ((URGENCY, "Urgency"))                                                      \
    ((USER_AGENT, "User-Agent"))                                                \
    ((VARIANT_VARY, "Variant-Vary"))                                            \
    ((VARY, "Vary"))                                                            \
    ((VERSION, "Version"))                                                      \
    ((VIA, "Via"))                                                              \
    ((WWW_AUTHENTICATE, "WWW-Authenticate"))                                    \
    ((WANT_DIGEST, "Want-Digest"))                                              \
    ((WARNING, "Warning"))                                                      \
    ((X_CACHE, "X-Cache"))                                                      \
    ((X_CONTENT_TYPE_OPTIONS, "X-Content-Type-Options"))                        \
    ((X_DEVICE_ACCEPT, "X-Device-Accept"))                                      \
    ((X_DEVICE_ACCEPT_CHARSET, "X-Device-Accept-Charset"))                      \
    ((X_DEVICE_ACCEPT_ENCODING, "X-Device-Accept-Encoding"))                    \
    ((X_DEVICE_ACCEPT_LANGUAGE, "X-Device-Accept-Language"))                    \
    ((X_DEVICE_USER_AGENT, "X-Device-User-Agent"))                              \
    ((X_FRAME_OPTIONS, "X-Frame-Options"))                                      \
    ((X_HTTP_METHOD_OVERRIDE, "X-Http-Method-Override"))                        \
    ((X_POWERED_BY, "X-Powered-By"))                                            \
    ((X_REQUESTED_WITH, "X-Requested-With"))                                    \
    ((X_UA_COMPATIBLE, "X-UA-Compatible"))                                      \
    ((X_UIDH, "X-UIDH"))                                                        \
    ((X_WAP_PROFILE, "X-Wap-Profile"))                                          \
    ((X_WEBKIT_CSP, "X-WebKit-CSP"))                                            \
    ((X_XSS_PROTECTION, "X-XSS-Protection"))

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem),

enum class header
{
    UNKNOWN = 0,
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_HTTP_HEADERS_SEQ)
    UPPER_BOUND_VALUE
};

std::string const& to_string(header);
header to_header(string_view);

inline size_t hash_value(header h) { return (int)h; }

#undef SONIA_PRINT_TYPE_ENUM

}

#endif // SONIA_HTTP_HEADERS_HPP
