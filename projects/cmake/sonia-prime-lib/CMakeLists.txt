include (${COMMON_INCLUDE})

include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)

set (BUNDLE_NAME sonia-prime-lib)
set (BUNDLE_HOME ${BUNDLES}/sonia-prime)
set (BUNDLE_SOURCES
    ${BUNDLE_HOME}/sonia/logger/logger.cpp
    ${BUNDLE_HOME}/sonia/services/bundle.cpp
    ${BUNDLE_HOME}/sonia/utility/assert_handler.cpp
    ${BUNDLE_HOME}/sonia/utility/file_persister.cpp
    ${BUNDLE_HOME}/sonia/utility/json/json_value.cpp
    ${BUNDLE_HOME}/sonia/utility/parsers/json/json_model.cpp
    ${BUNDLE_HOME}/sonia/utility/parameters/parameters.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/file_region_iterator.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/basic_scheduler.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/fiber_work_stealing_scheduler.cpp
    ${BUNDLE_HOME}/sonia/services/io/file.cpp
)

if ( CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sal/windows_sal.cpp
        ${BUNDLE_HOME}/sonia/utility/windows.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_win.cpp
    )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sal/linux_sal.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_linux.cpp
    )
endif ()

add_library(${BUNDLE_NAME} STATIC ${BUNDLE_SOURCES})
target_link_libraries(${BUNDLE_NAME} ${BOOST_LOG} ${BOOST_LOG_SETUP})
#target_link_libraries(${BUNDLE_NAME} ${ZLIB_LIB})
set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")

