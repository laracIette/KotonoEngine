cmake_minimum_required(VERSION 3.20)
project(KotonoGenerator LANGUAGES CXX)

if(MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)

    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

file(GLOB_RECURSE KGENERATOR_SOURCES "src/*.cpp")
file(GLOB_RECURSE KGENERATOR_HEADERS "include/kotono_generator/*.h")

add_executable(KotonoGenerator ${KGENERATOR_SOURCES})

# Require C++20
target_compile_features(KotonoGenerator PRIVATE cxx_std_20)

target_sources(KotonoGenerator 
    PRIVATE ${KGENERATOR_SOURCES}
    PRIVATE ${KGENERATOR_HEADERS} 
)

# Include directories
target_include_directories(KotonoGenerator 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_generator"
)

# Output .lib to KotonoGenerator/lib/
set_target_properties(KotonoGenerator PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Library search folder
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")