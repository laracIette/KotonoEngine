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
    PRIVATE
        ${KGRAPHICS_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KGRAPHICS_PUBLIC_HEADERS}
)

target_include_directories(KotonoGraphics
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoGraphics PUBLIC
    KotonoMath
    KotonoIO
)
