cmake_minimum_required(VERSION 3.20)
project(KotonoTestApplication LANGUAGES CXX)

if(MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)

    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE KTESTAPPLICATION_SOURCES "src/*.cpp")
file(GLOB_RECURSE KTESTAPPLICATION_HEADERS "include/kotono_testapplication/*.h")

# Create executable
add_executable(KotonoTestApplication ${KTESTAPPLICATION_SOURCES})

target_sources(KotonoTestApplication 
    PRIVATE ${KTESTAPPLICATION_SOURCES}
    PRIVATE ${KTESTAPPLICATION_HEADERS} 
)

# Include directories (if you have headers in the app)
target_include_directories(KotonoTestApplication
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_testapplication"
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

# Set the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KotonoTestApplication)
