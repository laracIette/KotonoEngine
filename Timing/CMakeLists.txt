cmake_minimum_required(VERSION 3.20)
project(KotonoTiming LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoTiming STATIC)

# Require C++20
target_compile_features(KotonoTiming PUBLIC cxx_std_20)

file(GLOB_RECURSE KTIMING_SOURCES "src/*.cpp")
file(GLOB_RECURSE KTIMING_HEADERS "include/kotono_timing/*.h")

target_sources(KotonoTiming
    PRIVATE ${KTIMING_SOURCES}
    PRIVATE ${KTIMING_HEADERS}
)

# Include directories
target_include_directories(KotonoTiming
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_timing"
)

# Link libs
target_link_libraries(KotonoTiming PUBLIC
    KotonoCommon
)
