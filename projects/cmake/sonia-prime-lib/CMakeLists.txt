include (${COMMON_INCLUDE})

include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)

set (BUNDLE_NAME sonia-prime-lib)
set (BUNDLE_HOME ${BUNDLES}/sonia-prime)

set (BUNDLE_SOURCES
    ${BUNDLE_HOME}/sonia/logger/logger.cpp
    ${BUNDLE_HOME}/sonia/services/bundle.cpp
    ${BUNDLE_HOME}/sonia/fibers/context.cpp
    ${BUNDLE_HOME}/sonia/fibers/scheduler.cpp
    ${BUNDLE_HOME}/sonia/fibers/mutex.cpp
    ${BUNDLE_HOME}/sonia/fibers/postponer.cpp
    ${BUNDLE_HOME}/sonia/fibers/future.cpp
    ${BUNDLE_HOME}/sonia/fibers/fiber.cpp
    ${BUNDLE_HOME}/sonia/fibers/condition_variable.cpp
    ${BUNDLE_HOME}/sonia/fibers/barrier.cpp
    ${BUNDLE_HOME}/sonia/fibers/algo/algorithm.cpp
    ${BUNDLE_HOME}/sonia/fibers/algo/round_robin.cpp
    ${BUNDLE_HOME}/sonia/sal/sal.cpp
    ${BUNDLE_HOME}/sonia/net/uri.cpp
    ${BUNDLE_HOME}/sonia/net/http/http.cpp
    ${BUNDLE_HOME}/sonia/net/http/http_message.cpp
    ${BUNDLE_HOME}/sonia/net/http/sonnet.cpp
    ${BUNDLE_HOME}/sonia/utility/assert_handler.cpp
    ${BUNDLE_HOME}/sonia/utility/file_persister.cpp
    ${BUNDLE_HOME}/sonia/utility/json/json_value.cpp
    ${BUNDLE_HOME}/sonia/utility/parsers/json/json_model.cpp
    ${BUNDLE_HOME}/sonia/utility/parameters/parameters.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/file_region_iterator.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/tar_iterator.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/archive_extract_iterator.cpp
    ${BUNDLE_HOME}/sonia/utility/number/number_math.cpp
    ${BUNDLE_HOME}/sonia/services/singleton_locator.cpp
    ${BUNDLE_HOME}/sonia/services/local_type_registry.cpp
    ${BUNDLE_HOME}/sonia/services/durable_type_registry.cpp
    ${BUNDLE_HOME}/sonia/services/multimethod_registry.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/basic_scheduler.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/fiber_work_stealing_scheduler.cpp
    ${BUNDLE_HOME}/sonia/services/io/file.cpp
    ${BUNDLE_HOME}/sonia/services/io/io_factory.cpp
    ${BUNDLE_HOME}/sonia/services/io/io_ssl_factory.cpp
    ${BUNDLE_HOME}/sonia/services/io/socket_address.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sys/windows/windows.cpp
        ${BUNDLE_HOME}/sonia/sys/windows/thread_pool_api.cpp
        ${BUNDLE_HOME}/sonia/sys/windows/thread_pool.cpp
        ${BUNDLE_HOME}/sonia/sal/windows_sal.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_win.cpp
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sys/linux/linux.cpp
        ${BUNDLE_HOME}/sonia/sys/linux/signals_api.cpp
        ${BUNDLE_HOME}/sonia/sys/linux/linux_timer.cpp
        ${BUNDLE_HOME}/sonia/sal/linux_sal.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_linux.cpp
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sys/macos/dispatch.cpp
        ${BUNDLE_HOME}/sonia/sal/macos_sal.cpp
        ${BUNDLE_HOME}/sonia/sys/linux//linux.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_macos.cpp
    )
endif ()

add_definitions(-DSONIA_PRIME_STATIC)

add_library(${BUNDLE_NAME} STATIC ${BUNDLE_SOURCES})

set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")
