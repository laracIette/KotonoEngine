cmake_minimum_required(VERSION 4.2)
project(KotonoApplication LANGUAGES C CXX)

add_library(KotonoApplication STATIC)

target_compile_features(KotonoApplication PUBLIC cxx_std_23)

set_target_properties(KotonoApplication PROPERTIES FOLDER "KotonoEngine")


set(KAPPLICATION_HEADERS
    "Application/Application.h"
    "ClassRegistrator/ClassRegistrator.h"
    "WindowContext/WindowContext.h"
)
set(KAPPLICATION_SOURCES
    "Application/Application.cpp"
    "ClassRegistrator/ClassRegistrator.cpp"
    "WindowContext/WindowContext.cpp"
)

target_sources(KotonoApplication
    PRIVATE ${KAPPLICATION_HEADERS}
    PRIVATE ${KAPPLICATION_SOURCES}
)

target_include_directories(KotonoApplication
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
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
