cmake_minimum_required(VERSION 3.20)
project(KotonoPlatform LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

include(FetchContent)

##############
# --- glfw ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)

# Build glfw as a static library
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(glfw)
##############

find_package(Vulkan REQUIRED)

# Static library
add_library(KotonoPlatform STATIC)

# Require C++20
target_compile_features(KotonoPlatform PUBLIC cxx_std_20)

set_target_properties(KotonoPlatform PROPERTIES FOLDER "KotonoEngine")

set(KPLATFORM_HEADERS
    "include/kotono_platform/AllocatedBuffer.h"
    "include/kotono_platform/Context.h"
    "include/kotono_platform/QueueFamilyIndices.h"
    "include/kotono_platform/SwapChainSupportDetails.h"
    "include/kotono_platform/vk_utils.h"
    "include/kotono_platform/Window.h"
    "include/kotono_platform/WindowViewport.h"
)
set(KPLATFORM_SOURCES
    "src/Context.cpp"
    "src/vk_utils.cpp"
    "src/Window.cpp"
    "src/WindowViewport.cpp"
)

target_sources(KotonoPlatform
    PRIVATE ${KPLATFORM_HEADERS}
    PRIVATE ${KPLATFORM_SOURCES}
)

set(THIRD_PARTY_DIR "${ENGINE_PATH}/ThirdParty")

# Include directories
target_include_directories(KotonoPlatform
    PUBLIC  "${THIRD_PARTY_DIR}/glm/include"
    PUBLIC  "${THIRD_PARTY_DIR}/vma/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_platform"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoPlatform PUBLIC
    GLM_FORCE_RADIANS
    GLM_FORCE_LEFT_HANDED
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_ENABLE_EXPERIMENTAL
    GLM_FORCE_CXX20
    GLFW_INCLUDE_VULKAN
    GLFW_EXPOSE_NATIVE_WIN32
    VK_USE_PLATFORM_WIN32_KHR
)

# Link system + third-party libs
target_link_libraries(KotonoPlatform PUBLIC
    glfw
    Vulkan::Vulkan
)

# Link libs
target_link_libraries(KotonoPlatform PUBLIC
    KotonoCommon
)
