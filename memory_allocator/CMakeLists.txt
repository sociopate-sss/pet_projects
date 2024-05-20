cmake_minimum_required(VERSION 3.21)

project(heap_allocator)

add_executable(${PROJECT_NAME} test.cpp heap_allocator.cpp)