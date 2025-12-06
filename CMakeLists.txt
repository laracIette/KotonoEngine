cmake_minimum_required(VERSION 3.20)
project(KotonoEngine LANGUAGES CXX)

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
add_subdirectory(Common)
add_subdirectory(Generator)
add_subdirectory(IO)
add_subdirectory(Audio)
add_subdirectory(Timing)
add_subdirectory(Platform)
add_subdirectory(Input)
add_subdirectory(Graphics)
add_subdirectory(Interface)
add_subdirectory(Core)
if (WITH_EDITOR)
    add_subdirectory(Editor)
endif()
add_subdirectory(TestApplication)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTY VS_STARTUP_PROJECT KotonoTestApplication
    )
endif()
