include(FetchContent)
FetchContent_Declare(
    gaven
    GIT_REPOSITORY https://github.com/iGhaithSquare/Gaven
    GIT_TAG 43b6c312481dff6863be17c228a6e2fa41272f46
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gaven)
target_compile_options(gaven PRIVATE
    -w
)


FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.5.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)    


find_package(GLEW REQUIRED)