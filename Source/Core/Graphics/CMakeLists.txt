cmake_minimum_required(VERSION 4.2)
project(KotonoGraphics LANGUAGES C CXX)

add_library(KotonoGraphics STATIC)

target_compile_features(KotonoGraphics PUBLIC cxx_std_23)

set_target_properties(KotonoGraphics PROPERTIES FOLDER "KotonoEngine/Core")

set(KGRAPHICS_HEADERS
    "Color.h"
    "Mobility.h"
    "Padding.h"
    "SceneView.h"
    "SceneVisibility.h"
    "Scissor.h"
    "Visibility.h"
    "Font/Font.h"
    "RenderGraph/DirectionalLightData.h"
    "RenderGraph/InterfaceDrawData.h"
    "RenderGraph/InterfaceRenderGraph.h"
    "RenderGraph/PointLightData.h"
    "RenderGraph/SceneDrawData.h"
    "RenderGraph/SceneRenderGraph.h"
    "SpvCompiler/SpvCompiler.h"
)
set(KGRAPHICS_SOURCES
    "Font/Font.cpp"
    "SpvCompiler/SpvCompiler.cpp"
)

target_sources(KotonoGraphics
    PRIVATE ${KGRAPHICS_HEADERS}
    PRIVATE ${KGRAPHICS_SOURCES}
)

target_include_directories(KotonoGraphics
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
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
