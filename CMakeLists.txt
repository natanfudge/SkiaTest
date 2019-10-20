cmake_minimum_required(VERSION 3.15)
project(SkiaTest)

set(CMAKE_CXX_STANDARD 17)

set(SKIA_INCLUDE libs/skia)
set(GLEW_INCLUDE libs/glew/include/GL)
set(GLFW_INCLUDE libs/glfw/include)
set(SDL_INCLUDE libs/sdl/include)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

file(GLOB_RECURSE  SKIA_HEADERS "${SKIA_INCLUDE}/include/*.h")
#file(GLOB SKIA_GPU "libs/skia/include/gpu/*.h")


add_executable(SkiaTest WIN32
        ${SKIA_HEADERS}
        ${SKIA_GPU}
        src/main.cpp

        )
#tools/sk_app/win/main_win.cpp tools/sk_app/win/Window_win.cpp
#        tools/sk_app/Window.cpp tools/sk_app/Window.h

target_include_directories(SkiaTest PRIVATE
        ${SKIA_INCLUDE}
        libs/skia/include/core libs/skia/include/gpu libs/skia/include/config libs/skia/include/utils
        ${CMAKE_SOURCE_DIR}
        ${GLEW_INCLUDE}
        ${SDL_INCLUDE}
        ${GLFW_INCLUDE})

target_link_directories(SkiaTest PRIVATE libs/skia/lib/release
        libs/sdl/lib
        libs/glew/lib
        libs/glfw/lib
        )
target_link_libraries(SkiaTest skia SDL2
        glew32
        glfw3
        opengl32
        )
target_compile_definitions(SkiaTest PUBLIC NOMINMAX)

