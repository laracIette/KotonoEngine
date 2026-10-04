cmake_minimum_required(VERSION 4.2)
project(KotonoPlatform LANGUAGES C CXX)

##############
include(FetchContent)

# --- glfw ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)

# Build glfw as a static library
set(BUILD_SHARED_LIBS   OFF CACHE INTERNAL "")

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "")
set(GLFW_INSTALL        OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)

foreach(glfw_target
    glfw
    update_mappings
)
    if(TARGET ${glfw_target})
        set_target_properties(${glfw_target} PROPERTIES FOLDER "KotonoEngine/ThirdParty/GLFW3")
    endif()
endforeach()
##############

find_package(Vulkan 1.4 REQUIRED)

add_library(KotonoPlatform STATIC)

target_compile_features(KotonoPlatform PUBLIC cxx_std_23)

set_target_properties(KotonoPlatform PROPERTIES FOLDER "KotonoEngine/Core")

set(KPLATFORM_PUBLIC_HEADERS
    "Public/AllocatedBuffer.h"
    "Public/AllocatedImage.h"
    "Public/glm_utils.h"
    "Public/QueueFamilyIndices.h"
    "Public/SwapchainSupportDetails.h"
    "Public/Context/Context.h"
    "Public/Device/Device.h"
    "Public/Surface/Surface.h"
    "Public/Swapchain/Swapchain.h"
    "Public/vk_utils/vk_utils.h"
    "Public/Window/Window.h"
)
set(KPLATFORM_SOURCES
    "Private/Context/Context.cpp"
    "Private/Device/Device.cpp"
    "Private/Surface/Surface.cpp"
    "Private/Swapchain/Swapchain.cpp"
    "Private/vk_utils/vk_utils.cpp"
    "Private/Window/Window.cpp"
)

target_sources(KotonoPlatform
    PUBLIC ${KPLATFORM_PUBLIC_HEADERS}
    PRIVATE ${KPLATFORM_SOURCES}
)

target_include_directories(KotonoPlatform
    PUBLIC "${ENGINE_PATH}/Source/ThirdParty/vma/include"
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

# Preprocessor defines for all builds
target_compile_definitions(KotonoPlatform PUBLIC
    GLFW_INCLUDE_VULKAN
    GLFW_EXPOSE_NATIVE_WIN32
    VK_USE_PLATFORM_WIN32_KHR
)

target_link_libraries(KotonoPlatform PUBLIC
    glfw
    Vulkan::Vulkan
)

target_link_libraries(KotonoPlatform PUBLIC
    KotonoCommon
    KotonoMath
)
