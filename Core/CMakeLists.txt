cmake_minimum_required(VERSION 3.20)
project(KotonoCore LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoCore STATIC)

# Require C++20
target_compile_features(KotonoCore PUBLIC cxx_std_20)

file(GLOB KCORE_HEADERS "include/kotono_core/*.h")
file(GLOB KCORE_SOURCES "src/*.cpp")

file(GLOB KCORE_GENERATED_HEADERS "include/kotono_core/generated/*.generated.h")
file(GLOB KCORE_GENERATED_SOURCES "src/generated/*.generated.cpp")

target_sources(KotonoCore
    PRIVATE ${KCORE_HEADERS}
    PRIVATE ${KCORE_SOURCES}
    PRIVATE ${KCORE_GENERATED_HEADERS}
    PRIVATE ${KCORE_GENERATED_SOURCES}
)

# Main groups
source_group("Header Files" FILES ${KCORE_HEADERS})
source_group("Source Files" FILES ${KCORE_SOURCES})

# Generated groups
source_group("Header Files\\Generated" FILES ${KCORE_GENERATED_HEADERS})
source_group("Source Files\\Generated" FILES ${KCORE_GENERATED_SOURCES})

# Include directories
target_include_directories(KotonoCore
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_core"
)

# Link libs
target_link_libraries(KotonoCore PUBLIC
    KotonoAudio
    KotonoInput
    KotonoTiming
    KotonoGraphics
)

add_custom_target(RunKotonoGenerator
    COMMAND $<TARGET_FILE:KotonoGenerator>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:KotonoGenerator>
    DEPENDS KotonoGenerator
    COMMENT "Running KotonoGenerator..."
)

add_dependencies(KotonoCore RunKotonoGenerator)
