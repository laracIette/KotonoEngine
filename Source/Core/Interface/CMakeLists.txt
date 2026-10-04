cmake_minimum_required(VERSION 4.2)
project(KotonoInterface LANGUAGES C CXX)

add_library(KotonoInterface STATIC)

target_compile_features(KotonoInterface PUBLIC cxx_std_23)

set_target_properties(KotonoInterface PROPERTIES FOLDER "KotonoEngine/Core")

set(KINTERFACE_HEADERS
    "widgets.h"
    "Align/Align.h"
    "Box/Box.h"
    "Button/Button.h"
    "ChildOwner/ChildOwner.h"
    "ChildrenOwner/ChildrenOwner.h"
    "Color/Color.h"
    "Column/Column.h"
    "Constraint/Constraint.h"
    "Crop/Crop.h"
    "Expanded/Expanded.h"
    "HorizontalWrapList/HorizontalWrapList.h"
    "HorizontalWrapListBody/HorizontalWrapListBody.h"
    "Image/Image.h"
    "List/List.h"
    "ListBody/ListBody.h"
    "Offset/Offset.h"
    "Padding/Padding.h"
    "Row/Row.h"
    "SceneTexture/SceneTexture.h"
    "Scrollable/Scrollable.h"
    "Socket/Socket.h"
    "Spacer/Spacer.h"
    "Stack/Stack.h"
    "Text/Text.h"
    "Wrap/Wrap.h"
)
set(KINTERFACE_SOURCES
    "Align/Align.cpp"
    "Box/Box.cpp"
    "Button/Button.cpp"
    "ChildOwner/ChildOwner.cpp"
    "ChildrenOwner/ChildrenOwner.cpp"
    "Color/Color.cpp"
    "Column/Column.cpp"
    "Constraint/Constraint.cpp"
    "Crop/Crop.cpp"
    "Expanded/Expanded.cpp"
    "HorizontalWrapList/HorizontalWrapList.cpp"
    "HorizontalWrapListBody/HorizontalWrapListBody.cpp"
    "Image/Image.cpp"
    "List/List.cpp"
    "ListBody/ListBody.cpp"
    "Offset/Offset.cpp"
    "Padding/Padding.cpp"
    "Row/Row.cpp"
    "SceneTexture/SceneTexture.cpp"
    "Scrollable/Scrollable.cpp"
    "Socket/Socket.cpp"
    "Spacer/Spacer.cpp"
    "Stack/Stack.cpp"
    "Text/Text.cpp"
    "Wrap/Wrap.cpp"
)

target_sources(KotonoInterface
    PRIVATE ${KINTERFACE_HEADERS}
    PRIVATE ${KINTERFACE_SOURCES}
)

target_include_directories(KotonoInterface
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoInterface PUBLIC
    KotonoObject
)
