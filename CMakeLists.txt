cmake_minimum_required(VERSION 3.20)
project(Kotono LANGUAGES CXX)

# --- Global settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Enable multithreaded compilation on MSVC ---
if (MSVC)
    add_compile_options(/MP)
    add_compile_options(/sdl)
endif()

# --- Editor option (propagates to child targets) ---
option(WITH_EDITOR "Build the Kotono Editor" ON)

# --- Add all modules (subprojects) ---
# Order matters!
add_subdirectory(KotonoFramework)
add_subdirectory(KotonoGenerator)
add_subdirectory(KotonoInterface)
add_subdirectory(KotonoEngine)
if (WITH_EDITOR)
    add_subdirectory(KotonoEditor)
endif()
add_subdirectory(KotonoTestApplication)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTY VS_STARTUP_PROJECT KotonoTestApplication
    )
endif()
