cmake_minimum_required(VERSION 4.2)
project(KotonoInterface LANGUAGES C CXX)

add_library(KotonoInterface STATIC)

target_compile_features(KotonoInterface PUBLIC cxx_std_23)

set_target_properties(KotonoInterface PROPERTIES FOLDER "KotonoEngine/Core")

set(KINTERFACE_PUBLIC_HEADERS
    "Public/core_widgets.h"
    "Public/CoreWidgets/Align/Align.h"
    "Public/CoreWidgets/Box/Box.h"
    "Public/CoreWidgets/Button/Button.h"
    "Public/ChildOwner/ChildOwner.h"
    "Public/ChildrenOwner/ChildrenOwner.h"
    "Public/CoreWidgets/Color/Color.h"
    "Public/CoreWidgets/Column/Column.h"
    "Public/CoreWidgets/Constraint/Constraint.h"
    "Public/CoreWidgets/Crop/Crop.h"
    "Public/CoreWidgets/Expanded/Expanded.h"
    "Public/CoreWidgets/HorizontalWrapList/HorizontalWrapList.h"
    "Public/CoreWidgets/Image/Image.h"
    "Public/CoreWidgets/List/List.h"
    "Public/CoreWidgets/Offset/Offset.h"
    "Public/CoreWidgets/Padding/Padding.h"
    "Public/CoreWidgets/Row/Row.h"
    "Public/CoreWidgets/SceneTexture/SceneTexture.h"
    "Public/CoreWidgets/Scrollable/Scrollable.h"
    "Public/CoreWidgets/Socket/Socket.h"
    "Public/CoreWidgets/Spacer/Spacer.h"
    "Public/CoreWidgets/Stack/Stack.h"
    "Public/CoreWidgets/Switcher/Switcher.h"
    "Public/CoreWidgets/Text/Text.h"
    "Public/CoreWidgets/Wrap/Wrap.h"
)
set(KINTERFACE_PRIVATE_HEADERS
    "Private/HorizontalWrapListBody/HorizontalWrapListBody.h"
    "Private/ListBody/ListBody.h"
)
set(KINTERFACE_SOURCES
    "Private/CoreWidgets/Align/Align.cpp"
    "Private/CoreWidgets/Box/Box.cpp"
    "Private/CoreWidgets/Button/Button.cpp"
    "Private/ChildOwner/ChildOwner.cpp"
    "Private/ChildrenOwner/ChildrenOwner.cpp"
    "Private/CoreWidgets/Color/Color.cpp"
    "Private/CoreWidgets/Column/Column.cpp"
    "Private/CoreWidgets/Constraint/Constraint.cpp"
    "Private/CoreWidgets/Crop/Crop.cpp"
    "Private/CoreWidgets/Expanded/Expanded.cpp"
    "Private/CoreWidgets/HorizontalWrapList/HorizontalWrapList.cpp"
    "Private/HorizontalWrapListBody/HorizontalWrapListBody.cpp"
    "Private/CoreWidgets/Image/Image.cpp"
    "Private/CoreWidgets/List/List.cpp"
    "Private/ListBody/ListBody.cpp"
    "Private/CoreWidgets/Offset/Offset.cpp"
    "Private/CoreWidgets/Padding/Padding.cpp"
    "Private/CoreWidgets/Row/Row.cpp"
    "Private/CoreWidgets/SceneTexture/SceneTexture.cpp"
    "Private/CoreWidgets/Scrollable/Scrollable.cpp"
    "Private/CoreWidgets/Socket/Socket.cpp"
    "Private/CoreWidgets/Spacer/Spacer.cpp"
    "Private/CoreWidgets/Stack/Stack.cpp"
    "Private/CoreWidgets/Switcher/Switcher.cpp"
    "Private/CoreWidgets/Text/Text.cpp"
    "Private/CoreWidgets/Wrap/Wrap.cpp"
)

target_sources(KotonoInterface
    PRIVATE
        ${KINTERFACE_SOURCES}
        ${KINTERFACE_PRIVATE_HEADERS}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KINTERFACE_PUBLIC_HEADERS}
)

target_include_directories(KotonoInterface
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoInterface PUBLIC
    KotonoObject
)
