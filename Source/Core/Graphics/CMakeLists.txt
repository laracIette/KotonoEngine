cmake_minimum_required(VERSION 4.2)
project(KotonoGraphics LANGUAGES C CXX)

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
    "Public/Vertex.h"
    "Public/Visibility.h"
    "Public/Font/Font.h"
    "Public/MeshLoader/MeshData.h"
    "Public/MeshLoader/MeshLoader.h"
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
    "Private/MeshLoader/MeshLoader.cpp"
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

target_compile_definitions(KotonoGraphics PUBLIC
    ASSIMP_BUILD_NO_EXPORT
)

target_link_libraries(KotonoGraphics PRIVATE
    assimp::assimp
)

target_link_libraries(KotonoGraphics PUBLIC
    KotonoMath
    KotonoIO
)
