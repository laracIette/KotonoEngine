cmake_minimum_required(VERSION 4.2)
project(KotonoEngine LANGUAGES C CXX)

add_subdirectory(ThirdParty/dr_libs)

add_subdirectory(Core/Common)
add_subdirectory(Core/Math)
add_subdirectory(Core/IO)
add_subdirectory(Core/Reflection)
add_subdirectory(Core/Audio)
add_subdirectory(Core/Timing)
add_subdirectory(Core/Platform)
add_subdirectory(Core/Input)
add_subdirectory(Core/Graphics)
add_subdirectory(Core/Rendering)

add_subdirectory(Generator)

add_subdirectory(Core/Object)
add_subdirectory(Core/Interface)
add_subdirectory(Core/Scene)

if (WITH_EDITOR)
    add_subdirectory(Editor)
endif()
add_subdirectory(Application)
