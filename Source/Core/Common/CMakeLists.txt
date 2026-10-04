cmake_minimum_required(VERSION 4.2)
project(KotonoCommon LANGUAGES C CXX)

add_library(KotonoCommon STATIC)

target_compile_features(KotonoCommon PUBLIC cxx_std_23)

set_target_properties(KotonoCommon PROPERTIES FOLDER "KotonoEngine/Core")

set(KCOMMON_HEADERS
    "Average.h"
    "Bindable.h"
    "Cached.h"
    "Clamped.h"
    "conversion_utils.h"
    "enum_utils.h"
    "Handle.h"
    "hash_utils.h"
    "Notify.h"
    "types.h"
    "Asset/Asset.h"
    "Containers/FixedHeapContainer.h"
    "Containers/Matrix.h"
    "Containers/Pool.h"
    "Containers/Set.h"
    "Event/Delegate.h"
    "Event/Event.h"
    "Logging/log.h"
    "Logging/LogImportanceLevel.h"
    "Path/Path.h"
    "Path/PathManager.h"
)
set(KCOMMON_SOURCES
    "Asset/Asset.cpp"
    "Path/Path.cpp"
    "Path/PathManager.cpp"
)

target_sources(KotonoCommon
    PRIVATE ${KCOMMON_HEADERS}
    PRIVATE ${KCOMMON_SOURCES}
)

target_include_directories(KotonoCommon
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(KotonoCommon PUBLIC
    NOMINMAX
    ENGINE_DIRECTORY="${ENGINE_PATH}"
)
