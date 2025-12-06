cmake_minimum_required(VERSION 3.20)
project(KotonoGraphics LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoGraphics STATIC)

# Require C++20
target_compile_features(KotonoGraphics PUBLIC cxx_std_20)

file(GLOB_RECURSE KGRAPHICS_SOURCES "src/*.cpp")
file(GLOB_RECURSE KGRAPHICS_HEADERS "include/kotono_graphics/*.h")

target_sources(KotonoGraphics
    PRIVATE ${KGRAPHICS_SOURCES}
    PRIVATE ${KGRAPHICS_HEADERS}
)

# Include directories
target_include_directories(KotonoGraphics
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_graphics"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoGraphics PUBLIC
    ASSIMP_BUILD_NO_EXPORT
)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

# Link system + third-party libs
target_link_libraries(KotonoGraphics PUBLIC
    "${THIRD_PARTY_DIR}/lib/SPIRV-Reflect.lib"

    $<$<CONFIG:Debug>:${THIRD_PARTY_DIR}/lib/zlibstaticd.lib>
    $<$<CONFIG:Release>:${THIRD_PARTY_DIR}/lib/zlibstatic.lib>
    $<$<CONFIG:Debug>:${THIRD_PARTY_DIR}/lib/assimp-vc143-mtd.lib>
    $<$<CONFIG:Release>:${THIRD_PARTY_DIR}/lib/assimp-vc143-mt.lib>
)

# Shader folder included for IDE visibility (not compiled)
file(GLOB_RECURSE KGRAPHICS_SHADERS "shaders/*")
source_group("Shaders" FILES ${KGRAPHICS_SHADERS})
target_sources(KotonoGraphics PRIVATE ${KGRAPHICS_SHADERS})

# Link libs
target_link_libraries(KotonoGraphics PUBLIC
    KotonoIO
    KotonoPlatform
)
