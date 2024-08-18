cmake_minimum_required(VERSION 3.00)
project (red-wave-renderer-v2)
include_directories(../../Source/Runtime/)

set(Main ../Math/VectorTests.cpp)

set(Core ../../Source/Runtime/Core/*.cpp)
set(WorkQueue ../../Source/Runtime/WorkQueue/*.cpp)
set(Math ../../Source/Runtime/Math/*.cpp)

file(GLOB_RECURSE Srcs ${Core} ${WorkQueue} ${Math})

add_compile_options(-std=c++20 -Wall)

add_executable(main.out ${Main} ${Srcs})
