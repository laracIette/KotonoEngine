cmake_minimum_required(VERSION 3.20)
project(KotonoAudio LANGUAGES CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoAudio STATIC)

# Require C++20
target_compile_features(KotonoAudio PUBLIC cxx_std_20)

file(GLOB_RECURSE KAUDIO_SOURCES "src/*.cpp")
file(GLOB_RECURSE KAUDIO_HEADERS "include/kotono_audio/*.h")

target_sources(KotonoAudio
    PRIVATE ${KAUDIO_SOURCES}
    PRIVATE ${KAUDIO_HEADERS}
)

# Include directories
target_include_directories(KotonoAudio
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_audio"
)

# Output .lib to KotonoAudio/lib/
set_target_properties(KotonoAudio PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

# Link system + third-party libs
target_link_libraries(KotonoAudio PUBLIC
    "${THIRD_PARTY_DIR}/lib/OpenAL32.lib"
)

# Link libs
target_link_libraries(KotonoAudio PUBLIC
    KotonoCommon
)
