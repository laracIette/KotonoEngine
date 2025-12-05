cmake_minimum_required(VERSION 3.20)
project(KotonoIO LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoIO STATIC)

# Require C++20
target_compile_features(KotonoIO PUBLIC cxx_std_20)

file(GLOB_RECURSE KIO_SOURCES "src/*.cpp")
file(GLOB_RECURSE KIO_HEADERS "include/kotono_io/*.h")

target_sources(KotonoIO
    PRIVATE ${KIO_SOURCES}
    PRIVATE ${KIO_HEADERS}
)

# Include directories
target_include_directories(KotonoIO
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_io"
)

# Output .lib to KotonoIO/lib/
set_target_properties(KotonoIO PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Link libs
target_link_libraries(KotonoIO PUBLIC
    KotonoCommon
)
