cmake_minimum_required(VERSION 4.2)
project(KotonoGenerator LANGUAGES C CXX)

add_executable(KotonoGenerator)

target_compile_features(KotonoGenerator PRIVATE cxx_std_23)

set_target_properties(KotonoGenerator PROPERTIES FOLDER "KotonoEngine")

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

# Include directories
target_include_directories(KotonoGenerator
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_generator"
)

# Link libs
target_link_libraries(KotonoGenerator PRIVATE KotonoReflection)
