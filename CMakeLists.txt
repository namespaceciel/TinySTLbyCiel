cmake_minimum_required(VERSION 3.25)
project(TinySTLbyCiel)

set(CMAKE_CXX_STANDARD 17)

add_executable(TinySTLbyCiel allocator.h vector.h array.h iterator.h test/array_test.cpp test.cpp algorithm.h test/vector_test.cpp memory.h)