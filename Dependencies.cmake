include(FetchContent)
FetchContent_Declare(
    cjson
    GIT_REPOSITORY https://github.com/davegamble/cjson.git
    GIT_TAG v1.7.19
)
FetchContent_MakeAvailable(cjson)
target_compile_options(cjson PRIVATE
    -w
)
FetchContent_GetProperties(cjson)


FetchContent_Declare(
    gaven
    GIT_REPOSITORY https://github.com/iGhaithSquare/Gaven
    GIT_TAG a8ff51f2bfa2f6620c694074a29d3ce114ec1454
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
elseif(DMAGH_WINDOW_API STREQUAL "ANDROID")
    target_compile_definitions(dmagh PRIVATE DMAGH_WINDOW_ANDROID)
    target_link_libraries(dmagh PRIVATE android log)
    set(ANDROID_GLUE ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue)
    target_sources(dmagh PRIVATE ${ANDROID_GLUE}/android_native_app_glue.c)
    target_include_directories(dmagh PRIVATE ${ANDROID_GLUE})
endif()

if(DMAGH_RENDERING_API STREQUAL "OpenGl3_3")
    target_compile_definitions(dmagh PRIVATE DMAGH_RENDERER_OPENGL3_3)
    find_package(GLEW REQUIRED)
    target_link_libraries(dmagh PRIVATE GLEW::GLEW
                                        opengl32)
elseif(DMAGH_RENDERING_API STREQUAL "OpenGlEs3_2")
    target_compile_definitions(dmagh PRIVATE DMAGH_RENDERER_OPENGLES3_2)
    target_link_libraries(dmagh PRIVATE EGL
                                        GLESv3)
endif()





FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm
    GIT_TAG v0.9.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cglm)    
target_link_libraries(dmagh PRIVATE cglm)