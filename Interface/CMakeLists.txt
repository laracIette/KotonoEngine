cmake_minimum_required(VERSION 3.20)
project(KotonoInterface LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoInterface STATIC)

# Require C++20
target_compile_features(KotonoInterface PUBLIC cxx_std_20)

set(KINTERFACE_HEADERS
    "include/kotono_interface/Box.h"
    "include/kotono_interface/Button.h"
    "include/kotono_interface/Center.h"
    "include/kotono_interface/ChildOwnerWidget.h"
    "include/kotono_interface/ChildrenOwnerWidget.h"
    "include/kotono_interface/Color.h"
    "include/kotono_interface/Column.h"
    "include/kotono_interface/Constraint.h"
    "include/kotono_interface/Expanded.h"
    "include/kotono_interface/Flex.h"
    "include/kotono_interface/Image.h"
    "include/kotono_interface/List.h"
    "include/kotono_interface/ListBody.h"
    "include/kotono_interface/Offset.h"
    "include/kotono_interface/Padding.h"
    "include/kotono_interface/Row.h"
    "include/kotono_interface/Spacer.h"
    "include/kotono_interface/Stack.h"
    "include/kotono_interface/Text.h"
    "include/kotono_interface/utils.h"
    "include/kotono_interface/Widget.h"
    "include/kotono_interface/widgets.h"
)
set(KINTERFACE_SOURCES
    "src/Box.cpp"
    "src/Button.cpp"
    "src/Center.cpp"
    "src/ChildOwnerWidget.cpp"
    "src/ChildrenOwnerWidget.cpp"
    "src/Color.cpp"
    "src/Column.cpp"
    "src/Constraint.cpp"
    "src/Expanded.cpp"
    "src/Image.cpp"
    "src/List.cpp"
    "src/ListBody.cpp"
    "src/Offset.cpp"
    "src/Padding.cpp"
    "src/Row.cpp"
    "src/Spacer.cpp"
    "src/Stack.cpp"
    "src/Text.cpp"
    "src/utils.cpp"
    "src/Widget.cpp"
)

target_sources(KotonoInterface
    PRIVATE ${KINTERFACE_HEADERS}
    PRIVATE ${KINTERFACE_SOURCES}
)

# Include directories
target_include_directories(KotonoInterface
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_interface"
)

# Link libs
target_link_libraries(KotonoInterface PUBLIC
    KotonoInput
    KotonoGraphics
)
