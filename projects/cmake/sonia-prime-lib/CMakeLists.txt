include (${COMMON_INCLUDE})

set (SONIA_HOME ${CMAKE_CURRENT_LIST_DIR}/../../..)

include_directories(${PROJECT_HOME})
include_directories(${SONIA_HOME})
include_directories(${TPLS}/numetron/include)
include_directories(${TPLS}/dataforge/include)
include_directories(${TPLS}/lexertl14/include)

set (BUNDLE_NAME sonia-prime-lib)

set (BUNDLE_SOURCES
    ${SONIA_HOME}/sonia/logger/logger.cpp
    ${SONIA_HOME}/sonia/services/bundle.cpp
    ${SONIA_HOME}/sonia/fibers/context.cpp
    ${SONIA_HOME}/sonia/fibers/scheduler.cpp
    ${SONIA_HOME}/sonia/fibers/mutex.cpp
    ${SONIA_HOME}/sonia/fibers/postponer.cpp
    ${SONIA_HOME}/sonia/fibers/future.cpp
    ${SONIA_HOME}/sonia/fibers/fiber.cpp
    ${SONIA_HOME}/sonia/fibers/condition_variable.cpp
    ${SONIA_HOME}/sonia/fibers/barrier.cpp
    ${SONIA_HOME}/sonia/fibers/algo/algorithm.cpp
    ${SONIA_HOME}/sonia/fibers/algo/round_robin.cpp
    
    ${SONIA_HOME}/sonia/sal/sal.cpp
    ${SONIA_HOME}/sonia/net/uri.cpp
    ${SONIA_HOME}/sonia/utility/assert_handler.cpp
    ${SONIA_HOME}/sonia/utility/file_persister.cpp
    ${SONIA_HOME}/sonia/utility/json/json_value.cpp
    ${SONIA_HOME}/sonia/utility/parsers/json/json_model.cpp
    ${SONIA_HOME}/sonia/utility/parameters/parameters.cpp
    ${SONIA_HOME}/sonia/utility/iterators/file_region_iterator.cpp
    ${SONIA_HOME}/sonia/utility/iterators/tar_iterator.cpp
    ${SONIA_HOME}/sonia/utility/iterators/archive_extract_iterator.cpp
    
    ${SONIA_HOME}/sonia/utility/invocation/invocable.cpp
    ${SONIA_HOME}/sonia/utility/invocation/value_encoder.cpp
    ${SONIA_HOME}/sonia/utility/invocation/value_decoder.cpp
    ${SONIA_HOME}/sonia/utility/mvvm/view_model.cpp
    
    ${SONIA_HOME}/sonia/utility/xmlbuilder/xmlbuilder.cpp
    ${SONIA_HOME}/sonia/utility/xmlbuilder/bunch_builder.cpp
    ${SONIA_HOME}/sonia/utility/xmlbuilder/attr_parser.cpp
    ${SONIA_HOME}/sonia/utility/xmlbuilder/basic_attribute_resolver.cpp
    ${SONIA_HOME}/sonia/utility/xmlbuilder/ui_attribute_resolver.cpp
    ${SONIA_HOME}/sonia/utility/xmlbuilder/detail/xmlbuilder_types.cpp
    ${SONIA_HOME}/sonia/utility/xmlbuilder/detail/xmlbuilder_model.cpp
    
    ${SONIA_HOME}/sonia/services/singleton_locator.cpp
    ${SONIA_HOME}/sonia/services/local_type_registry.cpp
    ${SONIA_HOME}/sonia/services/durable_type_registry.cpp
    ${SONIA_HOME}/sonia/services/multimethod_registry.cpp
    ${SONIA_HOME}/sonia/services/scheduler/basic_scheduler.cpp
    ${SONIA_HOME}/sonia/services/scheduler/fiber_work_stealing_scheduler.cpp
    ${SONIA_HOME}/sonia/services/io/file.cpp
    
)

