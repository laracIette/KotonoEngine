cmake_minimum_required(VERSION 4.2)
project(KotonoRendering LANGUAGES C CXX)

add_library(KotonoRendering STATIC)

target_compile_features(KotonoRendering PUBLIC cxx_std_23)

set_target_properties(KotonoRendering PROPERTIES FOLDER "KotonoEngine/Core")

set(KRENDERING_PUBLIC_HEADERS
    "Public/DrawCommand.h"
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

target_link_libraries(KotonoRendering PUBLIC
    KotonoGraphics
    KotonoPlatform
)
