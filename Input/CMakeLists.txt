cmake_minimum_required(VERSION 3.20)
project(KotonoInput LANGUAGES C CXX)

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

set(KINPUT_HEADERS
    "include/kotono_input/Button.h"
    "include/kotono_input/InputManager.h"
    "include/kotono_input/InputState.h"
    "include/kotono_input/Key.h"
    "include/kotono_input/Keyboard.h"
    "include/kotono_input/Mouse.h"
)
set(KINPUT_SOURCES
    "src/InputManager.cpp"
    "src/Keyboard.cpp"
    "src/Mouse.cpp"
)

target_sources(KotonoInput
    PRIVATE ${KINPUT_HEADERS}
    PRIVATE ${KINPUT_SOURCES}
)

# Include directories
target_include_directories(KotonoInput
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_input"
)

# Link libs
target_link_libraries(KotonoInput PUBLIC
    KotonoPlatform
)
