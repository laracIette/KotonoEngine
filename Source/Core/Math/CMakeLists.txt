cmake_minimum_required(VERSION 4.2)
project(KotonoMath LANGUAGES C CXX)

add_library(KotonoMath STATIC)

target_compile_features(KotonoMath PUBLIC cxx_std_23)

set_target_properties(KotonoMath PROPERTIES FOLDER "KotonoEngine/Core")

set(KMATH_PUBLIC_HEADERS
    "Public/math_utils/math_utils.h"
)
set(KMATH_SOURCES
    "Private/math_utils/math_utils.cpp"
)

target_compile_definitions(KotonoMath PUBLIC
    GLM_FORCE_RADIANS
    GLM_FORCE_LEFT_HANDED
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_ENABLE_EXPERIMENTAL
    GLM_FORCE_CXX20
)

target_sources(KotonoMath
    PRIVATE
        ${KMATH_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KMATH_PUBLIC_HEADERS}
)

target_include_directories(KotonoMath
    PUBLIC "${ENGINE_PATH}/Source/ThirdParty/glm/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)
