cmake_minimum_required(VERSION 4.2)
project(KotonoTiming LANGUAGES C CXX)

if (MSVC)
    # Enable SDL security checks
    add_compile_options(/sdl)
endif()

# Static library
add_library(KotonoTiming STATIC)

# Require C++20
target_compile_features(KotonoTiming PUBLIC cxx_std_23)

set_target_properties(KotonoTiming PROPERTIES FOLDER "KotonoEngine")

set(KTIMING_HEADERS
    "include/kotono_timing/Clock.h"
    "include/kotono_timing/Stopwatch.h"
    "include/kotono_timing/TimeContext.h"
    "include/kotono_timing/Timer.h"
    "include/kotono_timing/TimerManager.h"
)
set(KTIMING_SOURCES
    "src/Clock.cpp"
    "src/Stopwatch.cpp"
    "src/TimeContext.cpp"
    "src/Timer.cpp"
    "src/TimerManager.cpp"
)

target_sources(KotonoTiming
    PRIVATE ${KTIMING_HEADERS}
    PRIVATE ${KTIMING_SOURCES}
)

# Include directories
target_include_directories(KotonoTiming
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/kotono_timing"
)

# Link libs
target_link_libraries(KotonoTiming PUBLIC
    KotonoCommon
)
