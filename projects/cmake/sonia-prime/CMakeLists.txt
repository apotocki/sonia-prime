include (${COMMON_INCLUDE})

include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)
include_directories(${TPLS}/lexertl14/include)

set (BUNDLE_NAME sonia-prime)
set (BUNDLE_HOME ${BUNDLES}/sonia-prime)
set (BUNDLE_SOURCES
    ${BUNDLE_HOME}/sonia/services/services.cpp
    ${BUNDLE_HOME}/sonia/services/environment.cpp
    ${BUNDLE_HOME}/sonia/services/services_host.cpp
    ${BUNDLE_HOME}/sonia/services/singleton_locator.cpp
    ${BUNDLE_HOME}/sonia/services/service_locator.cpp
    ${BUNDLE_HOME}/sonia/services/basic_service_factory.cpp
    ${BUNDLE_HOME}/sonia/services/local_service_registry.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/scheduler_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/scheduler_service.cpp
    ${BUNDLE_HOME}/sonia/services/io/io_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/io/io_service.cpp
    ${BUNDLE_HOME}/sonia/services/io/io_cache_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/io/io_cache_service.cpp
    ${BUNDLE_HOME}/sonia/services/transceiver/transceiver_builder.cpp
    ${BUNDLE_HOME}/sonia/services/transceiver/transceiver_service.cpp
    ${BUNDLE_HOME}/sonia/services/net/net_service_builder.cpp
    ${BUNDLE_HOME}/sonia/services/net/net_service.cpp
    ${BUNDLE_HOME}/sonia/services/net/echo/echo_connector_builder.cpp
    ${BUNDLE_HOME}/sonia/services/net/echo/echo_connector.cpp
    ${BUNDLE_HOME}/sonia/services/net/http/http_connector_builder.cpp
    ${BUNDLE_HOME}/sonia/services/net/http/http_connector.cpp
    ${BUNDLE_HOME}/sonia/services/net/http/http_default_application_builder.cpp
    ${BUNDLE_HOME}/sonia/services/net/http/http_default_application.cpp
    ${BUNDLE_HOME}/sonia/services/prime_bundle.cpp
)

if ( CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/utility/posix/signals.cpp
    )
endif ()

add_definitions(-DSONIA_PRIME_EXPORTS)

if ("${LIBRARY_TYPE}" STREQUAL "STATIC")
	add_definitions(-DSONIA_PRIME_STATIC)
    set_property(GLOBAL PROPERTY ${BUNDLE_NAME}-import-flags -DSONIA_PRIME_STATIC)
endif ()

add_library(${BUNDLE_NAME} ${LIBRARY_TYPE} ${BUNDLE_SOURCES})
target_link_libraries(${BUNDLE_NAME} sonia-prime-lib ${OPENSSL} ${ZLIB_LIB} ${BOOST_SERIALIZATION} ${BOOST_FILESYSTEM} ${BOOST_FIBERS} ${BOOST_THREAD} ${BOOST_PROGRAM_OPTIONS} ${BOOST_CONTEXT} ${BOOST_REGEX} ${PLATFORM_LIBS})
set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")