if (NOT DEFINED NO_LUA)
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${SONIA_HOME}/sonia/utility/lua/lua.cpp
        ${SONIA_HOME}/sonia/utility/lua/variant_lib.cpp
        ${SONIA_HOME}/sonia/utility/lua/bigint_lib.cpp
        ${SONIA_HOME}/sonia/utility/lua/sonia_lib.cpp
        ${SONIA_HOME}/sonia/utility/mvvm/lua_view_model.cpp
        
        ${SONIA_HOME}/sonia/utility/template-engine/compound.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/code_element.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/document_element.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/foreach_element.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/file_template_element.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/lua_build_visitor.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/templated_unit.cpp
        ${SONIA_HOME}/sonia/utility/template-engine/templated_bunch.cpp
    )
else()
    add_definitions(-DSONIA_NO_LUA)
endif ()
if (NOT DEFINED NO_NET)
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${SONIA_HOME}/sonia/net/http/http.cpp
        ${SONIA_HOME}/sonia/net/http/http_message.cpp
        ${SONIA_HOME}/sonia/net/http/sonnet.cpp
        ${SONIA_HOME}/sonia/services/net/http/mime_mapping.cpp
        ${SONIA_HOME}/sonia/services/io/io_factory.cpp
        ${SONIA_HOME}/sonia/services/io/socket_address.cpp
    )

    if (NOT DEFINED NO_SSL)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${SONIA_HOME}/sonia/services/io/io_ssl_factory.cpp
        )
    else()
        add_definitions(-DSONIA_NO_SSL)
    endif()

else()
    add_definitions(-DSONIA_NO_NET)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${SONIA_HOME}/sonia/sys/windows/path.cpp
        ${SONIA_HOME}/sonia/sys/windows/windows.cpp
        ${SONIA_HOME}/sonia/sys/windows/thread_pool_api.cpp
        ${SONIA_HOME}/sonia/sys/windows/thread_pool.cpp
        ${SONIA_HOME}/sonia/sal/windows_sal.cpp
        ${SONIA_HOME}/sonia/services/io/io_factory_win.cpp
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${SONIA_HOME}/sonia/sys/posix/posix.cpp
        #${SONIA_HOME}/sonia/sys/linux/linux.cpp
        ${SONIA_HOME}/sonia/sys/linux/signals_api.cpp
        ${SONIA_HOME}/sonia/sys/linux/signals.cpp
        ${SONIA_HOME}/sonia/sys/linux/linux_timer.cpp
        ${SONIA_HOME}/sonia/sal/posix_sal.cpp
        ${SONIA_HOME}/sonia/sal/linux_sal.cpp
        ${SONIA_HOME}/sonia/services/io/io_factory_linux.cpp
    )
    if (CMAKE_SYSTEM_NAME STREQUAL "Android")
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${SONIA_HOME}/sonia/logger/android_sink.cpp
            ${SONIA_HOME}/sonia/java/jni_env.cpp
            ${SONIA_HOME}/sonia/utility/invocation/jni_decoder.cpp
            ${SONIA_HOME}/sonia/utility/invocation/jni_encoder.cpp
            ${SONIA_HOME}/sonia/utility/invocation/jni_invoker.cpp
        )
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${SONIA_HOME}/sonia/sys/posix/posix.cpp
        ${SONIA_HOME}/sonia/sys/macos/dispatch.cpp
        ${SONIA_HOME}/sonia/sal/posix_sal.cpp
        ${SONIA_HOME}/sonia/sal/macos_sal.cpp
        ${SONIA_HOME}/sonia/services/io/io_factory_macos.cpp
    )
endif ()

add_definitions(-DSONIA_PRIME_STATIC)

add_library(${BUNDLE_NAME} STATIC ${BUNDLE_SOURCES})
target_include_directories(${BUNDLE_NAME} 
    PRIVATE ${Boost_INCLUDE_DIRS}
    INTERFACE ${Boost_INCLUDE_DIRS}
)
set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")
target_compile_options(${BUNDLE_NAME} PRIVATE -Ulinux)