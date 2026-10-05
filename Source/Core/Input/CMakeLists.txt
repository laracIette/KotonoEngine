cmake_minimum_required(VERSION 4.2)
project(KotonoInput LANGUAGES C CXX)

add_library(KotonoInput STATIC)

target_compile_features(KotonoInput PUBLIC cxx_std_23)

set_target_properties(KotonoInput PROPERTIES FOLDER "KotonoEngine/Core")

set(KINPUT_PUBLIC_HEADERS
    "Public/Button.h"
    "Public/InputState.h"
    "Public/Key.h"
    "Public/Modifier.h"
    "Public/InputHoldAction/InputHoldAction.h"
    "Public/Keyboard/Keyboard.h"
    "Public/Mouse/Mouse.h"
)
set(KINPUT_SOURCES
    "Private/InputHoldAction/InputHoldAction.cpp"
    "Private/Keyboard/Keyboard.cpp"
    "Private/Mouse/Mouse.cpp"
)

target_sources(KotonoInput
    PRIVATE
        ${KINPUT_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KINPUT_PUBLIC_HEADERS}
)

target_include_directories(KotonoInput
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoInput PRIVATE
    KotonoPlatform
)
