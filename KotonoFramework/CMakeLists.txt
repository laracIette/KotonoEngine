cmake_minimum_required(VERSION 3.20)
project(KotonoFramework LANGUAGES CXX)

if(MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)

    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoFramework STATIC)

# Require C++20
target_compile_features(KotonoFramework PUBLIC cxx_std_20)

file(GLOB_RECURSE KFRAMEWORK_SOURCES "src/*.cpp")
file(GLOB_RECURSE KFRAMEWORK_HEADERS "include/kotono_framework/*.h")

target_sources(KotonoFramework
    PRIVATE ${KFRAMEWORK_SOURCES}
    PRIVATE ${KFRAMEWORK_HEADERS} 
)

# Include directories
target_include_directories(KotonoFramework PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_framework"
)

# Output .lib to KotonoFramework/lib/
set_target_properties(KotonoFramework PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoFramework PUBLIC
    NOMINMAX
    EDITOR
)

# Library search folder
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Link system + third-party libs
target_link_libraries(KotonoFramework PUBLIC
    "${LIB_DIR}/vulkan-1.lib"
    "${LIB_DIR}/glfw3.lib"
    "${LIB_DIR}/OpenAL32.lib"
    "${LIB_DIR}/SPIRV-Reflect.lib"

    $<$<CONFIG:Debug>:${LIB_DIR}/zlibstaticd.lib>
    $<$<CONFIG:Release>:${LIB_DIR}/zlibstatic.lib>
    $<$<CONFIG:Debug>:${LIB_DIR}/assimp-vc143-mtd.lib>
    $<$<CONFIG:Release>:${LIB_DIR}/assimp-vc143-mt.lib>
)

# Shader folder included for IDE visibility (not compiled)
file(GLOB_RECURSE KFRAMEWORK_SHADERS "shaders/*")
source_group("Shaders" FILES ${KFRAMEWORK_SHADERS})
target_sources(KotonoFramework PRIVATE ${KFRAMEWORK_SHADERS})