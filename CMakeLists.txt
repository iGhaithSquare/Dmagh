cmake_minimum_required(VERSION 3.10)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
project(dmagh)
include(Dependencies.cmake)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/)
add_executable(dmagh)
target_link_libraries(dmagh PRIVATE gaven
                                    glfw
                                    GLEW::GLEW
                                    opengl32)
target_sources(dmagh PRIVATE ${SOURCES})


add_custom_command(TARGET dmagh POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:dmagh> $<TARGET_RUNTIME_DLLS:dmagh>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/GAVEN/libgaven.dll" "${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/"
    COMMAND_EXPAND_LISTS
)