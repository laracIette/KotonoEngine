cmake_minimum_required(VERSION 4.2)
project(KotonoInterface LANGUAGES C CXX)

add_library(KotonoInterface STATIC)

target_compile_features(KotonoInterface PUBLIC cxx_std_23)

set_target_properties(KotonoInterface PROPERTIES FOLDER "KotonoEngine")

set(KINTERFACE_HEADERS
    "include/kotono_interface/Box.h"
    "include/kotono_interface/Button.h"
    "include/kotono_interface/Center.h"
    "include/kotono_interface/ChildOwner.h"
    "include/kotono_interface/ChildrenOwner.h"
    "include/kotono_interface/Color.h"
    "include/kotono_interface/Column.h"
    "include/kotono_interface/Constraint.h"
    "include/kotono_interface/Expanded.h"
    "include/kotono_interface/HorizontalWrapList.h"
    "include/kotono_interface/HorizontalWrapListBody.h"
    "include/kotono_interface/Image.h"
    "include/kotono_interface/List.h"
    "include/kotono_interface/ListBody.h"
    "include/kotono_interface/Offset.h"
    "include/kotono_interface/Padding.h"
    "include/kotono_interface/Row.h"
    "include/kotono_interface/SceneRenderer.h"
    "include/kotono_interface/Scrollable.h"
    "include/kotono_interface/Spacer.h"
    "include/kotono_interface/Stack.h"
    "include/kotono_interface/Text.h"
    "include/kotono_interface/widgets.h"
    "include/kotono_interface/Wrap.h"
)
set(KINTERFACE_SOURCES
    "src/Box.cpp"
    "src/Button.cpp"
    "src/Center.cpp"
    "src/ChildOwner.cpp"
    "src/ChildrenOwner.cpp"
    "src/Color.cpp"
    "src/Column.cpp"
    "src/Constraint.cpp"
    "src/Expanded.cpp"
    "src/HorizontalWrapList.cpp"
    "src/HorizontalWrapListBody.cpp"
    "src/Image.cpp"
    "src/List.cpp"
    "src/ListBody.cpp"
    "src/Offset.cpp"
    "src/Padding.cpp"
    "src/Row.cpp"
    "src/SceneRenderer.cpp"
    "src/Scrollable.cpp"
    "src/Spacer.cpp"
    "src/Stack.cpp"
    "src/Text.cpp"
    "src/Wrap.cpp"
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
    KotonoObject
)
