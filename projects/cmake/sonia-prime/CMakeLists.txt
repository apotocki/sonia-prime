include (${COMMON_INCLUDE})

include_directories(${PROJECT_HOME})
include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)
include_directories(${TPLS}/lexertl14/include)

set (BUNDLE_NAME sonia-prime)
set (BUNDLE_HOME ${BUNDLES}/sonia-prime)
set (BUNDLE_SOURCES
    ${BUNDLE_HOME}/sonia/services/services.cpp
    ${BUNDLE_HOME}/sonia/services/environment.cpp
    ${BUNDLE_HOME}/sonia/services/services_host.cpp
    ${BUNDLE_HOME}/sonia/services/service_locator.cpp
    ${BUNDLE_HOME}/sonia/services/basic_service_factory.cpp
    ${BUNDLE_HOME}/sonia/services/local_service_registry.cpp
    ${BUNDLE_HOME}/sonia/services/registry/registry_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/registry/registry_service.cpp
    ${BUNDLE_HOME}/sonia/services/bookkeeper/bookkeeper_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/bookkeeper/bookkeeper_service.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/scheduler_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/scheduler_service.cpp
    ${BUNDLE_HOME}/sonia/services/auth/auth_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/auth/auth_service.cpp
    ${BUNDLE_HOME}/sonia/services/tenger/tenger_service.cpp
    ${BUNDLE_HOME}/sonia/services/tenger/tenger_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/prime_bundle.cpp
    ${BUNDLE_HOME}/sonia/utility/invokation/invokation.cpp
)

if (NOT DEFINED NO_NET)
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/services/io/io_service_builder.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_service.cpp



        ${BUNDLE_HOME}/sonia/services/net/net_service_builder.cpp
        ${BUNDLE_HOME}/sonia/services/net/net_service.cpp

        ${BUNDLE_HOME}/sonia/services/net/http/http_connector_builder.cpp
        ${BUNDLE_HOME}/sonia/services/net/http/http_connector.cpp

        ${BUNDLE_HOME}/sonia/services/net/http/http_static_application_builder.cpp
        ${BUNDLE_HOME}/sonia/services/net/http/http_static_application.cpp
        ${BUNDLE_HOME}/sonia/services/net/http/http_digest_authentication_application_builder.cpp
        ${BUNDLE_HOME}/sonia/services/net/http/http_digest_authentication_application.cpp
    )

    if (NOT DEFINED NO_IO_CACHE)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/services/io/io_cache_service.cpp
            ${BUNDLE_HOME}/sonia/services/io/io_cache_service_builder.cpp
        )
    else()
        add_definitions(-DSONIA_NO_IO_CACHE)
    endif()

    if (NOT DEFINED NO_ECHO)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/services/net/echo/echo_connector_builder.cpp
            ${BUNDLE_HOME}/sonia/services/net/echo/echo_connector.cpp
        )
    else()
        add_definitions(-DSONIA_NO_ECHO)
    endif()

    if (NOT DEFINED NO_HTTP_DEFAULT)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/services/net/http/http_default_application_builder.cpp
            ${BUNDLE_HOME}/sonia/services/net/http/http_default_application.cpp
        )
    else()
        add_definitions(-DSONIA_NO_HTTP_DEFAULT)
    endif()

    if (NOT DEFINED NO_TRANSCEIVER)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/services/transceiver/transceiver_builder.cpp
            ${BUNDLE_HOME}/sonia/services/transceiver/transceiver_service.cpp
        )
    else()
        add_definitions(-DSONIA_NO_TRANSCEIVER)
    endif()

    if (NOT DEFINED NO_SSL)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/services/io/io_ssl_service_builder.cpp
        )
    else()
        add_definitions(-DSONIA_NO_SSL)
    endif()
else()
    add_definitions(-DSONIA_NO_NET)
endif ()

add_definitions(-DSONIA_PRIME_EXPORTS)

if ("${LIBRARY_TYPE}" STREQUAL "STATIC")
	add_definitions(-DSONIA_PRIME_STATIC)
    set_property(GLOBAL PROPERTY ${BUNDLE_NAME}-import-flags -DSONIA_PRIME_STATIC)
endif ()

add_library(${BUNDLE_NAME} ${LIBRARY_TYPE} ${BUNDLE_SOURCES})
target_link_libraries(${BUNDLE_NAME} sonia-prime-lib ${ZLIB} ${BZLIB} ${LIBLZMA} ${BOOST_LOG_SETUP} ${BOOST_LOG} ${BOOST_SERIALIZATION} ${BOOST_FILESYSTEM} ${BOOST_THREAD} ${BOOST_PROGRAM_OPTIONS} ${BOOST_CONTEXT} ${BOOST_REGEX} ${BOOST_STACKTRACE} ${BOOST_RANDOM} ${PLATFORM_LIBS})
if (NOT DEFINED NO_SSL)
    target_link_libraries(${BUNDLE_NAME} ${OPENSSL})
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_link_libraries(${BUNDLE_NAME} log)
endif()
set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")