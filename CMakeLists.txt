cmake_minimum_required(VERSION 3.10)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
project(dmagh)


set(DMAGH_WINDOW_API "GLFW" CACHE STRING "Window Api to Use")
set_property(CACHE DMAGH_WINDOW_API PROPERTY STRINGS
    GLFW
    ANDROID)

set(DMAGH_RENDERING_API "OpenGl3_3" CACHE STRING "Rendering API to Use")
set_property(CACHE DMAGH_RENDERING_API PROPERTY STRINGS
    OpenGl3_3
    OpenGlEs3_2)

if (ANDROID)
    add_library(dmagh SHARED)
    set(DMAGH_WINDOW_API "ANDROID")
    set(DMAGH_RENDERING_API "OpenGlEs3_2")
else ()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/)
    add_executable(dmagh)
    add_custom_command(TARGET dmagh POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:dmagh> $<TARGET_RUNTIME_DLLS:dmagh>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/GAVEN/libgaven.dll" "${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/"
        COMMAND_EXPAND_LISTS
    )
endif()
target_sources(dmagh PRIVATE ${SOURCES})

include(Dependencies.cmake)