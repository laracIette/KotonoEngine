cmake_minimum_required(VERSION 4.2)
project(KotonoCore LANGUAGES C CXX)

add_library(KotonoCore STATIC)

target_compile_features(KotonoCore PUBLIC cxx_std_23)

set_target_properties(KotonoCore PROPERTIES FOLDER "KotonoEngine")

set(KCORE_HEADERS
    "include/kotono_core/Axis.h"
    "include/kotono_core/CoordinateSpace.h"
    "include/kotono_core/Expand.h"
    "include/kotono_core/Flex.h"
    "include/kotono_core/GameState.h"
    "include/kotono_core/Guid.h"
    "include/kotono_core/Interface.h"
    "include/kotono_core/Object.h"
    "include/kotono_core/ObjectFactory.h"
    "include/kotono_core/ObjectManager.h"
    "include/kotono_core/ProjectSettings.h"
    "include/kotono_core/Ptr.h"
    "include/kotono_core/Scene.h"
    "include/kotono_core/SceneComponent.h"
    "include/kotono_core/SceneContext.h"
    "include/kotono_core/SceneObject.h"
    "include/kotono_core/SceneWidget.h"
    "include/kotono_core/serialize_glm.h"
    "include/kotono_core/Transform.h"
    "include/kotono_core/VariableInfo.h"
    "include/kotono_core/Widget.h"
    "include/kotono_core/WidgetDisplaySettings.h"
)
set(KCORE_SOURCES
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

add_dependencies(KotonoCore RunKotonoGenerator)
