cmake_minimum_required(VERSION 4.2)
project(KotonoIO LANGUAGES C CXX)

add_library(KotonoIO STATIC)

target_compile_features(KotonoIO PUBLIC cxx_std_23)

set_target_properties(KotonoIO PROPERTIES FOLDER "KotonoEngine/Core")

set(KIO_PUBLIC_HEADERS
    "Public/File/File.h"
    "Public/FileExplorer/FileExplorer.h"
    "Public/serialize_base/serialize_base.h"
    "Public/Serializer/Serializer.h"
)
set(KIO_SOURCES
    "Private/File/File.cpp"
    "Private/FileExplorer/FileExplorer.cpp"
    "Private/serialize_base/serialize_base.cpp"
    "Private/Serializer/Serializer.cpp"
)

target_sources(KotonoIO
    PRIVATE
        ${KIO_SOURCES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KIO_PUBLIC_HEADERS}
)

target_include_directories(KotonoIO
    PUBLIC "${ENGINE_PATH}/Source/ThirdParty/nlohmann/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoIO PUBLIC
    KotonoCommon
)
