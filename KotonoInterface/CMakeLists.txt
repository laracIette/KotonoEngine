cmake_minimum_required(VERSION 3.20)
project(KotonoInterface LANGUAGES CXX)

# Enable multi-core compilation on MSVC
if(MSVC)
    add_compile_options(/MP)
endif()

# Static library
add_library(KotonoInterface STATIC)

# Require C++20
target_compile_features(KotonoInterface PUBLIC cxx_std_20)

# Source files
file(GLOB_RECURSE KINTERFACE_SOURCES
    "src/*.cpp"
)
target_sources(KotonoInterface PRIVATE ${KINTERFACE_SOURCES})

# Include directories
target_include_directories(KotonoInterface PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_interface"

    # Include KotonoFramework
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoFramework/include"
)

# Output .lib to KotonoInterface/lib/
set_target_properties(KotonoInterface PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Preprocessor defines
target_compile_definitions(KotonoInterface PUBLIC
    NOMINMAX
    EDITOR
)

# Link libs
target_link_libraries(KotonoInterface PUBLIC 
    KotonoFramework
)
