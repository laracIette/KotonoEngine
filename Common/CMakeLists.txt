cmake_minimum_required(VERSION 3.20)
project(KotonoCommon LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoCommon STATIC)

# Require C++20
target_compile_features(KotonoCommon PUBLIC cxx_std_20)

file(GLOB_RECURSE KCOMMON_SOURCES "src/*.cpp")
file(GLOB_RECURSE KCOMMON_HEADERS "include/kotono_common/*.h")

target_sources(KotonoCommon
    PRIVATE ${KCOMMON_SOURCES}
    PRIVATE ${KCOMMON_HEADERS}
)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

# Include directories
target_include_directories(KotonoCommon
    PUBLIC  "${THIRD_PARTY_DIR}/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_common"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoCommon PUBLIC
    NOMINMAX
    ROOT_DIRECTORY="${CMAKE_SOURCE_DIR}"
)
