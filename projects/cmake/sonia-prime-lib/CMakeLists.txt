include (${COMMON_INCLUDE})

include_directories(${PROJECT_HOME})
include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)
include_directories(${TPLS}/lexertl14/include)

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
    ${BUNDLE_HOME}/sonia/bang/bang.cpp
    ${BUNDLE_HOME}/sonia/bang/bang.tab.cpp
    ${BUNDLE_HOME}/sonia/bang/bang.yy.cpp
    ${BUNDLE_HOME}/sonia/bang/bang_errors.cpp
    ${BUNDLE_HOME}/sonia/bang/bang_parser.cpp
    ${BUNDLE_HOME}/sonia/bang/bang_semantic.cpp
    ${BUNDLE_HOME}/sonia/bang/bang_unit.cpp
    ${BUNDLE_HOME}/sonia/bang/bang_auxiliary.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/assign_expression_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/base_expression_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/ct_expression_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/declaration_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/expression_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/fn_compiler_context.cpp
    ${BUNDLE_HOME}/sonia/bang/ast/functional_declaration_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/functional.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/prepared_call.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/type_entity.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/variable_entity.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/collections/array_elements_implicit_cast_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/collections/array_implicit_cast_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/ellipsis/ellipsis_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/enum/enum_entity.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/enum/enum_implicit_cast_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/functions/create_identifier_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/functions/expression_resolver.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/functions/expression_resolver.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/functions/function_entity.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/functions/internal_function_entity.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/literals/numeric_implicit_cast_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_argument_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_bit_and_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_empty_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_head_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_tail_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/metaobject/metaobject_typeof_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/struct/struct_entity.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/struct/struct_fn_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/struct/struct_get_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/struct/struct_implicit_cast_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/struct/struct_init_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/struct/struct_new_pattern.cpp

    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_empty_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_get_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_head_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_make_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_implicit_cast_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_pattern_base.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_set_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_size_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/tuple/tuple_tail_pattern.cpp

    ${BUNDLE_HOME}/sonia/bang/entities/union/union_bit_or_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/entities/union/union_implicit_cast_pattern.cpp

    ${BUNDLE_HOME}/sonia/bang/functional/basic_fn_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/external_fn_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/signature_matcher.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/value_match_visitor.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/value_type_match_visitor.cpp

    ${BUNDLE_HOME}/sonia/bang/functional/general/assert_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/general/deref_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/general/equal_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/general/mut_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/general/to_string_pattern.cpp
    ${BUNDLE_HOME}/sonia/bang/functional/general/typeof_pattern.cpp

    ${BUNDLE_HOME}/sonia/bang/semantic/semantic_expression_printer.cpp

    ${BUNDLE_HOME}/sonia/bang/errors/assign_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/circular_dependency_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/function_call_match_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/identifier_redefinition_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/no_position_argument_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/pattern_match_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/type_mismatch_error.cpp
    ${BUNDLE_HOME}/sonia/bang/errors/value_mismatch_error.cpp

    ${BUNDLE_HOME}/sonia/bang/library/bang_library.cpp
    ${BUNDLE_HOME}/sonia/bang/vm/bang_vm.cpp
    ${BUNDLE_HOME}/sonia/sal/sal.cpp
    ${BUNDLE_HOME}/sonia/net/uri.cpp
    ${BUNDLE_HOME}/sonia/utility/assert_handler.cpp
    ${BUNDLE_HOME}/sonia/utility/file_persister.cpp
    ${BUNDLE_HOME}/sonia/utility/json/json_value.cpp
    ${BUNDLE_HOME}/sonia/utility/parsers/json/json_model.cpp
    ${BUNDLE_HOME}/sonia/utility/parameters/parameters.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/file_region_iterator.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/tar_iterator.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/archive_extract_iterator.cpp
    ${BUNDLE_HOME}/sonia/utility/lua/lua.cpp
    ${BUNDLE_HOME}/sonia/utility/lua/variant_lib.cpp
    ${BUNDLE_HOME}/sonia/utility/lua/bigint_lib.cpp
    ${BUNDLE_HOME}/sonia/utility/lua/sonia_lib.cpp
    ${BUNDLE_HOME}/sonia/utility/invocation/invocable.cpp
    ${BUNDLE_HOME}/sonia/utility/invocation/value_encoder.cpp
    ${BUNDLE_HOME}/sonia/utility/invocation/value_decoder.cpp
    ${BUNDLE_HOME}/sonia/utility/mvvm/view_model.cpp
    ${BUNDLE_HOME}/sonia/utility/mvvm/lua_view_model.cpp
    ${BUNDLE_HOME}/sonia/utility/mvvm/bang_view_model.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/xmlbuilder.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/bunch_builder.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/attr_parser.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/basic_attribute_resolver.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/ui_attribute_resolver.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/detail/xmlbuilder_types.cpp
    ${BUNDLE_HOME}/sonia/utility/xmlbuilder/detail/xmlbuilder_model.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/compound.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/code_element.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/document_element.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/foreach_element.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/file_template_element.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/lua_build_visitor.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/templated_unit.cpp
    ${BUNDLE_HOME}/sonia/utility/template-engine/templated_bunch.cpp
    ${BUNDLE_HOME}/sonia/services/singleton_locator.cpp
    ${BUNDLE_HOME}/sonia/services/local_type_registry.cpp
    ${BUNDLE_HOME}/sonia/services/durable_type_registry.cpp
    ${BUNDLE_HOME}/sonia/services/multimethod_registry.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/basic_scheduler.cpp
    ${BUNDLE_HOME}/sonia/services/scheduler/fiber_work_stealing_scheduler.cpp
    ${BUNDLE_HOME}/sonia/services/io/file.cpp
    
)

