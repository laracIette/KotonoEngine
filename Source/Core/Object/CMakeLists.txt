cmake_minimum_required(VERSION 4.2)
project(KotonoObject LANGUAGES C CXX)

add_library(KotonoObject STATIC)

target_compile_features(KotonoObject PUBLIC cxx_std_23)

set_target_properties(KotonoObject PROPERTIES FOLDER "KotonoEngine/Core")

set(KOBJECT_PUBLIC_HEADERS
    "Public/Axis.h"
    "Public/CoordinateSpace.h"
    "Public/Expand.h"
    "Public/Flex.h"
    "Public/GameState.h"
    "Public/Ptr.h"
    "Public/VariableInfo.h"
    "Public/WidgetDisplaySettings.h"
    "Public/Guid/Guid.h"
    "Public/Interface/Interface.h"
    "Public/InterfaceRoot/InterfaceRoot.h"
    "Public/Object/Object.h"
    "Public/ObjectFactory/ObjectFactory.h"
    "Public/ProjectSettings/ProjectSettings.h"
    "Public/Scene/Scene.h"
    "Public/SceneComponent/SceneComponent.h"
    "Public/SceneContext/SceneContext.h"
    "Public/SceneObject/SceneObject.h"
    "Public/SceneWidget/SceneWidget.h"
    "Public/serialize_glm/serialize_glm.h"
    "Public/Transform/Transform.h"
    "Public/Widget/Widget.h"
)
set(KOBJECT_SOURCES
    "Private/Guid/Guid.cpp"
    "Private/Interface/Interface.cpp"
    "Private/InterfaceRoot/InterfaceRoot.cpp"
    "Private/Object/Object.cpp"
    "Private/ObjectFactory/ObjectFactory.cpp"
    "Private/ProjectSettings/ProjectSettings.cpp"
    "Private/Scene/Scene.cpp"
    "Private/SceneComponent/SceneComponent.cpp"
    "Private/SceneContext/SceneContext.cpp"
    "Private/SceneObject/SceneObject.cpp"
    "Private/SceneWidget/SceneWidget.cpp"
    "Private/serialize_glm/serialize_glm.cpp"
    "Private/Transform/Transform.cpp"
    "Private/Widget/Widget.cpp"
)

target_sources(KotonoObject
    PUBLIC ${KOBJECT_PUBLIC_HEADERS}
    PRIVATE ${KOBJECT_SOURCES}
)

target_include_directories(KotonoObject
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoObject PUBLIC
    KotonoAudio
    KotonoInput
    KotonoGraphics
    KotonoTiming
)

add_custom_target(RunKotonoGenerator
    COMMAND $<TARGET_FILE:KotonoGenerator>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:KotonoGenerator>
    DEPENDS KotonoGenerator
    COMMENT "Running KotonoGenerator..."
)

set_target_properties(RunKotonoGenerator PROPERTIES FOLDER "KotonoEngine/UtilityTargets")

add_dependencies(KotonoObject RunKotonoGenerator)
