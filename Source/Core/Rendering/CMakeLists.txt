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

set(KRENDERING_PUBLIC_HEADERS
    "Public/DrawDataBufferData.h"
    "Public/FrameContextBufferData.h"
    "Public/frames_in_flight.h"
    "Public/MaterialBufferData.h"
    "Public/ParametersBufferData.h"
    "Public/PushConstants.h"
    "Public/TransformBufferData.h"
    "Public/FrameContextBuffer/FrameContextBuffer.h"
    "Public/GPUBuffers/GPUBuffers.h"
    "Public/IndexBuffer/IndexBuffer.h"
    "Public/InterfaceRender/InterfaceRender.h"
    "Public/InterfaceRenderer/InterfaceRenderer.h"
    "Public/LightBuffers/LightBuffers.h"
    "Public/PipelineResourceManager/PipelineResourceManager.h"
    "Public/Renderer/Renderer.h"
    "Public/SceneRender/SceneRender.h"
    "Public/SceneRenderer/SceneRenderer.h"
)
set(KRENDERING_PRIVATE_HEADERS
    "Private/DirectionalLight.h"
    "Private/DrawCommand.h"
    "Private/PointLight.h"
    "Private/Barriers/Barriers.h"
    "Private/Material/Material.h"
    "Private/Model/Model.h"
    "Private/Sampler/Sampler.h"
    "Private/Shader/Shader.h"
    "Private/Texture/Texture.h"
)
set(KRENDERING_SOURCES
    "Private/implementations.cpp"
    "Private/Barriers/Barriers.cpp"
    "Private/FrameContextBuffer/FrameContextBuffer.cpp"
    "Private/GPUBuffers/GPUBuffers.cpp"
    "Private/IndexBuffer/IndexBuffer.cpp"
    "Private/InterfaceRender/InterfaceRender.cpp"
    "Private/InterfaceRenderer/InterfaceRenderer.cpp"
    "Private/LightBuffers/LightBuffers.cpp"
    "Private/Material/Material.cpp"
    "Private/Model/Model.cpp"
    "Private/PipelineResourceManager/PipelineResourceManager.cpp"
    "Private/Renderer/Renderer.cpp"
    "Private/Sampler/Sampler.cpp"
    "Private/SceneRender/SceneRender.cpp"
    "Private/SceneRenderer/SceneRenderer.cpp"
    "Private/Shader/Shader.cpp"
    "Private/Texture/Texture.cpp"
)

target_sources(KotonoRendering
    PRIVATE
        ${KRENDERING_SOURCES}
        ${KRENDERING_PRIVATE_HEADERS}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KRENDERING_PUBLIC_HEADERS}
)

target_include_directories(KotonoRendering
    PRIVATE "${ENGINE_PATH}/Source/ThirdParty/stbimage/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
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
