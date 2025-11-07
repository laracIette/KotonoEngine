cmake_minimum_required(VERSION 3.20)
project(KotonoTestApplication LANGUAGES CXX)

option(WITH_EDITOR "Build with editor features" ON)

if(MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)

    # Enable multi-core compilation
    add_compile_options(/MP)
endif()

file(GLOB_RECURSE KTESTAPPLICATION_SOURCES "src/*.cpp")
file(GLOB_RECURSE KTESTAPPLICATION_HEADERS "include/kotono_testapplication/*.h")

# Create executable
add_executable(KotonoTestApplication ${KTESTAPPLICATION_SOURCES})

# Preprocessor defines
if (WITH_EDITOR)
	target_compile_definitions(KotonoTestApplication PRIVATE EDITOR)
endif()

target_sources(KotonoTestApplication 
    PRIVATE ${KTESTAPPLICATION_SOURCES}
    PRIVATE ${KTESTAPPLICATION_HEADERS} 
)

# Include directories (if you have headers in the app)
target_include_directories(KotonoTestApplication
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_testapplication"
)

# Add the generator
add_subdirectory("../KotonoGenerator" "${CMAKE_CURRENT_BINARY_DIR}/KotonoGenerator_build")

# --- Create a custom target to run the generator ---
add_custom_target(RunKotonoGenerator
    COMMAND $<TARGET_FILE:KotonoGenerator>
    DEPENDS KotonoGenerator
    COMMENT "Running KotonoGenerator..."
)

# --- Add dependencies as subdirectories ---
# Adjust paths to your folder structure
add_subdirectory("../KotonoFramework" "${CMAKE_CURRENT_BINARY_DIR}/KotonoFramework_build")
add_subdirectory("../KotonoInterface" "${CMAKE_CURRENT_BINARY_DIR}/KotonoInterface_build")
add_subdirectory("../KotonoEngine" "${CMAKE_CURRENT_BINARY_DIR}/KotonoEngine_build")
if (WITH_EDITOR)
	add_subdirectory("../KotonoEditor" "${CMAKE_CURRENT_BINARY_DIR}/KotonoEditor_build")
endif()

# --- Make everything that uses generated files depend on the generator ---
add_dependencies(KotonoEngine RunKotonoGenerator)

# Link the final dependencies
# CMake will automatically propagate include directories and libraries
if (WITH_EDITOR)
    target_link_libraries(KotonoTestApplication PRIVATE KotonoEditor)
else()
    target_link_libraries(KotonoTestApplication PRIVATE KotonoEngine)
endif()

# Set the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KotonoTestApplication)
