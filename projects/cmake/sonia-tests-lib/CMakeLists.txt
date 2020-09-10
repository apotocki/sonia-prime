include (${COMMON_INCLUDE})

include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)
include_directories(${TPLS}/lexertl14/include)

set (BUNDLE_NAME sonia-tests-lib)
set (BUNDLE_HOME ${BUNDLES}/sonia-tests-lib)

set (MAIN_TESTS
    timer_test
    bit_coder_test
    bookkeeper_test
    cmp_test
    marshaling_test
    file_persister_test
    json_parser_test
    iterators_test
    number_test
    optimized_test
    serialization_stream_test
    prime_test
    tls_crash_test
    server_test
    transceiver_test
    http_iterators_test
    http_test
    uri_test
)

set (AGNOSTIC_TESTS
    vector_test
    avl_test
)

set (BUNDLE_SOURCES
	${BUNDLES}/sonia-prime/applied/tests_external.cpp
    ${BUNDLES}/sonia-prime/applied/scoped_services.cpp
    ${BUNDLES}/sonia-prime/tests/all_tests_registrar.cpp
)

foreach(ITEM ${MAIN_TESTS})
    set (FWD_TEST_DECLS "${FWD_TEST_DECLS}void ${ITEM}_registrar();\n")
    set (TEST_LIST "${TEST_LIST}    ${ITEM}_registrar();\n")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES} ${BUNDLES}/sonia-prime/tests/${ITEM}.cpp)
endforeach()
    
if (NOT (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS"))
foreach(ITEM ${AGNOSTIC_TESTS})
    set (FWD_TEST_DECLS "${FWD_TEST_DECLS}void agnostic_${ITEM}_registrar();\n")
    set (TEST_LIST "${TEST_LIST}    agnostic_${ITEM}_registrar();\n")
    set (BUNDLE_SOURCES ${BUNDLE_SOURCES} ${BUNDLES}/sonia-prime/tests/agnostic/${ITEM}.cpp)
endforeach()
endif()

configure_file (
    "${BUNDLES}/sonia-prime/tests/all_tests_registrar.cpp.in"
    "${BUNDLES}/sonia-prime/tests/all_tests_registrar.cpp"
)

#FILE(WRITE ${BUNDLES}/sonia-prime/tests/all_tests_registrar.cpp ${FILE_CONTENT})

add_definitions(-DSONIA_PRIME_STATIC)

add_library(${BUNDLE_NAME} STATIC ${BUNDLE_SOURCES})

sonia_link_library(${BUNDLE_NAME} sonia-prime)
sonia_link_library(${BUNDLE_NAME} sonia-prime-lib)

target_link_libraries(${BUNDLE_NAME} ${BOOST_TEST_LIB})

set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")
