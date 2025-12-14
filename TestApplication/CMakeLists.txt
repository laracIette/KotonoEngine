cmake_minimum_required(VERSION 3.20)
project(KotonoTestApplication LANGUAGES CXX)

# --- MSVC settings ---
if (MSVC)
    # Security checks
    add_compile_options(/sdl)
    # Multi-core
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
set(KTESTAPPLICATION_HEADERS
)
set(KTESTAPPLICATION_SOURCES
    "src/main.cpp"
)

target_sources(KotonoTestApplication
    PRIVATE ${KTESTAPPLICATION_HEADERS}
    PRIVATE ${KTESTAPPLICATION_SOURCES}
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
    target_link_libraries(KotonoTestApplication PRIVATE KotonoCore)
endif()

# --- Copy config file ---
add_custom_command(TARGET KotonoTestApplication POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying config.json..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.json"
        "$<TARGET_FILE_DIR:KotonoTestApplication>/config.json"
)
