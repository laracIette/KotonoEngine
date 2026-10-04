cmake_minimum_required(VERSION 4.2)
project(KotonoGraphics LANGUAGES C CXX)

add_library(KotonoGraphics STATIC)

target_compile_features(KotonoGraphics PUBLIC cxx_std_23)

set_target_properties(KotonoGraphics PROPERTIES FOLDER "KotonoEngine/Core")

set(KGRAPHICS_PUBLIC_HEADERS
    "Public/Color.h"
    "Public/Mobility.h"
    "Public/Padding.h"
    "Public/SceneView.h"
    "Public/SceneVisibility.h"
    "Public/Scissor.h"
    "Public/Visibility.h"
    "Public/Font/Font.h"
    "Public/RenderGraph/DirectionalLightData.h"
    "Public/RenderGraph/InterfaceDrawData.h"
    "Public/RenderGraph/InterfaceRenderGraph.h"
    "Public/RenderGraph/PointLightData.h"
    "Public/RenderGraph/SceneDrawData.h"
    "Public/RenderGraph/SceneRenderGraph.h"
    "Public/SpvCompiler/SpvCompiler.h"
)
set(KGRAPHICS_SOURCES
    "Private/Font/Font.cpp"
    "Private/SpvCompiler/SpvCompiler.cpp"
)

target_sources(KotonoGraphics
    PUBLIC ${KGRAPHICS_PUBLIC_HEADERS}
    PRIVATE ${KGRAPHICS_SOURCES}
)

target_include_directories(KotonoGraphics
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

# Shader folder included for IDE visibility (not compiled)
file(GLOB_RECURSE KGRAPHICS_SHADERS CONFIGURE_DEPENDS "${ENGINE_PATH}/Shaders/*")
set_source_files_properties(${KGRAPHICS_SHADERS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(TREE "${ENGINE_PATH}/Shaders" PREFIX "Shaders" FILES ${KGRAPHICS_SHADERS})
target_sources(KotonoGraphics PRIVATE ${KGRAPHICS_SHADERS})

file(GLOB_RECURSE KGRAPHICS_ASSETS CONFIGURE_DEPENDS "${ENGINE_PATH}/Assets/*")
set_source_files_properties(${KGRAPHICS_ASSETS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(TREE "${ENGINE_PATH}/Assets" PREFIX "Assets" FILES ${KGRAPHICS_ASSETS})
target_sources(KotonoGraphics PRIVATE ${KGRAPHICS_ASSETS})

target_link_libraries(KotonoGraphics PUBLIC
    KotonoIO
)