if (NOT DEFINED NO_NET)
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/net/http/http.cpp
        ${BUNDLE_HOME}/sonia/net/http/http_message.cpp
        ${BUNDLE_HOME}/sonia/net/http/sonnet.cpp
        ${BUNDLE_HOME}/sonia/services/net/http/mime_mapping.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory.cpp
        ${BUNDLE_HOME}/sonia/services/io/socket_address.cpp
    )

    if (NOT DEFINED NO_SSL)
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/services/io/io_ssl_factory.cpp
        )
    else()
        add_definitions(-DSONIA_NO_SSL)
    endif()

else()
    add_definitions(-DSONIA_NO_NET)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sys/windows/path.cpp
        ${BUNDLE_HOME}/sonia/sys/windows/windows.cpp
        ${BUNDLE_HOME}/sonia/sys/windows/thread_pool_api.cpp
        ${BUNDLE_HOME}/sonia/sys/windows/thread_pool.cpp
        ${BUNDLE_HOME}/sonia/sal/windows_sal.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_win.cpp
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sys/posix/posix.cpp
        #${BUNDLE_HOME}/sonia/sys/linux/linux.cpp
        ${BUNDLE_HOME}/sonia/sys/linux/signals_api.cpp
        ${BUNDLE_HOME}/sonia/sys/linux/signals.cpp
        ${BUNDLE_HOME}/sonia/sys/linux/linux_timer.cpp
        ${BUNDLE_HOME}/sonia/sal/posix_sal.cpp
        ${BUNDLE_HOME}/sonia/sal/linux_sal.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_linux.cpp
    )
    if (CMAKE_SYSTEM_NAME STREQUAL "Android")
        set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
            ${BUNDLE_HOME}/sonia/logger/android_sink.cpp
            ${BUNDLE_HOME}/sonia/java/jni_env.cpp
            ${BUNDLE_HOME}/sonia/utility/invocation/jni_decoder.cpp
            ${BUNDLE_HOME}/sonia/utility/invocation/jni_encoder.cpp
        )
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES}
        ${BUNDLE_HOME}/sonia/sys/posix/posix.cpp
        ${BUNDLE_HOME}/sonia/sys/macos/dispatch.cpp
        ${BUNDLE_HOME}/sonia/sal/posix_sal.cpp
        ${BUNDLE_HOME}/sonia/sal/macos_sal.cpp
        ${BUNDLE_HOME}/sonia/services/io/io_factory_macos.cpp
    )
endif ()

add_definitions(-DSONIA_PRIME_STATIC)

add_library(${BUNDLE_NAME} STATIC ${BUNDLE_SOURCES})

set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")
