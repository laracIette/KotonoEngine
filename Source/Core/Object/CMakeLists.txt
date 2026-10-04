cmake_minimum_required(VERSION 4.2)
project(KotonoObject LANGUAGES C CXX)

add_library(KotonoObject STATIC)

target_compile_features(KotonoObject PUBLIC cxx_std_23)

set_target_properties(KotonoObject PROPERTIES FOLDER "KotonoEngine/Core")

set(KOBJECT_HEADERS
    "Axis.h"
    "CoordinateSpace.h"
    "Expand.h"
    "Flex.h"
    "GameState.h"
    "Ptr.h"
    "VariableInfo.h"
    "WidgetDisplaySettings.h"
    "Guid/Guid.h"
    "Interface/Interface.h"
    "InterfaceRoot/InterfaceRoot.h"
    "Object/Object.h"
    "ObjectFactory/ObjectFactory.h"
    "ProjectSettings/ProjectSettings.h"
    "Scene/Scene.h"
    "SceneComponent/SceneComponent.h"
    "SceneContext/SceneContext.h"
    "SceneObject/SceneObject.h"
    "SceneWidget/SceneWidget.h"
    "serialize_glm/serialize_glm.h"
    "Transform/Transform.h"
    "Widget/Widget.h"
)
set(KOBJECT_SOURCES
    "Guid/Guid.cpp"
    "Interface/Interface.cpp"
    "InterfaceRoot/InterfaceRoot.cpp"
    "Object/Object.cpp"
    "ObjectFactory/ObjectFactory.cpp"
    "ProjectSettings/ProjectSettings.cpp"
    "Scene/Scene.cpp"
    "SceneComponent/SceneComponent.cpp"
    "SceneContext/SceneContext.cpp"
    "SceneObject/SceneObject.cpp"
    "SceneWidget/SceneWidget.cpp"
    "serialize_glm/serialize_glm.cpp"
    "Transform/Transform.cpp"
    "Widget/Widget.cpp"
)

target_sources(KotonoObject
    PRIVATE ${KOBJECT_HEADERS}
    PRIVATE ${KOBJECT_SOURCES}
)

target_include_directories(KotonoObject
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
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
