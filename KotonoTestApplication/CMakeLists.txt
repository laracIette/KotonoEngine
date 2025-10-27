cmake_minimum_required(VERSION 3.20)
project(KotonoTestApplication LANGUAGES CXX)

# Enable multi-core compilation on MSVC
if(MSVC)
    add_compile_options(/MP)
endif()

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find sources
file(GLOB_RECURSE APP_SOURCES
    "src/*.cpp"
)

# Create executable
add_executable(KotonoTestApplication ${APP_SOURCES})

# Include directories (if you have headers in the app)
target_include_directories(KotonoTestApplication PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_testapplication"

    # Include KotonoFramework
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoFramework/include"
    # Include KotonoEngine
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoEngine/include"
    # Include KotonoInterface
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoInterface/include"
    # Include KotonoEngineInterface
    "${CMAKE_CURRENT_SOURCE_DIR}/../KotonoEngineInterface/include"
)

# --- Add dependencies as subdirectories ---
# Adjust paths to your folder structure
add_subdirectory("../KotonoFramework" "${CMAKE_CURRENT_BINARY_DIR}/KotonoFramework_build")
add_subdirectory("../KotonoInterface" "${CMAKE_CURRENT_BINARY_DIR}/KotonoInterface_build")
add_subdirectory("../KotonoEngine" "${CMAKE_CURRENT_BINARY_DIR}/KotonoEngine_build")
add_subdirectory("../KotonoEngineInterface" "${CMAKE_CURRENT_BINARY_DIR}/KotonoEngineInterface_build")

# Link the final dependencies
# CMake will automatically propagate include directories and libraries
target_link_libraries(KotonoTestApplication PRIVATE 
    KotonoEngineInterface 
)
