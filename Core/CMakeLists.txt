cmake_minimum_required(VERSION 4.2)
project(KotonoCore LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
endif()

# Static library
add_library(KotonoCore STATIC)

# Require C++20
target_compile_features(KotonoCore PUBLIC cxx_std_23)

set_target_properties(KotonoCore PROPERTIES FOLDER "KotonoEngine")

set(KCORE_HEADERS
    "include/kotono_core/Anchor.h"
    "include/kotono_core/CameraComponent.h"
    "include/kotono_core/CoordinateSpace.h"
    "include/kotono_core/Core.h"
    "include/kotono_core/DirectionalLightComponent.h"
    "include/kotono_core/Game.h"
    "include/kotono_core/GameState.h"
    "include/kotono_core/Interface.h"
    "include/kotono_core/InterfaceBoxComponent.h"
    "include/kotono_core/InterfaceButtonComponent.h"
    "include/kotono_core/InterfaceColliderComponent.h"
    "include/kotono_core/InterfaceComponent.h"
    "include/kotono_core/InterfaceImageComponent.h"
    "include/kotono_core/InterfaceObject.h"
    "include/kotono_core/InterfacePhysicsManager.h"
    "include/kotono_core/Orientation.h"
    "include/kotono_core/PointLightComponent.h"
    "include/kotono_core/ProjectSettings.h"
    "include/kotono_core/Rect.h"
    "include/kotono_core/RotationUnit.h"
    "include/kotono_core/Scene.h"
    "include/kotono_core/SceneComponent.h"
    "include/kotono_core/SceneMeshComponent.h"
    "include/kotono_core/SceneObject.h"
    "include/kotono_core/serialize_glm.h"
    "include/kotono_core/Task.h"
    "include/kotono_core/TimeManager.h"
    "include/kotono_core/Transform.h"
)
set(KCORE_SOURCES
    "src/CameraComponent.cpp"
    "src/Core.cpp"
    "src/DirectionalLightComponent.cpp"
    "src/Game.cpp"
    "src/Interface.cpp"
    "src/InterfaceBoxComponent.cpp"
    "src/InterfaceButtonComponent.cpp"
    "src/InterfaceColliderComponent.cpp"
    "src/InterfaceComponent.cpp"
    "src/InterfaceImageComponent.cpp"
    "src/InterfaceObject.cpp"
    "src/InterfacePhysicsManager.cpp"
    "src/PointLightComponent.cpp"
    "src/ProjectSettings.cpp"
    "src/Rect.cpp"
    "src/Scene.cpp"
    "src/SceneComponent.cpp"
    "src/SceneMeshComponent.cpp"
    "src/SceneObject.cpp"
    "src/serialize_glm.cpp"
    "src/Task.cpp"
    "src/TimeManager.cpp"
    "src/Transform.cpp"
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
