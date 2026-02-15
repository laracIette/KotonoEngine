cmake_minimum_required(VERSION 3.20)
project(KotonoAudio LANGUAGES C CXX)

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

set(LIBTYPE "STATIC" CACHE STRING "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)

set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)

if (WIN32)
    set(ALSOFT_BACKEND_ALSA OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_OSS OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_SOLARIS OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_SNDIO OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_PORTAUDIO OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_PULSEAUDIO OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_JACK OFF CACHE BOOL "" FORCE)
elseif (LINUX)
    set(ALSOFT_BACKEND_DSOUND OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_WASAPI OFF CACHE BOOL "" FORCE)
    set(ALSOFT_BACKEND_WINMM OFF CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(OpenAL)

foreach(al_target
    OpenAL
    alsoft.common
    alsoft.excommon
    alsoft.fmt
    alsoft.build_version
    clang-tidy-check
)
    if(TARGET ${al_target})
        set_target_properties(${al_target} PROPERTIES FOLDER "ThirdParty/OpenAL")
    endif()
endforeach()
################

# Static library
add_library(KotonoAudio STATIC)

# Require C++20
target_compile_features(KotonoAudio PUBLIC cxx_std_23)

set_target_properties(KotonoAudio PROPERTIES FOLDER "KotonoEngine")

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

# Preprocessor defines for all builds
target_compile_definitions(KotonoAudio PUBLIC
    AL_LIBTYPE_STATIC
)

# Link system + third-party libs
target_link_libraries(KotonoAudio PRIVATE
    OpenAL::OpenAL
)

# Link libs
target_link_libraries(KotonoAudio PUBLIC
    KotonoCommon
)
