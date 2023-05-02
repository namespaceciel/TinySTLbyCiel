cmake_minimum_required(VERSION 3.25)
project(TinySTLbyCiel)

set(CMAKE_CXX_STANDARD 23)

file(GLOB TinySTLbyCiel_SRC "test/*.cpp")

add_executable(TinySTLbyCiel main.cpp ${TinySTLbyCiel_SRC})

target_include_directories(TinySTLbyCiel PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)