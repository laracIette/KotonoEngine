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

set_target_properties(KotonoRendering PROPERTIES FOLDER "KotonoEngine/Core")

set(KRENDERING_HEADERS
    "DirectionalLight.h"
    "DrawCommand.h"
    "DrawDataBufferData.h"
    "FrameContextBufferData.h"
    "frames_in_flight.h"
    "MaterialBufferData.h"
    "ParametersBufferData.h"
    "PointLight.h"
    "PushConstants.h"
    "TransformBufferData.h"
    "Barriers/Barriers.h"
    "FrameContextBuffer/FrameContextBuffer.h"
    "GPUBuffers/GPUBuffers.h"
    "IndexBuffer/IndexBuffer.h"
    "InterfaceRender/InterfaceRender.h"
    "InterfaceRenderer/InterfaceRenderer.h"
    "LightBuffers/LightBuffers.h"
    "Material/Material.h"
    "Model/Model.h"
    "PipelineResourceManager/PipelineResourceManager.h"
    "Renderer/Renderer.h"
    "Sampler/Sampler.h"
    "SceneRender/SceneRender.h"
    "SceneRenderer/SceneRenderer.h"
    "Shader/Shader.h"
    "Texture/Texture.h"
)
set(KRENDERING_SOURCES
    "implementations.cpp"
    "Barriers/Barriers.cpp"
    "FrameContextBuffer/FrameContextBuffer.cpp"
    "GPUBuffers/GPUBuffers.cpp"
    "IndexBuffer/IndexBuffer.cpp"
    "InterfaceRender/InterfaceRender.cpp"
    "InterfaceRenderer/InterfaceRenderer.cpp"
    "LightBuffers/LightBuffers.cpp"
    "Material/Material.cpp"
    "Model/Model.cpp"
    "PipelineResourceManager/PipelineResourceManager.cpp"
    "Renderer/Renderer.cpp"
    "Sampler/Sampler.cpp"
    "SceneRender/SceneRender.cpp"
    "SceneRenderer/SceneRenderer.cpp"
    "Shader/Shader.cpp"
    "Texture/Texture.cpp"
)

target_sources(KotonoRendering
    PRIVATE ${KRENDERING_HEADERS}
    PRIVATE ${KRENDERING_SOURCES}
)

target_include_directories(KotonoRendering
    PRIVATE "${ENGINE_PATH}/Source/ThirdParty/stbimage/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoRendering PUBLIC
    ASSIMP_BUILD_NO_EXPORT
)

target_link_libraries(KotonoRendering PRIVATE
    assimp::assimp
)

target_link_libraries(KotonoRendering PUBLIC
    KotonoGraphics
    KotonoPlatform
)
