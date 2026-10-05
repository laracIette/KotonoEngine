cmake_minimum_required(VERSION 4.2)
project(KotonoGenerator LANGUAGES C CXX)

add_executable(KotonoGenerator)

target_compile_features(KotonoGenerator PRIVATE cxx_std_23)

set_target_properties(KotonoGenerator PROPERTIES FOLDER "KotonoEngine")

set(KGENERATOR_PRIVATE_HEADERS
    "Private/Generator/Generator.h"
)
set(KGENERATOR_SOURCES
    "Private/Generator/Generator.cpp"
    "Private/main.cpp"
)

target_sources(KotonoGenerator
    PRIVATE
        ${KGENERATOR_SOURCES}
        ${KGENERATOR_PRIVATE_HEADERS}
)

target_include_directories(KotonoGenerator
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoGenerator PRIVATE KotonoReflection)
