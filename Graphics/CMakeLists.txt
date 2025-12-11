cmake_minimum_required(VERSION 3.20)
project(KotonoGraphics LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# --- Add Assimp via FetchContent ---
include(FetchContent)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v6.0.2
)

# Force Assimp to build as a STATIC library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Disable unnecessary Assimp extras to speed up build times
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)

# Download and add Assimp to the build
FetchContent_MakeAvailable(assimp)

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
    assimp::assimp
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
