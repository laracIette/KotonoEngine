cmake_minimum_required(VERSION 3.20)
project(KotonoCommon LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoCommon STATIC)

# Require C++20
target_compile_features(KotonoCommon PUBLIC cxx_std_20)

set(KCOMMON_HEADERS
    "include/kotono_common/Average.h"
    "include/kotono_common/bitwise_utils.h"
    "include/kotono_common/Buildable.h"
    "include/kotono_common/Cached.h"
    "include/kotono_common/Collection.h"
    "include/kotono_common/Delegate.h"
    "include/kotono_common/Event.h"
    "include/kotono_common/hash_utils.h"
    "include/kotono_common/log.h"
    "include/kotono_common/LogImportanceLevel.h"
    "include/kotono_common/Path.h"
    "include/kotono_common/Pool.h"
)
set(KCOMMON_SOURCES
    "src/hash_utils.cpp"
    "src/Path.cpp"
)

target_sources(KotonoCommon
    PRIVATE ${KCOMMON_HEADERS}
    PRIVATE ${KCOMMON_SOURCES}
)

# Include directories
target_include_directories(KotonoCommon
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_common"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoCommon PUBLIC
    NOMINMAX
    ROOT_DIRECTORY="${CMAKE_SOURCE_DIR}"
)
