cmake_minimum_required(VERSION 4.2)
project(KotonoEditor LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
endif()

# Static library
add_library(KotonoEditor STATIC)

target_compile_features(KotonoEditor PUBLIC cxx_std_23)

set_target_properties(KotonoEditor PROPERTIES FOLDER "KotonoEngine")

set(KEDITOR_HEADERS
    "include/kotono_editor/AssetExplorer.h"
    "include/kotono_editor/AssetExplorerDirectory.h"
    "include/kotono_editor/AssetExplorerFile.h"
    "include/kotono_editor/AssetExplorerItem.h"
    "include/kotono_editor/Camera.h"
    "include/kotono_editor/DrawTimeText.h"
    "include/kotono_editor/GameStateButton.h"
    "include/kotono_editor/InputTextBox.h"
    "include/kotono_editor/MainWindow.h"
    "include/kotono_editor/ObjectProperties.h"
    "include/kotono_editor/PropertiesWindow.h"
    "include/kotono_editor/SceneExplorer.h"
    "include/kotono_editor/SceneExplorerAddButton.h"
    "include/kotono_editor/SceneExplorerItem.h"
    "include/kotono_editor/SceneExplorerRemoveButton.h"
    "include/kotono_editor/UpdateTimeText.h"
    "include/kotono_editor/VisualizationField.h"
    "include/kotono_editor/ValueBox.h"
    "include/kotono_editor/ValueSlider.h"
    "include/kotono_editor/ValueSliderFloat.h"
    "include/kotono_editor/Visualizer.h"
    "include/kotono_editor/VisualizerWindow.h"
    "include/kotono_editor/VisualizerWindowItem.h"
)
set(KEDITOR_SOURCES
    "src/AssetExplorer.cpp"
    "src/AssetExplorerDirectory.cpp"
    "src/AssetExplorerFile.cpp"
    "src/AssetExplorerItem.cpp"
    "src/Camera.cpp"
    "src/DrawTimeText.cpp"
    "src/GameStateButton.cpp"
    "src/InputTextBox.cpp"
    "src/MainWindow.cpp"
    "src/ObjectProperties.cpp"
    "src/PropertiesWindow.cpp"
    "src/SceneExplorer.cpp"
    "src/SceneExplorerAddButton.cpp"
    "src/SceneExplorerItem.cpp"
    "src/SceneExplorerRemoveButton.cpp"
    "src/UpdateTimeText.cpp"
    "src/ValueBox.cpp"
    "src/ValueSlider.cpp"
    "src/ValueSliderFloat.cpp"
    "src/Visualizer.cpp"
    "src/VisualizerWindow.cpp"
    "src/VisualizerWindowItem.cpp"
)

target_sources(KotonoEditor
    PRIVATE ${KEDITOR_HEADERS}
    PRIVATE ${KEDITOR_SOURCES}
)

# Include directories
target_include_directories(KotonoEditor
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_editor"
)

# Link libs
target_link_libraries(KotonoEditor PUBLIC
    KotonoCore
    KotonoInterface
)