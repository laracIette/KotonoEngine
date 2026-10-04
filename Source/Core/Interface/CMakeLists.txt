cmake_minimum_required(VERSION 4.2)
project(KotonoInterface LANGUAGES C CXX)

add_library(KotonoInterface STATIC)

target_compile_features(KotonoInterface PUBLIC cxx_std_23)

set_target_properties(KotonoInterface PROPERTIES FOLDER "KotonoEngine/Core")

set(KINTERFACE_PUBLIC_HEADERS
    "Public/widgets.h"
    "Public/Align/Align.h"
    "Public/Box/Box.h"
    "Public/Button/Button.h"
    "Public/ChildOwner/ChildOwner.h"
    "Public/ChildrenOwner/ChildrenOwner.h"
    "Public/Color/Color.h"
    "Public/Column/Column.h"
    "Public/Constraint/Constraint.h"
    "Public/Crop/Crop.h"
    "Public/Expanded/Expanded.h"
    "Public/HorizontalWrapList/HorizontalWrapList.h"
    "Public/Image/Image.h"
    "Public/List/List.h"
    "Public/Offset/Offset.h"
    "Public/Padding/Padding.h"
    "Public/Row/Row.h"
    "Public/SceneTexture/SceneTexture.h"
    "Public/Scrollable/Scrollable.h"
    "Public/Socket/Socket.h"
    "Public/Spacer/Spacer.h"
    "Public/Stack/Stack.h"
    "Public/Text/Text.h"
    "Public/Wrap/Wrap.h"
)
set(KINTERFACE_PRIVATE_HEADERS
    "Private/HorizontalWrapListBody/HorizontalWrapListBody.h"
    "Private/ListBody/ListBody.h"
)
set(KINTERFACE_SOURCES
    "Private/Align/Align.cpp"
    "Private/Box/Box.cpp"
    "Private/Button/Button.cpp"
    "Private/ChildOwner/ChildOwner.cpp"
    "Private/ChildrenOwner/ChildrenOwner.cpp"
    "Private/Color/Color.cpp"
    "Private/Column/Column.cpp"
    "Private/Constraint/Constraint.cpp"
    "Private/Crop/Crop.cpp"
    "Private/Expanded/Expanded.cpp"
    "Private/HorizontalWrapList/HorizontalWrapList.cpp"
    "Private/HorizontalWrapListBody/HorizontalWrapListBody.cpp"
    "Private/Image/Image.cpp"
    "Private/List/List.cpp"
    "Private/ListBody/ListBody.cpp"
    "Private/Offset/Offset.cpp"
    "Private/Padding/Padding.cpp"
    "Private/Row/Row.cpp"
    "Private/SceneTexture/SceneTexture.cpp"
    "Private/Scrollable/Scrollable.cpp"
    "Private/Socket/Socket.cpp"
    "Private/Spacer/Spacer.cpp"
    "Private/Stack/Stack.cpp"
    "Private/Text/Text.cpp"
    "Private/Wrap/Wrap.cpp"
)

target_sources(KotonoInterface
    PUBLIC ${KINTERFACE_PUBLIC_HEADERS}
    PRIVATE ${KINTERFACE_PRIVATE_HEADERS}
    PRIVATE ${KINTERFACE_SOURCES}
)

target_include_directories(KotonoInterface
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoInterface PUBLIC
    KotonoObject
)
