cmake_minimum_required(VERSION 3.20)
project(KotonoCore LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoCore STATIC)

# Require C++20
target_compile_features(KotonoCore PUBLIC cxx_std_20)

set_target_properties(KotonoCore PROPERTIES FOLDER "KotonoEngine")

set(KCORE_HEADERS
    "include/kotono_core/Anchor.h"
    "include/kotono_core/base_class.h"
    "include/kotono_core/CoordinateSpace.h"
    "include/kotono_core/Core.h"
    "include/kotono_core/Game.h"
    "include/kotono_core/Guid.h"
    "include/kotono_core/Interface.h"
    "include/kotono_core/InterfaceBoxComponent.h"
    "include/kotono_core/InterfaceButtonComponent.h"
    "include/kotono_core/InterfaceColliderComponent.h"
    "include/kotono_core/InterfaceComponent.h"
    "include/kotono_core/InterfaceImageComponent.h"
    "include/kotono_core/InterfaceImageObject.h"
    "include/kotono_core/InterfaceObject.h"
    "include/kotono_core/InterfacePhysicsManager.h"
    "include/kotono_core/math_utils.h"
    "include/kotono_core/Mobility.h"
    "include/kotono_core/Object.h"
    "include/kotono_core/ObjectFactory.h"
    "include/kotono_core/ObjectManager.h"
    "include/kotono_core/Orientation.h"
    "include/kotono_core/ProjectSettings.h"
    "include/kotono_core/Ptr.h"
    "include/kotono_core/Rect.h"
    "include/kotono_core/RotationUnit.h"
    "include/kotono_core/Scene.h"
    "include/kotono_core/SceneComponent.h"
    "include/kotono_core/SceneMeshComponent.h"
    "include/kotono_core/SceneMeshObject.h"
    "include/kotono_core/SceneObject.h"
    "include/kotono_core/serialize.h"
    "include/kotono_core/Task.h"
    "include/kotono_core/TimeManager.h"
    "include/kotono_core/Timer.h"
    "include/kotono_core/Transform.h"
    "include/kotono_core/Visibility.h"
)
set(KCORE_SOURCES
    "src/Core.cpp"
    "src/Game.cpp"
    "src/Guid.cpp"
    "src/Interface.cpp"
    "src/InterfaceBoxComponent.cpp"
    "src/InterfaceButtonComponent.cpp"
    "src/InterfaceColliderComponent.cpp"
    "src/InterfaceComponent.cpp"
    "src/InterfaceImageComponent.cpp"
    "src/InterfaceImageObject.cpp"
    "src/InterfaceObject.cpp"
    "src/InterfacePhysicsManager.cpp"
    "src/math_utils.cpp"
    "src/Object.cpp"
    "src/ObjectFactory.cpp"
    "src/ObjectManager.cpp"
    "src/ProjectSettings.cpp"
    "src/Scene.cpp"
    "src/SceneComponent.cpp"
    "src/SceneMeshComponent.cpp"
    "src/SceneMeshObject.cpp"
    "src/SceneObject.cpp"
    "src/serialize.cpp"
    "src/Task.cpp"
    "src/TimeManager.cpp"
    "src/Timer.cpp"
)

set(KCORE_GENERATED_HEADERS
    "include/kotono_core/generated/Interface.generated.h"
    "include/kotono_core/generated/InterfaceBoxComponent.generated.h"
    "include/kotono_core/generated/InterfaceButtonComponent.generated.h"
    "include/kotono_core/generated/InterfaceColliderComponent.generated.h"
    "include/kotono_core/generated/InterfaceComponent.generated.h"
    "include/kotono_core/generated/InterfaceImageComponent.generated.h"
    "include/kotono_core/generated/InterfaceImageObject.generated.h"
    "include/kotono_core/generated/InterfaceObject.generated.h"
    "include/kotono_core/generated/Object.generated.h"
    "include/kotono_core/generated/Scene.generated.h"
    "include/kotono_core/generated/SceneComponent.generated.h"
    "include/kotono_core/generated/SceneMeshComponent.generated.h"
    "include/kotono_core/generated/SceneMeshObject.generated.h"
    "include/kotono_core/generated/SceneObject.generated.h"
)
set(KCORE_GENERATED_SOURCES
    "src/generated/Interface.generated.cpp"
    "src/generated/InterfaceBoxComponent.generated.cpp"
    "src/generated/InterfaceButtonComponent.generated.cpp"
    "src/generated/InterfaceColliderComponent.generated.cpp"
    "src/generated/InterfaceComponent.generated.cpp"
    "src/generated/InterfaceImageComponent.generated.cpp"
    "src/generated/InterfaceImageObject.generated.cpp"
    "src/generated/InterfaceObject.generated.cpp"
    "src/generated/Object.generated.cpp"
    "src/generated/Scene.generated.cpp"
    "src/generated/SceneComponent.generated.cpp"
    "src/generated/SceneMeshComponent.generated.cpp"
    "src/generated/SceneMeshObject.generated.cpp"
    "src/generated/SceneObject.generated.cpp"
)

target_sources(KotonoCore
    PRIVATE ${KCORE_HEADERS}
    PRIVATE ${KCORE_SOURCES}
    PRIVATE ${KCORE_GENERATED_HEADERS}
    PRIVATE ${KCORE_GENERATED_SOURCES}
)

# Include directories
target_include_directories(KotonoCore
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_core"
)

# Link libs
target_link_libraries(KotonoCore PUBLIC
    KotonoAudio
    KotonoInput
    KotonoTiming
    KotonoGraphics
)

add_custom_target(RunKotonoGenerator
    COMMAND $<TARGET_FILE:KotonoGenerator>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:KotonoGenerator>
    DEPENDS KotonoGenerator
    COMMENT "Running KotonoGenerator..."
)

add_dependencies(KotonoCore RunKotonoGenerator)
