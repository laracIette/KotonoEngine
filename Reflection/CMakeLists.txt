cmake_minimum_required(VERSION 3.20)
project(KotonoReflection LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoReflection STATIC)

# Require C++20
target_compile_features(KotonoReflection PUBLIC cxx_std_23)

set_target_properties(KotonoReflection PROPERTIES FOLDER "KotonoEngine")

set(KREFLECTION_HEADERS
    "include/kotono_reflection/ReflectionResult.h"
    "include/kotono_reflection/Reflector.h"
)
set(KREFLECTION_SOURCES
    "src/Reflector.cpp"
)

target_sources(KotonoReflection
    PRIVATE ${KREFLECTION_HEADERS}
    PRIVATE ${KREFLECTION_SOURCES}
)

# Include directories
target_include_directories(KotonoReflection
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_reflection"
)

# Link libs
target_link_libraries(KotonoReflection PUBLIC
    KotonoIO
)