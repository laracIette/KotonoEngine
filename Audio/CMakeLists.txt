cmake_minimum_required(VERSION 3.20)
project(KotonoAudio LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

include(FetchContent)

################
# --- OpenAL ---
FetchContent_Declare(
    OpenAL
    GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG        1.24.3
    GIT_SHALLOW    TRUE
)

# Tell OpenAL Soft to not try to install itself globally
set(CMAKE_INSTALL_DO_NOT_USE_INSTALL_NAME_DIR YES)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_TESTS OFF)

FetchContent_MakeAvailable(OpenAL)
################

# Static library
add_library(KotonoAudio STATIC)

# Require C++20
target_compile_features(KotonoAudio PUBLIC cxx_std_20)

set(KAUDIO_HEADERS
    "include/kotono_audio/AudioManager.h"
)
set(KAUDIO_SOURCES
    "src/AudioManager.cpp"
)

target_sources(KotonoAudio
    PRIVATE ${KAUDIO_HEADERS}
    PRIVATE ${KAUDIO_SOURCES}
)

# Include directories
target_include_directories(KotonoAudio
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_audio"
)

# Link system + third-party libs
target_link_libraries(KotonoAudio PUBLIC
    OpenAL::OpenAL
)

# Link libs
target_link_libraries(KotonoAudio PUBLIC
    KotonoCommon
)
