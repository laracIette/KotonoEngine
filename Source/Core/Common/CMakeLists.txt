cmake_minimum_required(VERSION 4.2)
project(KotonoCommon LANGUAGES C CXX)

add_library(KotonoCommon STATIC)

target_compile_features(KotonoCommon PUBLIC cxx_std_23)

set_target_properties(KotonoCommon PROPERTIES FOLDER "KotonoEngine/Core")

set(KCOMMON_PUBLIC_HEADERS
    "Public/Average.h"
    "Public/Bindable.h"
    "Public/Cached.h"
    "Public/check.h"
    "Public/Clamped.h"
    "Public/conversion_utils.h"
    "Public/enum_utils.h"
    "Public/Handle.h"
    "Public/hash_utils.h"
    "Public/macro_utils.h"
    "Public/Notify.h"
    "Public/properties.h"
    "Public/types.h"
    "Public/Asset/Asset.h"
    "Public/Containers/FixedHeapContainer.h"
    "Public/Containers/Map.h"
    "Public/Containers/Matrix.h"
    "Public/Containers/Pool.h"
    "Public/Containers/Set.h"
    "Public/Event/Delegate.h"
    "Public/Event/Event.h"
    "Public/Logging/log.h"
    "Public/Logging/LogImportance.h"
    "Public/Logging/LogSeverity.h"
    "Public/Path/Path.h"
)
set(KCOMMON_SOURCES
    "Private/Asset/Asset.cpp"
    "Private/Path/Path.cpp"
)

target_sources(KotonoCommon
    PRIVATE
        ${KCOMMON_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KCOMMON_PUBLIC_HEADERS}
)

target_include_directories(KotonoCommon
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_compile_definitions(KotonoCommon PUBLIC
    NOMINMAX
    ENGINE_DIRECTORY="${ENGINE_PATH}"
)
