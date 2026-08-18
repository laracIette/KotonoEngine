cmake_minimum_required(VERSION 4.2)
project(KotonoMath LANGUAGES C CXX)

add_library(KotonoMath STATIC)

target_compile_features(KotonoMath PUBLIC cxx_std_23)

set_target_properties(KotonoMath PROPERTIES FOLDER "KotonoEngine")

set(KMATH_HEADERS
    "include/kotono_math/math_utils.h"
)
set(KMATH_SOURCES
    "src/math_utils.cpp"
)
    
target_compile_definitions(KotonoMath PUBLIC
    GLM_FORCE_RADIANS
    GLM_FORCE_LEFT_HANDED
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_ENABLE_EXPERIMENTAL
    GLM_FORCE_CXX20
)

target_sources(KotonoMath
    PRIVATE ${KMATH_HEADERS}
    PRIVATE ${KMATH_SOURCES}
)

set(THIRD_PARTY_DIR "${ENGINE_PATH}/ThirdParty")

# Include directories
target_include_directories(KotonoMath
    PUBLIC  "${THIRD_PARTY_DIR}/glm/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_math"
)