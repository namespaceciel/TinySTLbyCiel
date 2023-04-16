cmake_minimum_required(VERSION 3.25)
project(TinySTLbyCiel)

set(CMAKE_CXX_STANDARD 17)

add_executable(TinySTLbyCiel include/allocator.h include/vector.h include/array.h include/iterator.h test/array_test.cpp main.cpp include/algorithm.h test/vector_test.cpp include/memory.h)