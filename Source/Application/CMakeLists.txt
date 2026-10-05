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
    PRIVATE
        ${KAPPLICATION_SOURCES}
        ${KAPPLICATION_PRIVATE_HEADERS}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KAPPLICATION_PUBLIC_HEADERS}
)

target_include_directories(KotonoApplication
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

file(GLOB_RECURSE KSHADERS CONFIGURE_DEPENDS "${ENGINE_PATH}/Shaders/*")
set_source_files_properties(${KSHADERS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(TREE "${ENGINE_PATH}/Shaders" PREFIX "Shaders" FILES ${KSHADERS})
target_sources(KotonoApplication PRIVATE ${KSHADERS})

file(GLOB_RECURSE KASSETS CONFIGURE_DEPENDS "${ENGINE_PATH}/Assets/*")
set_source_files_properties(${KASSETS} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(TREE "${ENGINE_PATH}/Assets" PREFIX "Assets" FILES ${KASSETS})
target_sources(KotonoApplication PRIVATE ${KASSETS})
