cmake_minimum_required(VERSION 4.2)
project(KotonoScene LANGUAGES C CXX)

add_library(KotonoScene STATIC)

target_compile_features(KotonoScene PUBLIC cxx_std_23)

set_target_properties(KotonoScene PROPERTIES FOLDER "KotonoEngine")

set(KSCENE_HEADERS
    "include/kotono_scene/CameraComponent.h"
    "include/kotono_scene/DirectionalLightComponent.h"
    "include/kotono_scene/PointLightComponent.h"
    "include/kotono_scene/MeshComponent.h"
)
set(KSCENE_SOURCES
    "src/CameraComponent.cpp"
    "src/DirectionalLightComponent.cpp"
    "src/PointLightComponent.cpp"
    "src/MeshComponent.cpp"
)

target_sources(KotonoScene
    PRIVATE ${KSCENE_HEADERS}
    PRIVATE ${KSCENE_SOURCES}
)

# Include directories
target_include_directories(KotonoScene
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_scene"
)

# Link libs
target_link_libraries(KotonoScene PUBLIC
    KotonoCore
)
