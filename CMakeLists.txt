cmake_minimum_required(VERSION 3.20)

project(sonia-prime VERSION 0.1.0 LANGUAGES C CXX ASM)

set(PROJECT_HOME ${CMAKE_CURRENT_SOURCE_DIR})
set(SONIA_HOME ${CMAKE_CURRENT_SOURCE_DIR})
set(COMMON_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/projects/cmake/angel/common.cmake)
set(MODULES ${CMAKE_CURRENT_SOURCE_DIR}/modules)
set(TPLS ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(NUMETRON_HOME ${MODULES}/numetron)
set(DATAFORGE_HOME ${MODULES}/dataforge)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Self-contained build: every third party library is linked statically.
option(ANGEL_FULLY_STATIC "Link angel fully statically (including libc)" OFF)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "This CMake project builds angel for Linux only; use projects/vs/angel on Windows")
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS
    log log_setup serialization filesystem thread program_options context regex random chrono atomic date_time)

find_package(Threads REQUIRED)

# static libraries only
set(_angel_saved_suffixes ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_library(LZMA_LIBRARY NAMES lzma REQUIRED)
find_library(OPENSSL_SSL_LIB NAMES ssl REQUIRED)
find_library(OPENSSL_CRYPTO_LIB NAMES crypto REQUIRED)
find_library(LUA_LIBRARY NAMES lua5.4 lua5.3 lua REQUIRED)
find_library(ZSTD_LIBRARY NAMES zstd REQUIRED)
find_library(JITTERENTROPY_LIBRARY NAMES jitterentropy REQUIRED)
set(CMAKE_FIND_LIBRARY_SUFFIXES ${_angel_saved_suffixes})
find_path(LUA_INCLUDE_DIR NAMES lua.hpp PATH_SUFFIXES lua5.4 lua5.3 lua REQUIRED)
find_path(LUA_MULTIARCH_INCLUDE_DIR NAMES lua5.4-deb-multiarch.h PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE})
find_path(LZMA_INCLUDE_DIR NAMES lzma.h REQUIRED)
find_path(OPENSSL_INCLUDE_DIR NAMES openssl/ssl.h REQUIRED)

# Variables consumed by projects/cmake/sonia-prime*/CMakeLists.txt
set(ZLIB ${ZLIB_LIBRARIES})
set(BZLIB ${BZIP2_LIBRARIES})
set(LIBLZMA ${LZMA_LIBRARY})
set(OPENSSL ${OPENSSL_SSL_LIB} ${OPENSSL_CRYPTO_LIB} ${ZSTD_LIBRARY} ${JITTERENTROPY_LIBRARY})
set(LUALIB ${LUA_LIBRARY})
set(BOOST_LOG_SETUP Boost::log_setup)
set(BOOST_LOG Boost::log)
set(BOOST_SERIALIZATION Boost::serialization)
set(BOOST_FILESYSTEM Boost::filesystem)
set(BOOST_THREAD Boost::thread)
set(BOOST_PROGRAM_OPTIONS Boost::program_options)
set(BOOST_CONTEXT Boost::context)
set(BOOST_REGEX Boost::regex)
set(BOOST_RANDOM Boost::random)
set(BOOST_STACKTRACE)
set(PLATFORM_LIBS Threads::Threads ${CMAKE_DL_LIBS})

include_directories(${LUA_INCLUDE_DIR} ${LUA_MULTIARCH_INCLUDE_DIR} ${LZMA_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS} ${BZIP2_INCLUDE_DIRS})
add_compile_definitions(SONIA_PRIME_STATIC)
set(LIBRARY_TYPE STATIC)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/cmake/sonia-prime-lib "${CMAKE_BINARY_DIR}/sonia-prime-lib")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/cmake/sonia-prime "${CMAKE_BINARY_DIR}/sonia-prime")
add_subdirectory(${MODULES}/numetron/lib "${CMAKE_BINARY_DIR}/numetron-lib")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/cmake/angel "${CMAKE_BINARY_DIR}/angel")

