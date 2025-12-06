cmake_minimum_required(VERSION 3.20)
project(KotonoEditor LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoEditor STATIC)

# Require C++20
target_compile_features(KotonoEditor PUBLIC cxx_std_20)

file(GLOB_RECURSE KEDITOR_SOURCES "src/*.cpp")
file(GLOB_RECURSE KEDITOR_HEADERS "include/kotono_editor/*.h")

target_sources(KotonoEditor
    PRIVATE ${KEDITOR_SOURCES}
    PRIVATE ${KEDITOR_HEADERS}
)

# Include directories
target_include_directories(KotonoEditor
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_editor"
)

# Link libs
target_link_libraries(KotonoEditor PUBLIC
    KotonoCore
    KotonoInterface
)