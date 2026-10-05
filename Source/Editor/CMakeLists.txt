cmake_minimum_required(VERSION 4.2)
project(KotonoEditor LANGUAGES C CXX)

add_library(KotonoEditor STATIC)

target_compile_features(KotonoEditor PUBLIC cxx_std_23)

set_target_properties(KotonoEditor PROPERTIES FOLDER "KotonoEngine")

set(KEDITOR_PUBLIC_HEADERS
    "Public/MainWindow/MainWindow.h"
    "Public/SecondaryWindow/SecondaryWindow.h"
)
set(KEDITOR_PRIVATE_HEADERS
    "Private/AssetExplorer/AssetExplorer.h"
    "Private/AssetExplorerDirectory/AssetExplorerDirectory.h"
    "Private/AssetExplorerFile/AssetExplorerFile.h"
    "Private/AssetExplorerItem/AssetExplorerItem.h"
    "Private/DefaultSceneContext/DefaultSceneContext.h"
    "Private/Detachable/Detachable.h"
    "Private/GameSpeedButton/GameSpeedButton.h"
    "Private/GameStateButton/GameStateButton.h"
    "Private/InputTextBox/InputTextBox.h"
    "Private/ObjectProperties/ObjectProperties.h"
    "Private/PropertiesWindow/PropertiesWindow.h"
    "Private/SceneExplorer/SceneExplorer.h"
    "Private/SceneExplorerAddButton/SceneExplorerAddButton.h"
    "Private/SceneExplorerItem/SceneExplorerItem.h"
    "Private/SceneExplorerRemoveButton/SceneExplorerRemoveButton.h"
    "Private/SceneVisibilityField/SceneVisibilityField.h"
    "Private/SceneVisibilityWindow/SceneVisibilityWindow.h"
    "Private/SingleViewport/SingleViewport.h"
    "Private/UpdateTimeText/UpdateTimeText.h"
    "Private/ValueBox/ValueBox.h"
    "Private/ValueSlider/ValueSlider.h"
    "Private/ViewController/ViewController.h"
)
set(KEDITOR_SOURCES
    "Private/AssetExplorer/AssetExplorer.cpp"
    "Private/AssetExplorerDirectory/AssetExplorerDirectory.cpp"
    "Private/AssetExplorerFile/AssetExplorerFile.cpp"
    "Private/AssetExplorerItem/AssetExplorerItem.cpp"
    "Private/DefaultSceneContext/DefaultSceneContext.cpp"
    "Private/Detachable/Detachable.cpp"
    "Private/GameSpeedButton/GameSpeedButton.cpp"
    "Private/GameStateButton/GameStateButton.cpp"
    "Private/InputTextBox/InputTextBox.cpp"
    "Private/MainWindow/MainWindow.cpp"
    "Private/ObjectProperties/ObjectProperties.cpp"
    "Private/PropertiesWindow/PropertiesWindow.cpp"
    "Private/SceneExplorer/SceneExplorer.cpp"
    "Private/SceneExplorerAddButton/SceneExplorerAddButton.cpp"
    "Private/SceneExplorerItem/SceneExplorerItem.cpp"
    "Private/SceneExplorerRemoveButton/SceneExplorerRemoveButton.cpp"
    "Private/SceneVisibilityField/SceneVisibilityField.cpp"
    "Private/SceneVisibilityWindow/SceneVisibilityWindow.cpp"
    "Private/SecondaryWindow/SecondaryWindow.cpp"
    "Private/SingleViewport/SingleViewport.cpp"
    "Private/UpdateTimeText/UpdateTimeText.cpp"
    "Private/ValueBox/ValueBox.cpp"
    "Private/ValueSlider/ValueSlider.cpp"
    "Private/ViewController/ViewController.cpp"
)

target_sources(KotonoEditor
    PRIVATE
        ${KEDITOR_SOURCES}
        ${KEDITOR_PRIVATE_HEADERS}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KEDITOR_PUBLIC_HEADERS}
)

target_include_directories(KotonoEditor
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoEditor PUBLIC
    KotonoObject
    KotonoInterface
)
