cmake_minimum_required(VERSION 3.20)
project(KotonoEngine LANGUAGES CXX)

if(MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)

    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoEngine STATIC)

# Require C++20
target_compile_features(KotonoEngine PUBLIC cxx_std_20)

file(GLOB_RECURSE KENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE KENGINE_HEADERS "include/kotono_engine/*.h")

target_sources(KotonoEngine
    PRIVATE ${KENGINE_SOURCES}
    PRIVATE ${KENGINE_HEADERS} 
)

# Include directories
target_include_directories(KotonoEngine 
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_engine"
)

# Output .lib to KotonoEngine/lib/
set_target_properties(KotonoEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Link libs
target_link_libraries(KotonoEngine PUBLIC 
    KotonoFramework
)