cmake_minimum_required(VERSION 4.2)
project(KotonoGraphics LANGUAGES C CXX)

add_library(KotonoGraphics STATIC)

target_compile_features(KotonoGraphics PUBLIC cxx_std_23)

set_target_properties(KotonoGraphics PROPERTIES FOLDER "KotonoEngine")

set(KGRAPHICS_HEADERS
    "include/kotono_graphics/Color.h"
    "include/kotono_graphics/DirectionalLightData.h"
    "include/kotono_graphics/DrawData.h"
    "include/kotono_graphics/Font.h"
    "include/kotono_graphics/InterfaceRenderGraph.h"
    "include/kotono_graphics/Mobility.h"
    "include/kotono_graphics/PointLightData.h"
    "include/kotono_graphics/SceneRenderGraph.h"
    "include/kotono_graphics/SceneView.h"
    "include/kotono_graphics/Scissor.h"
    "include/kotono_graphics/SpvCompiler.h"
    "include/kotono_graphics/Visibility.h"
)
set(KGRAPHICS_SOURCES
    "src/Font.cpp"
    "src/SpvCompiler.cpp"
)

target_sources(KotonoGraphics
    PRIVATE ${KGRAPHICS_HEADERS}
    PRIVATE ${KGRAPHICS_SOURCES}
)

# Include directories
target_include_directories(KotonoGraphics
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_graphics"
)

# Shader folder included for IDE visibility (not compiled)
file(GLOB_RECURSE KGRAPHICS_SHADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")
set_source_files_properties(${KGRAPHICS_SHADERS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/shaders" PREFIX "Shaders" FILES ${KGRAPHICS_SHADERS})
target_sources(KotonoGraphics PRIVATE ${KGRAPHICS_SHADERS})

file(GLOB_RECURSE KGRAPHICS_ASSETS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
set_source_files_properties(${KGRAPHICS_ASSETS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/assets" PREFIX "Assets" FILES ${KGRAPHICS_ASSETS})
target_sources(KotonoGraphics PRIVATE ${KGRAPHICS_ASSETS})

# Link libs
target_link_libraries(KotonoGraphics PUBLIC
    KotonoIO
)
