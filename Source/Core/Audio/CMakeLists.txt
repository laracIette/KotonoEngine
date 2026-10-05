cmake_minimum_required(VERSION 4.2)
project(KotonoAudio LANGUAGES C CXX)

################
include(FetchContent)

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
        set_target_properties(${al_target} PROPERTIES FOLDER "KotonoEngine/ThirdParty/OpenAL")
    endif()
endforeach()
################

add_library(KotonoAudio STATIC)

target_compile_features(KotonoAudio PUBLIC cxx_std_23)

set_target_properties(KotonoAudio PROPERTIES FOLDER "KotonoEngine/Core")

set(KAUDIO_PUBLIC_HEADERS
    "Public/AudioContext/AudioContext.h"
    "Public/AudioSource/AudioSource.h"
)
set(KAUDIO_PRIVATE_HEADERS
    "Private/al_utils/al_utils.h"
    "Private/AudioDataWAV/AudioDataWAV.h"
)
set(KAUDIO_SOURCES
    "Private/al_utils/al_utils.cpp"
    "Private/AudioContext/AudioContext.cpp"
    "Private/AudioDataWAV/AudioDataWAV.cpp"
    "Private/AudioSource/AudioSource.cpp"
)

target_sources(KotonoAudio
    PRIVATE
        ${KAUDIO_SOURCES}
        ${KAUDIO_PRIVATE_HEADERS}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KAUDIO_PUBLIC_HEADERS}
)

target_include_directories(KotonoAudio
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_compile_definitions(KotonoAudio PUBLIC
    AL_LIBTYPE_STATIC
)

target_link_libraries(KotonoAudio PRIVATE
    OpenAL::OpenAL
    dr_libs
)

target_link_libraries(KotonoAudio PUBLIC
    KotonoCommon
    KotonoMath
)
