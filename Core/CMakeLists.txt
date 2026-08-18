cmake_minimum_required(VERSION 4.2)
project(KotonoCore LANGUAGES C CXX)

add_library(KotonoCore STATIC)

target_compile_features(KotonoCore PUBLIC cxx_std_23)

set_target_properties(KotonoCore PROPERTIES FOLDER "KotonoEngine")

set(KCORE_HEADERS
    "include/kotono_core/CameraComponent.h"
    "include/kotono_core/DirectionalLightComponent.h"
    "include/kotono_core/PointLightComponent.h"
    "include/kotono_core/MeshComponent.h"
    "include/kotono_core/Task.h"
)
set(KCORE_SOURCES
    "src/CameraComponent.cpp"
    "src/DirectionalLightComponent.cpp"
    "src/PointLightComponent.cpp"
    "src/MeshComponent.cpp"
    "src/Task.cpp"
)

target_sources(KotonoCore
    PRIVATE ${KCORE_HEADERS}
    PRIVATE ${KCORE_SOURCES}
)

# Include directories
target_include_directories(KotonoCore
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_core"
)

# Link libs
target_link_libraries(KotonoCore PUBLIC
    KotonoObject
)
