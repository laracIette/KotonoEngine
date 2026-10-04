cmake_minimum_required(VERSION 4.2)
project(KotonoScene LANGUAGES C CXX)

add_library(KotonoScene STATIC)

target_compile_features(KotonoScene PUBLIC cxx_std_23)

set_target_properties(KotonoScene PROPERTIES FOLDER "KotonoEngine/Core")

set(KSCENE_HEADERS
    "AudioComponent/AudioComponent.h"
    "CameraComponent/CameraComponent.h"
    "DirectionalLightComponent/DirectionalLightComponent.h"
    "PointLightComponent/PointLightComponent.h"
    "MeshComponent/MeshComponent.h"
)
set(KSCENE_SOURCES
    "AudioComponent/AudioComponent.cpp"
    "CameraComponent/CameraComponent.cpp"
    "DirectionalLightComponent/DirectionalLightComponent.cpp"
    "PointLightComponent/PointLightComponent.cpp"
    "MeshComponent/MeshComponent.cpp"
)

target_sources(KotonoScene
    PRIVATE ${KSCENE_HEADERS}
    PRIVATE ${KSCENE_SOURCES}
)

target_include_directories(KotonoScene
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoScene PUBLIC
    KotonoObject
)
