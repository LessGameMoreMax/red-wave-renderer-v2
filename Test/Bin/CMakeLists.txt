cmake_minimum_required(VERSION 3.00)
project (red-wave-renderer-v2)
find_package(glfw3 REQUIRED)
include_directories(../../Source/Runtime/ ../../Source/Runtime/Renderer/Includes)

set(Main ../Renderer/main.cpp)

set(Core ../../Source/Runtime/Core/*.cpp)
set(WorkQueue ../../Source/Runtime/WorkQueue/*.cpp)
set(Math ../../Source/Runtime/Math/*.cpp)
set(RendererCpp ../../Source/Runtime/Renderer/*.cpp)
set(RendererC ../../Source/Runtime/Renderer/*.c)

file(GLOB_RECURSE Srcs ${Core} ${WorkQueue} ${Math} ${RendererCpp} ${RendererC})

add_compile_options(-std=c++20 -Wall)

add_executable(main.out ${Main} ${Srcs})

target_link_libraries(main.out
    -lglfw
    -lGL
    -lm
    -lXrandr
    -lXi
    -lX11
    -lXxf86vm
    -lpthread
    -ldl
    -lXinerama
    -lXcursor
    -lassimp)
