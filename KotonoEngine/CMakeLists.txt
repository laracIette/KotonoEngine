cmake_minimum_required(VERSION 3.20)
project(KotonoEngine LANGUAGES CXX)

# Enable multi-core compilation on MSVC
if(MSVC)
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoEngine STATIC)

# Require C++20
target_compile_features(KotonoEngine PUBLIC cxx_std_20)

# Source files
file(GLOB_RECURSE KENGINE_SOURCES
    "src/*.cpp"
)
target_sources(KotonoEngine PRIVATE ${KENGINE_SOURCES})

# Include directories
target_include_directories(KotonoEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_engine"

    # Include KotonoFramework
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoFramework/include"
)

# Output .lib to KotonoEngine/lib/
set_target_properties(KotonoEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Preprocessor defines
target_compile_definitions(KotonoEngine PUBLIC
    NOMINMAX
    EDITOR
)

# Link libs
target_link_libraries(KotonoEngine PUBLIC 
    KotonoFramework
)