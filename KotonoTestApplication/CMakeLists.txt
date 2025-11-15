cmake_minimum_required(VERSION 3.20)
project(KotonoTestApplication LANGUAGES CXX)

# --- MSVC settings ---
if (MSVC)
    # Security checks + multi-core
    add_compile_options(/sdl)
    add_compile_options(/MP)
endif()

# ---- Create executable ---
add_executable(KotonoTestApplication)

# --- Enable C++20 ---
target_compile_features(KotonoTestApplication PRIVATE cxx_std_20)

# --- Preprocessor defines ---
if (WITH_EDITOR)
    target_compile_definitions(KotonoTestApplication PRIVATE EDITOR)
endif()

# --- Add source files ---
file(GLOB_RECURSE KTESTAPPLICATION_SOURCES "src/*.cpp")
file(GLOB_RECURSE KTESTAPPLICATION_HEADERS "include/kotono_testapplication/*.h")

target_sources(KotonoTestApplication
    PRIVATE ${KTESTAPPLICATION_SOURCES}
    PRIVATE ${KTESTAPPLICATION_HEADERS}
)

# --- Includes ---
target_include_directories(KotonoTestApplication
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_testapplication"
)

# --- Link engine or editor ---
if (WITH_EDITOR)
    target_link_libraries(KotonoTestApplication PRIVATE KotonoEditor)
else()
    target_link_libraries(KotonoTestApplication PRIVATE KotonoEngine)
endif()
