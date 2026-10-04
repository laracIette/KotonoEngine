cmake_minimum_required(VERSION 4.2)
project(KotonoRendering LANGUAGES C CXX)

################
include(FetchContent)

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

add_library(KotonoRendering STATIC)

target_compile_features(KotonoRendering PUBLIC cxx_std_23)

set_target_properties(KotonoRendering PROPERTIES FOLDER "KotonoEngine")

set(KRENDERING_HEADERS
    "include/kotono_rendering/Barriers.h"
    "include/kotono_rendering/DirectionalLight.h"
    "include/kotono_rendering/DrawCommand.h"
    "include/kotono_rendering/DrawDataBufferData.h"
    "include/kotono_rendering/FrameContextBuffer.h"
    "include/kotono_rendering/FrameContextBufferData.h"
    "include/kotono_rendering/frames_in_flight.h"
    "include/kotono_rendering/GPUBuffers.h"
    "include/kotono_rendering/IndexBuffer.h"
    "include/kotono_rendering/InterfaceRender.h"
    "include/kotono_rendering/InterfaceRenderer.h"
    "include/kotono_rendering/LightBuffers.h"
    "include/kotono_rendering/Material.h"
    "include/kotono_rendering/MaterialBufferData.h"
    "include/kotono_rendering/Model.h"
    "include/kotono_rendering/ParametersBufferData.h"
    "include/kotono_rendering/PipelineResourceManager.h"
    "include/kotono_rendering/PointLight.h"
    "include/kotono_rendering/PushConstants.h"
    "include/kotono_rendering/Renderer.h"
    "include/kotono_rendering/Sampler.h"
    "include/kotono_rendering/SceneRender.h"
    "include/kotono_rendering/SceneRenderer.h"
    "include/kotono_rendering/Shader.h"
    "include/kotono_rendering/Texture.h"
    "include/kotono_rendering/TransformBufferData.h"
)
set(KRENDERING_SOURCES
    "src/Barriers.cpp"
    "src/FrameContextBuffer.cpp"
    "src/GPUBuffers.cpp"
    "src/implementations.cpp"
    "src/IndexBuffer.cpp"
    "src/InterfaceRender.cpp"
    "src/InterfaceRenderer.cpp"    
    "src/LightBuffers.cpp"
    "src/Material.cpp"
    "src/Model.cpp"
    "src/PipelineResourceManager.cpp"
    "src/Renderer.cpp"
    "src/Sampler.cpp"
    "src/SceneRender.cpp"
    "src/SceneRenderer.cpp"
    "src/Shader.cpp"
    "src/Texture.cpp"
)

target_sources(KotonoRendering
    PRIVATE ${KRENDERING_HEADERS}
    PRIVATE ${KRENDERING_SOURCES}
)

set(THIRD_PARTY_DIR "${ENGINE_PATH}/ThirdParty")

# Include directories
target_include_directories(KotonoRendering    
    PRIVATE "${THIRD_PARTY_DIR}/stbimage/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_rendering"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoRendering PUBLIC
    ASSIMP_BUILD_NO_EXPORT
)

# Link system + third-party libs
target_link_libraries(KotonoRendering PRIVATE
    assimp::assimp
)

# Link libs
target_link_libraries(KotonoRendering PUBLIC
    KotonoGraphics
    KotonoPlatform
)
