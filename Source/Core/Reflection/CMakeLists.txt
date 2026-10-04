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
    PUBLIC ${KREFLECTION_PUBLIC_HEADERS}
    PRIVATE ${KREFLECTION_SOURCES}
)

target_include_directories(KotonoReflection
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoReflection PUBLIC
    KotonoIO
)
