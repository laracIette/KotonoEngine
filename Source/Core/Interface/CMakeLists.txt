cmake_minimum_required(VERSION 4.2)
project(KotonoInterface LANGUAGES C CXX)

add_library(KotonoInterface STATIC)

target_compile_features(KotonoInterface PUBLIC cxx_std_23)

set_target_properties(KotonoInterface PROPERTIES FOLDER "KotonoEngine/Core")

set(KINTERFACE_PUBLIC
    "Public/core_widgets.h"
)
set(KINTERFACE_PRIVATE
)

include("${CMAKE_CURRENT_SOURCE_DIR}/ObjectClasses.cmake")
list(APPEND KINTERFACE_PUBLIC ${CLASS_PUBLIC_HEADERS})
list(APPEND KINTERFACE_PRIVATE ${CLASS_PRIVATE_HEADERS} ${CLASS_SOURCES})

target_sources(KotonoInterface
    PRIVATE
        ${KINTERFACE_PRIVATE}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS Public
        FILES ${KINTERFACE_PUBLIC}
)

target_include_directories(KotonoInterface
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoInterface PUBLIC
    KotonoObject
)
