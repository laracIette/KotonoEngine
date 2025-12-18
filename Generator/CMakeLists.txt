cmake_minimum_required(VERSION 3.20)
project(KotonoGenerator LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Create executable
add_executable(KotonoGenerator)

# Enable C++20
target_compile_features(KotonoGenerator PRIVATE cxx_std_20)

# Collect sources
set(KGENERATOR_HEADERS
    "include/kotono_generator/Generator.h"
)
set(KGENERATOR_SOURCES
    "src/Generator.cpp"
    "src/main.cpp"
)

target_sources(KotonoGenerator
    PRIVATE ${KGENERATOR_HEADERS}
    PRIVATE ${KGENERATOR_SOURCES}
)

set(THIRD_PARTY_DIR "${ENGINE_PATH}/ThirdParty")

# Include directories
target_include_directories(KotonoGenerator
    PRIVATE "${THIRD_PARTY_DIR}/nlohmann/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_generator"
)

# Link libs
target_link_libraries(KotonoGenerator PRIVATE KotonoCommon)
