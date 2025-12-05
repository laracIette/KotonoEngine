cmake_minimum_required(VERSION 3.20)
project(KotonoPlatform LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoPlatform STATIC)

# Require C++20
target_compile_features(KotonoPlatform PUBLIC cxx_std_20)

file(GLOB_RECURSE KPLATFORM_SOURCES "src/*.cpp")
file(GLOB_RECURSE KPLATFORM_HEADERS "include/kotono_platform/*.h")

target_sources(KotonoPlatform
    PRIVATE ${KPLATFORM_SOURCES}
    PRIVATE ${KPLATFORM_HEADERS}
)

# Include directories
target_include_directories(KotonoPlatform
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_platform"
)

# Output .lib to KotonoPlatform/lib/
set_target_properties(KotonoPlatform PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
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

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

# Link system + third-party libs
target_link_libraries(KotonoPlatform PUBLIC
    "${THIRD_PARTY_DIR}/lib/vulkan-1.lib"
    "${THIRD_PARTY_DIR}/lib/glfw3.lib"
)

# Link libs
target_link_libraries(KotonoPlatform PUBLIC
    KotonoCommon
)
