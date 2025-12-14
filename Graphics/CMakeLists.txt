cmake_minimum_required(VERSION 3.20)
project(KotonoGraphics LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

include(FetchContent)

################
# --- Assimp ---
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v6.0.2
    GIT_SHALLOW    TRUE
)

# Force Assimp to build as a static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Build zlib
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

# Disable unnecessary Assimp extras to speed up build times
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)
################

# Static library
add_library(KotonoGraphics STATIC)

# Require C++20
target_compile_features(KotonoGraphics PUBLIC cxx_std_20)

set(KGRAPHICS_HEADERS
    "include/kotono_graphics/Color.h"
    "include/kotono_graphics/Font.h"
    "include/kotono_graphics/frames_in_flight.h"
    "include/kotono_graphics/ImageTexture.h"
    "include/kotono_graphics/ImageTextureManager.h"
    "include/kotono_graphics/InterfaceCuller.h"
    "include/kotono_graphics/InterfaceObjectData.h"
    "include/kotono_graphics/InterfaceRenderable.h"
    "include/kotono_graphics/InterfaceRenderableProxy.h"
    "include/kotono_graphics/InterfaceRenderer.h"
    "include/kotono_graphics/InterfaceUniformData.h"
    "include/kotono_graphics/Model.h"
    "include/kotono_graphics/ModelManager.h"
    "include/kotono_graphics/Renderer.h"
    "include/kotono_graphics/RendererFrameStats.h"
    "include/kotono_graphics/SceneCuller.h"
    "include/kotono_graphics/SceneCullerField.h"
    "include/kotono_graphics/SceneObjectData.h"
    "include/kotono_graphics/SceneRenderable.h"
    "include/kotono_graphics/SceneRenderableProxy.h"
    "include/kotono_graphics/SceneRenderer.h"
    "include/kotono_graphics/SceneUniformData.h"
    "include/kotono_graphics/Scissor.h"
    "include/kotono_graphics/Shader.h"
    "include/kotono_graphics/ShaderLayout.h"
    "include/kotono_graphics/ShaderManager.h"
    "include/kotono_graphics/SpvCompiler.h"
    "include/kotono_graphics/Vertex2D.h"
    "include/kotono_graphics/Vertex3D.h"
)
set(KGRAPHICS_SOURCES
    "src/Font.cpp"
    "src/ImageTexture.cpp"
    "src/ImageTextureManager.cpp"
    "src/implementations.cpp"
    "src/InterfaceCuller.cpp"
    "src/InterfaceRenderer.cpp"
    "src/Model.cpp"
    "src/ModelManager.cpp"
    "src/Renderer.cpp"
    "src/SceneCuller.cpp"
    "src/SceneRenderer.cpp"
    "src/Shader.cpp"
    "src/ShaderManager.cpp"
    "src/SpvCompiler.cpp"
)

target_sources(KotonoGraphics
    PRIVATE ${KGRAPHICS_HEADERS}
    PRIVATE ${KGRAPHICS_SOURCES}
)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

# Include directories
target_include_directories(KotonoGraphics
    PUBLIC  "${THIRD_PARTY_DIR}/stbimage/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_graphics"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoGraphics PUBLIC
    ASSIMP_BUILD_NO_EXPORT
)

# Link system + third-party libs
target_link_libraries(KotonoGraphics PUBLIC
    SPIRVReflect
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
