cmake_minimum_required(VERSION 3.00)
project (red-wave-renderer-v2)
include_directories(../../Source/Runtime/)

set(Main ../WorkQueue/WorkQueueTests.cpp)

set(Core ../../Source/Runtime/Core/*.cpp)
set(WorkQueue ../../Source/Runtime/WorkQueue/*.cpp)

file(GLOB_RECURSE Srcs ${Core} ${WorkQueue})

add_compile_options(-std=c++20 -Wall)
set(EXECUTABLE_OUTPUT_PATH ./Build)

add_executable(main.out ${Main} ${Srcs})
