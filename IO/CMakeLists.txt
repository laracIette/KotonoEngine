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

set(KIO_HEADERS
    "include/kotono_io/File.h"
    "include/kotono_io/FileExplorer.h"
    "include/kotono_io/Serializer.h"
)
set(KIO_SOURCES
    "src/File.cpp"
    "src/FileExplorer.cpp"
    "src/Serializer.cpp"
)

target_sources(KotonoIO
    PRIVATE ${KIO_HEADERS}
    PRIVATE ${KIO_SOURCES}
)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

# Include directories
target_include_directories(KotonoIO
    PUBLIC  "${THIRD_PARTY_DIR}/nlohmann/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_io"
)

# Link libs
target_link_libraries(KotonoIO PUBLIC
    KotonoCommon
)
