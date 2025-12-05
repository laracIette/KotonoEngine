cmake_minimum_required(VERSION 3.20)
project(KotonoInput LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoInput STATIC)

# Require C++20
target_compile_features(KotonoInput PUBLIC cxx_std_20)

file(GLOB_RECURSE KINPUT_SOURCES "src/*.cpp")
file(GLOB_RECURSE KINPUT_HEADERS "include/kotono_input/*.h")

target_sources(KotonoInput
    PRIVATE ${KINPUT_SOURCES}
    PRIVATE ${KINPUT_HEADERS}
)

# Include directories
target_include_directories(KotonoInput
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_input"
)

# Output .lib to KotonoInput/lib/
set_target_properties(KotonoInput PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Link libs
target_link_libraries(KotonoInput PUBLIC
    KotonoPlatform
)
