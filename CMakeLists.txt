cmake_minimum_required(VERSION 3.10)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
project(dmagh)

include(FetchContent)
FetchContent_Declare(
    gaven
    GIT_REPOSITORY https://github.com/iGhaithSquare/Gaven
    GIT_TAG 43b6c312481dff6863be17c228a6e2fa41272f46
)
FetchContent_MakeAvailable(gaven)
target_compile_options(gaven PRIVATE
    -w
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/$<CONFIG>/)
add_executable(dmagh)

target_sources(dmagh PRIVATE ${SOURCES})
target_link_libraries(dmagh PRIVATE gaven)