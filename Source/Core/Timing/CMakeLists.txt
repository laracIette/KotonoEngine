cmake_minimum_required(VERSION 4.2)
project(KotonoTiming LANGUAGES C CXX)

add_library(KotonoTiming STATIC)

target_compile_features(KotonoTiming PUBLIC cxx_std_23)

set_target_properties(KotonoTiming PROPERTIES FOLDER "KotonoEngine/Core")

set(KTIMING_PUBLIC_HEADERS
    "Public/Clock/Clock.h"
    "Public/Stopwatch/Stopwatch.h"
    "Public/Task/Task.h"
    "Public/TimeContext/TimeContext.h"
    "Public/Timer/Timer.h"
)
set(KTIMING_SOURCES
    "Private/Clock/Clock.cpp"
    "Private/Stopwatch/Stopwatch.cpp"
    "Private/Task/Task.cpp"
    "Private/TimeContext/TimeContext.cpp"
    "Private/Timer/Timer.cpp"
)

target_sources(KotonoTiming
    PUBLIC ${KTIMING_PUBLIC_HEADERS}
    PRIVATE ${KTIMING_SOURCES}
)

target_include_directories(KotonoTiming
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_link_libraries(KotonoTiming PUBLIC
    KotonoCommon
)
