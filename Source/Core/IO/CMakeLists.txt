cmake_minimum_required(VERSION 4.2)
project(KotonoIO LANGUAGES C CXX)

add_library(KotonoIO STATIC)

target_compile_features(KotonoIO PUBLIC cxx_std_23)

set_target_properties(KotonoIO PROPERTIES FOLDER "KotonoEngine/Core")

set(KIO_HEADERS
    "File/File.h"
    "FileExplorer/FileExplorer.h"
    "serialize_base/serialize_base.h"
    "Serializer/Serializer.h"
)
set(KIO_SOURCES
    "File/File.cpp"
    "FileExplorer/FileExplorer.cpp"
    "serialize_base/serialize_base.cpp"
    "Serializer/Serializer.cpp"
)

target_sources(KotonoIO
    PRIVATE ${KIO_HEADERS}
    PRIVATE ${KIO_SOURCES}
)

target_include_directories(KotonoIO
    PUBLIC  "${ENGINE_PATH}/Source/ThirdParty/nlohmann/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoIO PUBLIC
    KotonoCommon
)
