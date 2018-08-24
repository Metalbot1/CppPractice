cmake_minimum_required(VERSION 3.12)
project(Practice)

set(CMAKE_CXX_STANDARD 14)

file(GLOB UTILITIES util/*.cpp util/*.h common.h)

file(GLOB THREAD
        parallelism/thread_practice.cpp
        parallelism/thread_practice.h)

file(GLOB THREAD_POOL
        parallelism/thread_pool.cpp
        parallelism/thread_pool.h)

file(GLOB THREAD_BANK
        parallelism/threadbank.cpp
        parallelism/threadbank.h)

add_executable(thread_practice ${THREAD} ${UTILITIES})

add_executable(thread_pool ${THREAD_POOL} ${UTILITIES})

add_executable(thread_bank ${THREAD_BANK} ${UTILITIES})