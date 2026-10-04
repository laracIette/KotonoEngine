cmake_minimum_required(VERSION 4.2)
project(KotonoGenerator LANGUAGES C CXX)

add_executable(KotonoGenerator)

target_compile_features(KotonoGenerator PRIVATE cxx_std_23)

set_target_properties(KotonoGenerator PROPERTIES FOLDER "KotonoEngine")

# Collect sources
set(KGENERATOR_HEADERS
    "Generator/Generator.h"
)
set(KGENERATOR_SOURCES
    "Generator/Generator.cpp"
    "main.cpp"
)

target_sources(KotonoGenerator
    PRIVATE ${KGENERATOR_HEADERS}
    PRIVATE ${KGENERATOR_SOURCES}
)

target_include_directories(KotonoGenerator
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoGenerator PRIVATE KotonoReflection)
