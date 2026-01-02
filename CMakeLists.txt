cmake_minimum_required(VERSION 3.20)
project(KotonoEngine LANGUAGES C CXX)

# --- Global settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Enable multithreaded compilation on MSVC ---
if (MSVC)
    add_compile_options(/MP)
    add_compile_options(/sdl)
endif()

# --- Add all modules (subprojects) ---
add_subdirectory(Common)
add_subdirectory(IO)
add_subdirectory(Reflection)
add_subdirectory(Generator)
add_subdirectory(Audio)
add_subdirectory(Timing)
add_subdirectory(Platform)
add_subdirectory(Input)
add_subdirectory(ThirdParty/SPIRV-Reflect)
add_subdirectory(Graphics)
add_subdirectory(Interface)
add_subdirectory(Core)
if (WITH_EDITOR)
    add_subdirectory(Editor)
endif()
