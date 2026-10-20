cmake_minimum_required(VERSION 4.2)
project(KotonoScene LANGUAGES C CXX)

add_library(KotonoScene STATIC)

target_compile_features(KotonoScene PUBLIC cxx_std_23)

set_target_properties(KotonoScene PROPERTIES FOLDER "KotonoEngine/Core")

set(KSCENE_PUBLIC
)
set(KSCENE_PRIVATE
)

include("${CMAKE_CURRENT_SOURCE_DIR}/ObjectClasses.cmake")
list(APPEND KSCENE_PUBLIC ${CLASS_PUBLIC_HEADERS})
list(APPEND KSCENE_PRIVATE ${CLASS_PRIVATE_HEADERS} ${CLASS_SOURCES})

target_sources(KotonoScene
    PRIVATE
        ${KSCENE_PRIVATE}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KSCENE_PUBLIC}
)

target_include_directories(KotonoScene
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoScene PUBLIC
    KotonoObject
)
