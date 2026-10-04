cmake_minimum_required(VERSION 4.2)
project(KotonoTiming LANGUAGES C CXX)

add_library(KotonoTiming STATIC)

target_compile_features(KotonoTiming PUBLIC cxx_std_23)

set_target_properties(KotonoTiming PROPERTIES FOLDER "KotonoEngine/Core")

set(KTIMING_HEADERS
    "Clock/Clock.h"
    "Stopwatch/Stopwatch.h"
    "Task/Task.h"
    "TimeContext/TimeContext.h"
    "Timer/Timer.h"
)
set(KTIMING_SOURCES
    "Clock/Clock.cpp"
    "Stopwatch/Stopwatch.cpp"
    "Task/Task.cpp"
    "TimeContext/TimeContext.cpp"
    "Timer/Timer.cpp"
)

target_sources(KotonoTiming
    PRIVATE ${KTIMING_HEADERS}
    PRIVATE ${KTIMING_SOURCES}
)

target_include_directories(KotonoTiming
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(KotonoTiming PUBLIC
    KotonoCommon
)
