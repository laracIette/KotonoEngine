cmake_minimum_required(VERSION 4.2)
project(KotonoGraphics LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
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

# Disable the install targets
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

# Force Assimp to build as a static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Build zlib
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

# Disable unnecessary Assimp extras to speed up build times
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)

# Disable all importers by default
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

# Enable importers
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

# Disable exporters
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

foreach(assimp_target
    assimp
    UpdateAssimpLibsDebugSymbolsAndDLLs
    zlibstatic
)
    if(TARGET ${assimp_target})
        set_target_properties(${assimp_target} PROPERTIES FOLDER "KotonoEngine/ThirdParty/assimp")
    endif()
endforeach()
################

if(TARGET SPIRVReflect)
    set_target_properties(SPIRVReflect PROPERTIES FOLDER "KotonoEngine/ThirdParty")
endif()

# Static library
add_library(KotonoGraphics STATIC)

# Require C++20
target_compile_features(KotonoGraphics PUBLIC cxx_std_23)

set_target_properties(KotonoGraphics PROPERTIES FOLDER "KotonoEngine")

set(KGRAPHICS_HEADERS
    "include/kotono_graphics/Color.h"
    "include/kotono_graphics/Font.h"
    "include/kotono_graphics/frames_in_flight.h"
    "include/kotono_graphics/InterfaceCuller.h"
    "include/kotono_graphics/InterfaceObjectData.h"
    "include/kotono_graphics/InterfaceProxy.h"
    "include/kotono_graphics/InterfaceRenderable.h"
    "include/kotono_graphics/InterfaceRenderer.h"
    "include/kotono_graphics/InterfaceUniformData.h"
    "include/kotono_graphics/Mobility.h"
    "include/kotono_graphics/Model.h"
    "include/kotono_graphics/Renderer.h"
    "include/kotono_graphics/RendererFrameStats.h"
    "include/kotono_graphics/SceneCuller.h"
    "include/kotono_graphics/SceneCullerField.h"
    "include/kotono_graphics/SceneObjectData.h"
    "include/kotono_graphics/SceneProxy.h"
    "include/kotono_graphics/SceneRenderable.h"
    "include/kotono_graphics/SceneRenderer.h"
    "include/kotono_graphics/SceneUniformData.h"
    "include/kotono_graphics/Scissor.h"
    "include/kotono_graphics/Shader.h"
    "include/kotono_graphics/ShaderLayout.h"
    "include/kotono_graphics/SpvCompiler.h"
    "include/kotono_graphics/Texture.h"
    "include/kotono_graphics/Vertex2D.h"
    "include/kotono_graphics/Vertex3D.h"
    "include/kotono_graphics/Visibility.h"
)
set(KGRAPHICS_SOURCES
    "src/Font.cpp"
    "src/implementations.cpp"
    "src/InterfaceCuller.cpp"
    "src/InterfaceProxy.cpp"
    "src/InterfaceRenderer.cpp"
    "src/Model.cpp"
    "src/Renderer.cpp"
    "src/SceneCuller.cpp"
    "src/SceneProxy.cpp"
    "src/SceneRenderer.cpp"
    "src/Shader.cpp"
    "src/SpvCompiler.cpp"
    "src/Texture.cpp"
    "src/Vertex2D.cpp"
    "src/Vertex3D.cpp"

)

target_sources(KotonoGraphics
    PRIVATE ${KGRAPHICS_HEADERS}
    PRIVATE ${KGRAPHICS_SOURCES}
)

set(THIRD_PARTY_DIR "${ENGINE_PATH}/ThirdParty")

# Include directories
target_include_directories(KotonoGraphics
    PRIVATE  "${THIRD_PARTY_DIR}/stbimage/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_graphics"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoGraphics PUBLIC
    ASSIMP_BUILD_NO_EXPORT
)

# Link system + third-party libs
target_link_libraries(KotonoGraphics PRIVATE
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
