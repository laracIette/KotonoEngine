cmake_minimum_required(VERSION 4.2)
project(KotonoApplication LANGUAGES C CXX)

add_library(KotonoApplication STATIC)

target_compile_features(KotonoApplication PUBLIC cxx_std_23)

set_target_properties(KotonoApplication PROPERTIES FOLDER "KotonoEngine")


set(KAPPLICATION_HEADERS
    "include/kotono_application/Application.h"
)
set(KAPPLICATION_SOURCES
    "src/Application.cpp"
)

target_sources(KotonoApplication
    PRIVATE ${KAPPLICATION_HEADERS}
    PRIVATE ${KAPPLICATION_SOURCES}
)

target_include_directories(KotonoApplication
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_application"
)

if (TARGET KotonoEditor)
    target_compile_definitions(KotonoApplication PUBLIC EDITOR)
    target_link_libraries(KotonoApplication PUBLIC
        KotonoRendering
        KotonoEditor
    )
else()
    target_link_libraries(KotonoApplication PUBLIC
        KotonoRendering
        KotonoCore
    )
endif()
