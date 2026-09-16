cmake_minimum_required(VERSION 4.2)
project(dr_libs LANGUAGES C)

add_library(dr_libs STATIC)

target_compile_features(dr_libs PRIVATE c_std_11)

set_target_properties(dr_libs PROPERTIES FOLDER "KotonoEngine/ThirdParty")

set(DRLIBS_HEADERS
    "include/dr_libs/dr_flac.h"
    "include/dr_libs/dr_mp3.h"
    "include/dr_libs/dr_wav.h"
)
set(DRLIBS_SOURCES
    "src/dr_impl.c"
)

target_sources(dr_libs
    PRIVATE ${DRLIBS_HEADERS}
    PRIVATE ${DRLIBS_SOURCES}
)

target_include_directories(dr_libs
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/dr_libs"
)
