include_directories(${SONIA_HOME})
include_directories(${NUMETRON_HOME}/include)
include_directories(${DATAFORGE_HOME}/include)
include_directories(${TPLS}/lexertl14/include)

add_executable(angel
    ${SONIA_HOME}/applied/linux_console.cpp
    ${SONIA_HOME}/applied/scoped_services.cpp
)

# sonia-prime-lib goes after sonia-prime: the former is a dependency of the latter and
# static archives are resolved in order; the group makes the remaining cross references safe.
target_link_libraries(angel PRIVATE
    "$<LINK_GROUP:RESCAN,sonia-prime,sonia-prime-lib,numetron>"
    ${OPENSSL} ${LUALIB} ${LIBLZMA} ${BZLIB} ${ZLIB}
    Boost::log_setup Boost::log Boost::serialization Boost::filesystem Boost::thread
    Boost::program_options Boost::context Boost::regex Boost::random Boost::system Boost::chrono
    Boost::atomic Boost::date_time
    ${PLATFORM_LIBS})

target_compile_options(angel PRIVATE -Ulinux -Wno-deprecated-declarations)

if (ANGEL_FULLY_STATIC)
    target_link_options(angel PRIVATE -static)
else()
    target_link_options(angel PRIVATE -static-libstdc++ -static-libgcc)
endif()
