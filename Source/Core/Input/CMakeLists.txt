cmake_minimum_required(VERSION 4.2)
project(KotonoInput LANGUAGES C CXX)

add_library(KotonoInput STATIC)

target_compile_features(KotonoInput PUBLIC cxx_std_23)

set_target_properties(KotonoInput PROPERTIES FOLDER "KotonoEngine/Core")

set(KINPUT_HEADERS
    "Button.h"
    "InputState.h"
    "Key.h"
    "Modifier.h"
    "InputHoldAction/InputHoldAction.h"
    "Keyboard/Keyboard.h"
    "Mouse/Mouse.h"
)
set(KINPUT_SOURCES
    "InputHoldAction/InputHoldAction.cpp"
    "Keyboard/Keyboard.cpp"
    "Mouse/Mouse.cpp"
)

target_sources(KotonoInput
    PRIVATE ${KINPUT_HEADERS}
    PRIVATE ${KINPUT_SOURCES}
)

target_include_directories(KotonoInput
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoInput PRIVATE
    KotonoPlatform
)
