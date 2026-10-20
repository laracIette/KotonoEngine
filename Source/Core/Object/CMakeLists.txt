cmake_minimum_required(VERSION 4.2)
project(KotonoObject LANGUAGES C CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_library(KotonoObject STATIC)

target_compile_features(KotonoObject PUBLIC cxx_std_23)

set_target_properties(KotonoObject PROPERTIES FOLDER "KotonoEngine/Core")

set(KOBJECT_PUBLIC
    "Public/Axis.h"
    "Public/CoordinateSpace.h"
    "Public/Expand.h"
    "Public/Flex.h"
    "Public/GameState.h"
    "Public/VariableInfo.h"
    "Public/WidgetDisplaySettings.h"
    "Public/Guid/Guid.h"
    "Public/Interface/Interface.h"
    "Public/Object/ObjectBase.h"
    "Public/Object/ObjectReflectionBase.h"
    "Public/ObjectFactory/ObjectFactory.h"
    "Public/ProjectSettings/ProjectSettings.h"
    "Public/Ptr/Ptr.h"
    "Public/Ptr/ObjectPtr.h"
    "Public/Scene/Scene.h"
    "Public/serialize_glm/serialize_glm.h"
    "Public/Transform/Transform.h"
)
set(KOBJECT_PRIVATE
    "Private/Guid/Guid.cpp"
    "Private/Interface/Interface.cpp"
    "Private/Object/ObjectReflectionBase.cpp"
    "Private/ObjectFactory/ObjectFactory.cpp"
    "Private/ProjectSettings/ProjectSettings.cpp"
    "Private/Scene/Scene.cpp"
    "Private/serialize_glm/serialize_glm.cpp"
    "Private/Transform/Transform.cpp"
)

include("${CMAKE_CURRENT_SOURCE_DIR}/ObjectClasses.cmake")
list(APPEND KOBJECT_PUBLIC ${CLASS_PUBLIC_HEADERS})
list(APPEND KOBJECT_PRIVATE ${CLASS_PRIVATE_HEADERS} ${CLASS_SOURCES})

target_sources(KotonoObject
    PRIVATE
        ${KOBJECT_PRIVATE}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KOBJECT_PUBLIC}
)

target_include_directories(KotonoObject
    PUBLIC "${ENGINE_PATH}/Cache/Generator/Generated"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoObject PUBLIC
    KotonoAudio
    KotonoInput
    KotonoGraphics
    KotonoTiming
)

add_custom_target(RunGenerationScript
    COMMAND ${Python3_EXECUTABLE} "${ENGINE_PATH}/Scripts/Generation.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Running generation script..."
)

set_target_properties(RunGenerationScript PROPERTIES FOLDER "KotonoEngine/UtilityTargets")

add_dependencies(KotonoObject RunGenerationScript)
