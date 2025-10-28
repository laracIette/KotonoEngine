cmake_minimum_required(VERSION 3.20)
project(KotonoEngineInterface LANGUAGES CXX)

if(MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)

    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoEngineInterface STATIC)

# Require C++20
target_compile_features(KotonoEngineInterface PUBLIC cxx_std_20)

file(GLOB_RECURSE KENGINEINTERFACE_SOURCES "src/*.cpp")
file(GLOB_RECURSE KENGINEINTERFACE_HEADERS "include/kotono_engine_interface/*.h")

target_sources(KotonoEngineInterface
    PRIVATE ${KENGINEINTERFACE_SOURCES}
    PRIVATE ${KENGINEINTERFACE_HEADERS} 
)

# Include directories
target_include_directories(KotonoEngineInterface PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_engine_interface"

    # Include KotonoFramework
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoFramework/include"
    # Include KotonoEngine
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoEngine/include"
    # Include KotonoInterface
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoInterface/include"
)

# Output .lib to KotonoEngineInterface/lib/
set_target_properties(KotonoEngineInterface PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Preprocessor defines
target_compile_definitions(KotonoEngineInterface PUBLIC
    NOMINMAX
    EDITOR
)

# Link libs
target_link_libraries(KotonoEngineInterface PUBLIC 
    KotonoEngine
    KotonoInterface
)