cmake_minimum_required(VERSION 3.20)
project(SPIRVReflect LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(SPIRVReflect STATIC)

# Require C++20
target_compile_features(SPIRVReflect PUBLIC cxx_std_20)

file(GLOB_RECURSE SREFLECT_SOURCES "src/*.c")
file(GLOB_RECURSE SREFLECT_HEADERS "include/spirv-reflect/*.h")

target_sources(SPIRVReflect
    PRIVATE ${SREFLECT_SOURCES}
    PRIVATE ${SREFLECT_HEADERS}
)

# Include directories
target_include_directories(SPIRVReflect
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/spirv-reflect"
)
