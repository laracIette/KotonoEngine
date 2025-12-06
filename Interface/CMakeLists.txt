cmake_minimum_required(VERSION 3.20)
project(KotonoInterface LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoInterface STATIC)

# Require C++20
target_compile_features(KotonoInterface PUBLIC cxx_std_20)

file(GLOB_RECURSE KINTERFACE_SOURCES "src/*.cpp")
file(GLOB_RECURSE KINTERFACE_HEADERS "include/kotono_interface/*.h")

target_sources(KotonoInterface
    PRIVATE ${KINTERFACE_SOURCES}
    PRIVATE ${KINTERFACE_HEADERS}
)

# Include directories
target_include_directories(KotonoInterface
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_interface"
)

# Output .lib to Interface/lib/
set_target_properties(KotonoInterface PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Link libs
target_link_libraries(KotonoInterface PUBLIC
    KotonoInput
    KotonoGraphics
)
