cmake_minimum_required(VERSION 4.2)
project(KotonoScene LANGUAGES C CXX)

add_library(KotonoScene STATIC)

target_compile_features(KotonoScene PUBLIC cxx_std_23)

set_target_properties(KotonoScene PROPERTIES FOLDER "KotonoEngine/Core")

set(KSCENE_PUBLIC_HEADERS
    "Public/AudioComponent/AudioComponent.h"
    "Public/CameraComponent/CameraComponent.h"
    "Public/DirectionalLightComponent/DirectionalLightComponent.h"
    "Public/PointLightComponent/PointLightComponent.h"
    "Public/MeshComponent/MeshComponent.h"
)
set(KSCENE_SOURCES
    "Private/AudioComponent/AudioComponent.cpp"
    "Private/CameraComponent/CameraComponent.cpp"
    "Private/DirectionalLightComponent/DirectionalLightComponent.cpp"
    "Private/PointLightComponent/PointLightComponent.cpp"
    "Private/MeshComponent/MeshComponent.cpp"
)

target_sources(KotonoScene
    PUBLIC ${KSCENE_PUBLIC_HEADERS}
    PRIVATE ${KSCENE_SOURCES}
)

target_include_directories(KotonoScene
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoScene PUBLIC
    KotonoObject
)
