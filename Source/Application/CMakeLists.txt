cmake_minimum_required(VERSION 4.2)
project(KotonoApplication LANGUAGES C CXX)

add_library(KotonoApplication STATIC)

target_compile_features(KotonoApplication PUBLIC cxx_std_23)

set_target_properties(KotonoApplication PROPERTIES FOLDER "KotonoEngine")


set(KAPPLICATION_PUBLIC_HEADERS
    "Public/Application/Application.h"
)
set(KAPPLICATION_PRIVATE_HEADERS
    "Private/ClassRegistrator/ClassRegistrator.h"
    "Private/WindowContext/WindowContext.h"
)
set(KAPPLICATION_SOURCES
    "Private/Application/Application.cpp"
    "Private/ClassRegistrator/ClassRegistrator.cpp"
    "Private/WindowContext/WindowContext.cpp"
)

target_sources(KotonoApplication
    PUBLIC ${KAPPLICATION_PUBLIC_HEADERS}
    PRIVATE ${KAPPLICATION_PRIVATE_HEADERS}
    PRIVATE ${KAPPLICATION_SOURCES}
)

target_include_directories(KotonoApplication
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

if (TARGET KotonoEditor)
    target_compile_definitions(KotonoApplication PUBLIC EDITOR)
    target_link_libraries(KotonoApplication PUBLIC
        KotonoRendering
        KotonoScene
        KotonoEditor
    )
else()
    target_link_libraries(KotonoApplication PUBLIC
        KotonoRendering
        KotonoScene
    )
endif()
