cmake_minimum_required(VERSION 3.20)
project(KotonoObject LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
endif()

# Static library
add_library(KotonoObject STATIC)

# Require C++20
target_compile_features(KotonoObject PUBLIC cxx_std_23)

set_target_properties(KotonoObject PROPERTIES FOLDER "KotonoEngine")

set(KOBJECT_HEADERS
    "include/kotono_object/Guid.h"
    "include/kotono_object/Object.h"
    "include/kotono_object/ObjectFactory.h"
    "include/kotono_object/ObjectManager.h"
    "include/kotono_object/Ptr.h"
    "include/kotono_object/VariableInfo.h"
)
set(KOBJECT_SOURCES
    "src/Guid.cpp"
    "src/Object.cpp"
    "src/ObjectFactory.cpp"
    "src/ObjectManager.cpp"
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
    KotonoMath
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

set_target_properties(RunKotonoGenerator PROPERTIES FOLDER "KotonoEngine/UtilityTargets")

add_dependencies(KotonoObject RunKotonoGenerator)
