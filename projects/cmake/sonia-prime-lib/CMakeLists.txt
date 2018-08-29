include (${COMMON_INCLUDE})

include_directories(${BUNDLES})
include_directories(${BUNDLES}/sonia-prime)

set (BUNDLE_HOME ${BUNDLES}/sonia-prime)
set (BUNDLE_NAME sonia-prime-lib)
set (BUNDLE_SOURCES
    ${BUNDLE_HOME}/sonia/utility/assert_handler.cpp
    ${BUNDLE_HOME}/sonia/utility/file_persister.cpp
    ${BUNDLE_HOME}/sonia/utility/json/json_value.cpp
    ${BUNDLE_HOME}/sonia/utility/parsers/json/json_model.cpp
    ${BUNDLE_HOME}/sonia/utility/iterators/file_region_iterator.cpp
)

add_library(${BUNDLE_NAME} STATIC ${BUNDLE_SOURCES})
#target_link_libraries(${BUNDLE_NAME} ${ZLIB_LIB})
set_target_properties(${BUNDLE_NAME} PROPERTIES FOLDER "libs")

