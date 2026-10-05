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
    PRIVATE
        ${KSCENE_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KSCENE_PUBLIC_HEADERS}
)

target_include_directories(KotonoScene
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoScene PUBLIC
    KotonoObject
)
