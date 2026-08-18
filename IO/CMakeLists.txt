cmake_minimum_required(VERSION 4.2)
project(KotonoIO LANGUAGES C CXX)

add_library(KotonoIO STATIC)

target_compile_features(KotonoIO PUBLIC cxx_std_23)

set_target_properties(KotonoIO PROPERTIES FOLDER "KotonoEngine")

set(KIO_HEADERS
    "include/kotono_io/File.h"
    "include/kotono_io/FileExplorer.h"
    "include/kotono_io/serialize_base.h"
    "include/kotono_io/Serializer.h"
)
set(KIO_SOURCES
    "src/File.cpp"
    "src/FileExplorer.cpp"
    "src/serialize_base.cpp"
    "src/Serializer.cpp"
)

target_sources(KotonoIO
    PRIVATE ${KIO_HEADERS}
    PRIVATE ${KIO_SOURCES}
)

set(THIRD_PARTY_DIR "${ENGINE_PATH}/ThirdParty")

# Include directories
target_include_directories(KotonoIO
    PUBLIC  "${THIRD_PARTY_DIR}/nlohmann/include"
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_io"
)

# Link libs
target_link_libraries(KotonoIO PUBLIC
    KotonoCommon
)
