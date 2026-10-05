cmake_minimum_required(VERSION 4.2)
project(KotonoReflection LANGUAGES C CXX)

add_library(KotonoReflection STATIC)

target_compile_features(KotonoReflection PUBLIC cxx_std_23)

set_target_properties(KotonoReflection PROPERTIES FOLDER "KotonoEngine/Core")

set(KREFLECTION_PUBLIC_HEADERS
    "Public/ReflectionResult.h"
    "Public/Reflector/Reflector.h"
)
set(KREFLECTION_SOURCES
    "Private/Reflector/Reflector.cpp"
)

target_sources(KotonoReflection
    PRIVATE
        ${KREFLECTION_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KREFLECTION_PUBLIC_HEADERS}
)

target_include_directories(KotonoReflection
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoReflection PUBLIC
    KotonoIO
)
