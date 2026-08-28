cmake_minimum_required(VERSION 4.2)
project(KotonoObject LANGUAGES C CXX)

add_library(KotonoObject STATIC)

target_compile_features(KotonoObject PUBLIC cxx_std_23)

set_target_properties(KotonoObject PROPERTIES FOLDER "KotonoEngine")

set(KOBJECT_HEADERS
    "include/kotono_object/Axis.h"
    "include/kotono_object/CoordinateSpace.h"
    "include/kotono_object/Expand.h"
    "include/kotono_object/Flex.h"
    "include/kotono_object/GameState.h"
    "include/kotono_object/Guid.h"
    "include/kotono_object/Interface.h"
    "include/kotono_object/Object.h"
    "include/kotono_object/ObjectFactory.h"
    "include/kotono_object/ObjectManager.h"
    "include/kotono_object/ProjectSettings.h"
    "include/kotono_object/Ptr.h"
    "include/kotono_object/Scene.h"
    "include/kotono_object/SceneComponent.h"
    "include/kotono_object/SceneContext.h"
    "include/kotono_object/SceneObject.h"
    "include/kotono_object/SceneWidget.h"
    "include/kotono_object/serialize_glm.h"    
    "include/kotono_object/Transform.h"    
    "include/kotono_object/VariableInfo.h"
    "include/kotono_object/Widget.h"
    "include/kotono_object/WidgetDisplaySettings.h"
)
set(KOBJECT_SOURCES
    "src/Guid.cpp"
    "src/Interface.cpp"
    "src/Object.cpp"
    "src/ObjectFactory.cpp"
    "src/ObjectManager.cpp"
    "src/ProjectSettings.cpp"       
    "src/Scene.cpp"
    "src/SceneComponent.cpp"
    "src/SceneContext.cpp"
    "src/SceneObject.cpp"
    "src/SceneWidget.cpp"
    "src/serialize_glm.cpp"
    "src/Transform.cpp"
    "src/Widget.cpp"
)

target_sources(KotonoObject
    PRIVATE ${KOBJECT_HEADERS}
    PRIVATE ${KOBJECT_SOURCES}
)

# Include directories
target_include_directories(KotonoObject
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_object"
)

# Link libs
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
