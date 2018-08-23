cmake_minimum_required(VERSION 3.12)
project(Practice)

set(CMAKE_CXX_STANDARD 14)

file(GLOB UTILITIES util/*.cpp util/*.h)

file(GLOB THREAD
        parallelism/thread_practice.cpp
        parallelism/thread_practice.h)

add_executable(thread_practice ${THREAD} ${UTILITIES})