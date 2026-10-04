cmake_minimum_required(VERSION 4.2)
project(KotonoEditor LANGUAGES C CXX)

add_library(KotonoEditor STATIC)

target_compile_features(KotonoEditor PUBLIC cxx_std_23)

set_target_properties(KotonoEditor PROPERTIES FOLDER "KotonoEngine")

set(KEDITOR_HEADERS
    "AssetExplorer/AssetExplorer.h"
    "AssetExplorerDirectory/AssetExplorerDirectory.h"
    "AssetExplorerFile/AssetExplorerFile.h"
    "AssetExplorerItem/AssetExplorerItem.h"
    "DefaultSceneContext/DefaultSceneContext.h"
    "Detachable/Detachable.h"
    "GameSpeedButton/GameSpeedButton.h"
    "GameStateButton/GameStateButton.h"
    "InputTextBox/InputTextBox.h"
    "MainWindow/MainWindow.h"
    "ObjectProperties/ObjectProperties.h"
    "PropertiesWindow/PropertiesWindow.h"
    "SceneExplorer/SceneExplorer.h"
    "SceneExplorerAddButton/SceneExplorerAddButton.h"
    "SceneExplorerItem/SceneExplorerItem.h"
    "SceneExplorerRemoveButton/SceneExplorerRemoveButton.h"
    "SceneVisibilityField/SceneVisibilityField.h"
    "SceneVisibilityWindow/SceneVisibilityWindow.h"
    "SecondaryWindow/SecondaryWindow.h"
    "SingleViewport/SingleViewport.h"
    "UpdateTimeText/UpdateTimeText.h"
    "ValueBox/ValueBox.h"
    "ValueSlider/ValueSlider.h"
    "ViewController/ViewController.h"
)
set(KEDITOR_SOURCES
    "AssetExplorer/AssetExplorer.cpp"
    "AssetExplorerDirectory/AssetExplorerDirectory.cpp"
    "AssetExplorerFile/AssetExplorerFile.cpp"
    "AssetExplorerItem/AssetExplorerItem.cpp"
    "DefaultSceneContext/DefaultSceneContext.cpp"
    "Detachable/Detachable.cpp"
    "GameSpeedButton/GameSpeedButton.cpp"
    "GameStateButton/GameStateButton.cpp"
    "InputTextBox/InputTextBox.cpp"
    "MainWindow/MainWindow.cpp"
    "ObjectProperties/ObjectProperties.cpp"
    "PropertiesWindow/PropertiesWindow.cpp"
    "SceneExplorer/SceneExplorer.cpp"
    "SceneExplorerAddButton/SceneExplorerAddButton.cpp"
    "SceneExplorerItem/SceneExplorerItem.cpp"
    "SceneExplorerRemoveButton/SceneExplorerRemoveButton.cpp"
    "SceneVisibilityField/SceneVisibilityField.cpp"
    "SceneVisibilityWindow/SceneVisibilityWindow.cpp"
    "SecondaryWindow/SecondaryWindow.cpp"
    "SingleViewport/SingleViewport.cpp"
    "UpdateTimeText/UpdateTimeText.cpp"
    "ValueBox/ValueBox.cpp"
    "ValueSlider/ValueSlider.cpp"
    "ViewController/ViewController.cpp"
)

target_sources(KotonoEditor
    PRIVATE ${KEDITOR_HEADERS}
    PRIVATE ${KEDITOR_SOURCES}
)

target_include_directories(KotonoEditor
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoEditor PUBLIC
    KotonoObject
    KotonoInterface
)