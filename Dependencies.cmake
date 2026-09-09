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
target_link_libraries(dmagh PRIVATE gaven)


if(DMAGH_WINDOW_API STREQUAL "GLFW")
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.5.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)    
    target_link_libraries(dmagh PRIVATE glfw)
    target_compile_definitions(dmagh PRIVATE DMAGH_WINDOW_GLFW)
endif()


find_package(GLEW REQUIRED)
target_link_libraries(dmagh PRIVATE GLEW::GLEW
                                    opengl32)