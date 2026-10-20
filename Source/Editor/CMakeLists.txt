cmake_minimum_required(VERSION 4.2)
project(KotonoEditor LANGUAGES C CXX)

add_library(KotonoEditor STATIC)

target_compile_features(KotonoEditor PUBLIC cxx_std_23)

set_target_properties(KotonoEditor PROPERTIES FOLDER "KotonoEngine")

set(KEDITOR_PUBLIC
)
set(KEDITOR_PRIVATE
)

include("${CMAKE_CURRENT_SOURCE_DIR}/ObjectClasses.cmake")
list(APPEND KEDITOR_PUBLIC ${CLASS_PUBLIC_HEADERS})
list(APPEND KEDITOR_PRIVATE ${CLASS_PRIVATE_HEADERS} ${CLASS_SOURCES})

target_sources(KotonoEditor
    PRIVATE
        ${KEDITOR_PRIVATE}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KEDITOR_PUBLIC}
)

target_include_directories(KotonoEditor
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoEditor PUBLIC
    KotonoObject
    KotonoInterface
)
