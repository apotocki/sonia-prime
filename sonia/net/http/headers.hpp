//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include "sonia/string.hpp"

namespace sonia::http {

// windows defines OPTIONAL
#define OPTIONAL_FIX OPTIONAL_

#define SONIA_HTTP_HEADERS_SEQ                                                  \
    ((A_IM, "A-IM"sv))                                                            \
    ((ALPN, "ALPN"sv))                                                            \
    ((AMP_CACHE_TRANSFORM, "AMP-Cache-Transform"sv))                              \
    ((ACCEPT, "Accept"sv))                                                        \
    ((ACCEPT_ADDITIONS, "Accept-Additions"sv))                                    \
    ((ACCEPT_CHARSET, "Accept-Charset"sv))                                        \
    ((ACCEPT_DATETIME, "Accept-Datetime"sv))                                      \
    ((ACCEPT_ENCODING, "Accept-Encoding"sv))                                      \
    ((ACCEPT_FEATURES, "Accept-Features"sv))                                      \
    ((ACCEPT_LANGUAGE, "Accept-Language"sv))                                      \
    ((ACCEPT_PATCH, "Accept-Patch"sv))                                            \
    ((ACCEPT_POST, "Accept-Post"sv))                                              \
    ((ACCEPT_RANGES, "Accept-Ranges"sv))                                          \
    ((ACCESS_CONTROL, "Access-Control"sv))                                        \
    ((ACCESS_CONTROL_ALLOW_CREDENTIALS, "Access-Control-Allow-Credentials"sv))    \
    ((ACCESS_CONTROL_ALLOW_HEADERS, "Access-Control-Allow-Headers"sv))            \
    ((ACCESS_CONTROL_ALLOW_METHODS, "Access-Control-Allow-Methods"sv))            \
    ((ACCESS_CONTROL_ALLOW_ORIGIN, "Access-Control-Allow-Origin"sv))              \
    ((ACCESS_CONTROL_MAX_AGE, "Access-Control-Max-Age"sv))                        \
    ((ACCESS_CONTROL_REQUEST_HEADERS, "Access-Control-Request-Headers"sv))        \
    ((ACCESS_CONTROL_REQUEST_METHOD, "Access-Control-Request-Method"sv))          \
    ((AGE, "Age"sv))                                                              \
    ((ALLOW, "Allow"sv))                                                          \
    ((ALT_SVC, "Alt-Svc"sv))                                                      \
    ((ALT_USED, "Alt-Used"sv))                                                    \
    ((ALTERNATES, "Alternates"sv))                                                \
    ((APPLY_TO_REDIRECT_REF, "Apply-To-Redirect-Ref"sv))                          \
    ((AUTHENTICATION_CONTROL, "Authentication-Control"sv))                        \
    ((AUTHENTICATION_INFO, "Authentication-Info"sv))                              \
    ((AUTHORIZATION, "Authorization"sv))                                          \
    ((C_EXT, "C-Ext"sv))                                                          \
    ((C_MAN, "C-Man"sv))                                                          \
    ((C_OPT, "C-Opt"sv))                                                          \
    ((C_PEP, "C-PEP"sv))                                                          \
    ((C_PEP_INFO, "C-PEP-Info"sv))                                                \
    ((CDN_LOOP, "CDN-Loop"sv))                                                    \
    ((CACHE_CONTROL, "Cache-Control"sv))                                          \
    ((CAL_MANAGED_ID, "Cal-Managed-ID"sv))                                        \
    ((CALDAV_TIMEZONES, "CalDAV-Timezones"sv))                                    \
    ((CLOSE, "Close"sv))                                                          \
    ((COMPLIANCE, "Compliance"sv))                                                \
    ((CONNECTION, "Connection"sv))                                                \
    ((CONTENT_BASE, "Content-Base"sv))                                            \
    ((CONTENT_DISPOSITION, "Content-Disposition"sv))                              \
    ((CONTENT_ENCODING, "Content-Encoding"sv))                                    \
    ((CONTENT_ID, "Content-ID"sv))                                                \
    ((CONTENT_LANGUAGE, "Content-Language"sv))                                    \
    ((CONTENT_LENGTH, "Content-Length"sv))                                        \
    ((CONTENT_LOCATION, "Content-Location"sv))                                    \
    ((CONTENT_MD5, "Content-MD5"sv))                                              \
    ((CONTENT_RANGE, "Content-Range"sv))                                          \
    ((CONTENT_SCRIPT_TYPE, "Content-Script-Type"sv))                              \
    ((CONTENT_STYLE_TYPE, "Content-Style-Type"sv))                                \
    ((CONTENT_TRANSFER_ENCODING, "Content-Transfer-Encoding"sv))                  \
    ((CONTENT_TYPE, "Content-Type"sv))                                            \
    ((CONTENT_VERSION, "Content-Version"sv))                                      \
    ((COOKIE, "Cookie"sv))                                                        \
    ((COOKIE2, "Cookie2"sv))                                                      \
    ((COST, "Cost"sv))                                                            \
    ((DASL, "DASL"sv))                                                            \
    ((DAV, "DAV"sv))                                                              \
    ((DATE, "Date"sv))                                                            \
    ((DEFAULT_STYLE, "Default-Style"sv))                                          \
    ((DELTA_BASE, "Delta-Base"sv))                                                \
    ((DEPTH, "Depth"sv))                                                          \
    ((DERIVED_FROM, "Derived-From"sv))                                            \
    ((DESTINATION, "Destination"sv))                                              \
    ((DIFFERENTIAL_ID, "Differential-ID"sv))                                      \
    ((DIGEST, "Digest"sv))                                                        \
    ((EDIINT_FEATURES, "EDIINT-Features"sv))                                      \
    ((ETAG, "ETag"sv))                                                            \
    ((EARLY_DATA, "Early-Data"sv))                                                \
    ((EXPECT, "Expect"sv))                                                        \
    ((EXPECT_CT, "Expect-CT"sv))                                                  \
    ((EXPIRES, "Expires"sv))                                                      \
    ((EXT, "Ext"sv))                                                              \
    ((FORWARDED, "Forwarded"sv))                                                  \
    ((FROM, "From"sv))                                                            \
    ((GETPROFILE, "GetProfile"sv))                                                \
    ((HTTP2_SETTINGS, "HTTP2-Settings"sv))                                        \
    ((HOBAREG, "Hobareg"sv))                                                      \
    ((HOST, "Host"sv))                                                            \
    ((IM, "IM"sv))                                                                \
    ((IF, "If"sv))                                                                \
    ((IF_MATCH, "If-Match"sv))                                                    \
    ((IF_MODIFIED_SINCE, "If-Modified-Since"sv))                                  \
    ((IF_NONE_MATCH, "If-None-Match"sv))                                          \
    ((IF_RANGE, "If-Range"sv))                                                    \
    ((IF_SCHEDULE_TAG_MATCH, "If-Schedule-Tag-Match"sv))                          \
    ((IF_UNMODIFIED_SINCE, "If-Unmodified-Since"sv))                              \
    ((INCLUDE_REFERRED_TOKEN_BINDING_ID, "Include-Referred-Token-Binding-ID"sv))  \
    ((KEEP_ALIVE, "Keep-Alive"sv))                                                \
    ((LABEL, "Label"sv))                                                          \
    ((LAST_MODIFIED, "Last-Modified"sv))                                          \
    ((LINK, "Link"sv))                                                            \
    ((LOCATION, "Location"sv))                                                    \
    ((LOCK_TOKEN, "Lock-Token"sv))                                                \
    ((MIME_VERSION, "MIME-Version"sv))                                            \
    ((MAN, "Man"sv))                                                              \
    ((MAX_FORWARDS, "Max-Forwards"sv))                                            \
    ((MEMENTO_DATETIME, "Memento-Datetime"sv))                                    \
    ((MESSAGE_ID, "Message-ID"sv))                                                \
    ((METER, "Meter"sv))                                                          \
    ((METHOD_CHECK, "Method-Check"sv))                                            \
    ((METHOD_CHECK_EXPIRES, "Method-Check-Expires"sv))                            \
    ((NEGOTIATE, "Negotiate"sv))                                                  \
    ((NON_COMPLIANCE, "Non-Compliance"sv))                                        \
    ((OSCORE, "OSCORE"sv))                                                        \
    ((OPT, "Opt"sv))                                                              \
    ((OPTIONAL_FIX, "Optional"sv))                                                \
    ((OPTIONAL_WWW_AUTHENTICATE, "Optional-WWW-Authenticate"sv))                  \
    ((ORDERING_TYPE, "Ordering-Type"sv))                                          \
    ((ORIGIN, "Origin"sv))                                                        \
    ((OVERWRITE, "Overwrite"sv))                                                  \
    ((P3P, "P3P"sv))                                                              \
    ((PEP, "PEP"sv))                                                              \
    ((PICS_LABEL, "PICS-Label"sv))                                                \
    ((PEP_INFO, "Pep-Info"sv))                                                    \
    ((POSITION, "Position"sv))                                                    \
    ((PRAGMA, "Pragma"sv))                                                        \
    ((PREFER, "Prefer"sv))                                                        \
    ((PREFERENCE_APPLIED, "Preference-Applied"sv))                                \
    ((PROFILEOBJECT, "ProfileObject"sv))                                          \
    ((PROTOCOL, "Protocol"sv))                                                    \
    ((PROTOCOL_INFO, "Protocol-Info"sv))                                          \
    ((PROTOCOL_QUERY, "Protocol-Query"sv))                                        \
    ((PROTOCOL_REQUEST, "Protocol-Request"sv))                                    \
    ((PROXY_AUTHENTICATE, "Proxy-Authenticate"sv))                                \
    ((PROXY_AUTHENTICATION_INFO, "Proxy-Authentication-Info"sv))                  \
    ((PROXY_AUTHORIZATION, "Proxy-Authorization"sv))                              \
    ((PROXY_FEATURES, "Proxy-Features"sv))                                        \
    ((PROXY_INSTRUCTION, "Proxy-Instruction"sv))                                  \
    ((PUBLIC, "Public"sv))                                                        \
    ((PUBLIC_KEY_PINS, "Public-Key-Pins"sv))                                      \
    ((PUBLIC_KEY_PINS_REPORT_ONLY, "Public-Key-Pins-Report-Only"sv))              \
    ((RANGE, "Range"sv))                                                          \
    ((REDIRECT_REF, "Redirect-Ref"sv))                                            \
    ((REFERER, "Referer"sv))                                                      \
    ((REFERER_ROOT, "Referer-Root"sv))                                            \
    ((REPLAY_NONCE, "Replay-Nonce"sv))                                            \
    ((RESOLUTION_HINT, "Resolution-Hint"sv))                                      \
    ((RESOLVER_LOCATION, "Resolver-Location"sv))                                  \
    ((RETRY_AFTER, "Retry-After"sv))                                              \
    ((SLUG, "SLUG"sv))                                                            \
    ((SAFE, "Safe"sv))                                                            \
    ((SCHEDULE_REPLY, "Schedule-Reply"sv))                                        \
    ((SCHEDULE_TAG, "Schedule-Tag"sv))                                            \
    ((SEC_TOKEN_BINDING, "Sec-Token-Binding"sv))                                  \
    ((SEC_WEBSOCKET_ACCEPT, "Sec-WebSocket-Accept"sv))                            \
    ((SEC_WEBSOCKET_EXTENSIONS, "Sec-WebSocket-Extensions"sv))                    \
    ((SEC_WEBSOCKET_KEY, "Sec-WebSocket-Key"sv))                                  \
    ((SEC_WEBSOCKET_PROTOCOL, "Sec-WebSocket-Protocol"sv))                        \
    ((SEC_WEBSOCKET_VERSION, "Sec-WebSocket-Version"sv))                          \
    ((SECURITY_SCHEME, "Security-Scheme"sv))                                      \
    ((SERVER, "Server"sv))                                                        \
    ((SET_COOKIE, "Set-Cookie"sv))                                                \
    ((SET_COOKIE2, "Set-Cookie2"sv))                                              \
    ((SETPROFILE, "SetProfile"sv))                                                \
    ((SOAPACTION, "SoapAction"sv))                                                \
    ((STATUS_URI, "Status-URI"sv))                                                \
    ((STRICT_TRANSPORT_SECURITY, "Strict-Transport-Security"sv))                  \
    ((SUBOK, "SubOK"sv))                                                          \
    ((SUBST, "Subst"sv))                                                          \
    ((SUNSET, "Sunset"sv))                                                        \
    ((SURROGATE_CAPABILITY, "Surrogate-Capability"sv))                            \
    ((SURROGATE_CONTROL, "Surrogate-Control"sv))                                  \
    ((TCN, "TCN"sv))                                                              \
    ((TE, "TE"sv))                                                                \
    ((TTL, "TTL"sv))                                                              \
    ((TIMEOUT, "Timeout"sv))                                                      \
    ((TIMING_ALLOW_ORIGIN, "Timing-Allow-Origin"sv))                              \
    ((TITLE, "Title"sv))                                                          \
    ((TOPIC, "Topic"sv))                                                          \
    ((TRACEPARENT, "Traceparent"sv))                                              \
    ((TRACESTATE, "Tracestate"sv))                                                \
    ((TRAILER, "Trailer"sv))                                                      \
    ((TRANSFER_ENCODING, "Transfer-Encoding"sv))                                  \
    ((UA_COLOR, "UA-Color"sv))                                                    \
    ((UA_MEDIA, "UA-Media"sv))                                                    \
    ((UA_PIXELS, "UA-Pixels"sv))                                                  \
    ((UA_RESOLUTION, "UA-Resolution"sv))                                          \
    ((UA_WINDOWPIXELS, "UA-Windowpixels"sv))                                      \
    ((URI, "URI"sv))                                                              \
    ((UPGRADE, "Upgrade"sv))                                                      \
    ((URGENCY, "Urgency"sv))                                                      \
    ((USER_AGENT, "User-Agent"sv))                                                \
    ((VARIANT_VARY, "Variant-Vary"sv))                                            \
    ((VARY, "Vary"sv))                                                            \
    ((VERSION, "Version"sv))                                                      \
    ((VIA, "Via"sv))                                                              \
    ((WWW_AUTHENTICATE, "WWW-Authenticate"sv))                                    \
    ((WANT_DIGEST, "Want-Digest"sv))                                              \
    ((WARNING, "Warning"sv))                                                      \
    ((X_CACHE, "X-Cache"sv))                                                      \
    ((X_CONTENT_TYPE_OPTIONS, "X-Content-Type-Options"sv))                        \
    ((X_DEVICE_ACCEPT, "X-Device-Accept"sv))                                      \
    ((X_DEVICE_ACCEPT_CHARSET, "X-Device-Accept-Charset"sv))                      \
    ((X_DEVICE_ACCEPT_ENCODING, "X-Device-Accept-Encoding"sv))                    \
    ((X_DEVICE_ACCEPT_LANGUAGE, "X-Device-Accept-Language"sv))                    \
    ((X_DEVICE_USER_AGENT, "X-Device-User-Agent"sv))                              \
    ((X_FRAME_OPTIONS, "X-Frame-Options"sv))                                      \
    ((X_HTTP_METHOD_OVERRIDE, "X-Http-Method-Override"sv))                        \
    ((X_POWERED_BY, "X-Powered-By"sv))                                            \
    ((X_REQUESTED_WITH, "X-Requested-With"sv))                                    \
    ((X_UA_COMPATIBLE, "X-UA-Compatible"sv))                                      \
    ((X_UIDH, "X-UIDH"sv))                                                        \
    ((X_WAP_PROFILE, "X-Wap-Profile"sv))                                          \
    ((X_WEBKIT_CSP, "X-WebKit-CSP"sv))                                            \
    ((X_XSS_PROTECTION, "X-XSS-Protection"sv))

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem),

enum class header
{
    UNKNOWN = 0,
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_HTTP_HEADERS_SEQ)
    UPPER_BOUND_VALUE
};

std::string_view to_string_view(header);
header to_header(std::string_view);

inline size_t hash_value(header h) { return (int)h; }

#undef SONIA_PRINT_TYPE_ENUM

}
