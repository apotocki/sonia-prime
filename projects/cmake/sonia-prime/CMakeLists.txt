include (${COMMON_INCLUDE})

include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)

set (BUNDLE_NAME sonia-prime)
set (BUNDLE_HOME ${BUNDLES}/sonia-prime)
set (BUNDLE_SOURCES
    ${BUNDLE_HOME}/sonia/services/services.cpp
    ${BUNDLE_HOME}/sonia/services/environment.cpp
    ${BUNDLE_HOME}/sonia/services/services_host.cpp
    ${BUNDLE_HOME}/sonia/services/service_locator.cpp
    ${BUNDLE_HOME}/sonia/services/basic_service_factory.cpp
    ${BUNDLE_HOME}/sonia/services/local_service_registry.cpp
    ${BUNDLE_HOME}/sonia/services/local_type_registry.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/scheduler_builder.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/scheduler_service.cpp
    ${BUNDLE_HOME}/sonia/services/aio/aio_builder.cpp
    ${BUNDLE_HOME}/sonia/services/aio/aio_service.cpp
    ${BUNDLE_HOME}/sonia/utility/posix/signals.cpp
    ${BUNDLE_HOME}/sonia/services/prime_bundle.cpp
)

add_definitions(-DSONIA_PRIME_EXPORTS)

if ("${LIBRARY_TYPE}" STREQUAL "STATIC")
	add_definitions(-DSONIA_PRIME_STATIC)
    set_property(GLOBAL PROPERTY ${BUNDLE_NAME}-import-flags -DSONIA_PRIME_STATIC)
	#set_property(GLOBAL PROPERTY margot-core-import-flags -DMARGOT_CORE_STATIC -DMARGOT_ON_CLOSE_STATIC -DMARGOT_TYPE_IDENTIFIER_STATIC -DMARGOT_STRORABLE_STATIC -DDMARGOT_COMMAND_STATIC -DMARGOT_METHODS_STATIC -DMARGOT_SINGLETON_STATIC -DMARGOT_THREAD_API_STATIC)
endif ()

add_library(${BUNDLE_NAME} ${LIBRARY_TYPE} ${BUNDLE_SOURCES})
target_link_libraries(${BUNDLE_NAME} sonia-prime-lib ${BOOST_SERIALIZATION} ${BOOST_FILESYSTEM} ${BOOST_FIBERS} ${BOOST_THREAD} ${BOOST_PROGRAM_OPTIONS} ${BOOST_CONTEXT} ${BOOST_FIBERS} ${BOOST_SYSTEM} ${PLATFORM_LIBS})
set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")