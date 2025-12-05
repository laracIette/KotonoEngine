cmake_minimum_required(VERSION 3.20)
project(KotonoEngine LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoEngine STATIC)

# Require C++20
target_compile_features(KotonoEngine PUBLIC cxx_std_20)

file(GLOB KENGINE_HEADERS "include/kotono_engine/*.h")
file(GLOB KENGINE_SOURCES "src/*.cpp")

file(GLOB KENGINE_GENERATED_HEADERS "include/kotono_engine/generated/*.generated.h")
file(GLOB KENGINE_GENERATED_SOURCES "src/generated/*.generated.cpp")

target_sources(KotonoEngine
    PRIVATE ${KENGINE_HEADERS}
    PRIVATE ${KENGINE_SOURCES}
    PRIVATE ${KENGINE_GENERATED_HEADERS}
    PRIVATE ${KENGINE_GENERATED_SOURCES}
)

# Main groups
source_group("Header Files" FILES ${KENGINE_HEADERS})
source_group("Source Files" FILES ${KENGINE_SOURCES})

# Generated groups
source_group("Header Files\\Generated" FILES ${KENGINE_GENERATED_HEADERS})
source_group("Source Files\\Generated" FILES ${KENGINE_GENERATED_SOURCES})

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
    KotonoInput
)

add_custom_target(RunKotonoGenerator
    COMMAND $<TARGET_FILE:KotonoGenerator>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:KotonoGenerator>
    DEPENDS KotonoGenerator
    COMMENT "Running KotonoGenerator..."
)

add_dependencies(KotonoEngine RunKotonoGenerator)
