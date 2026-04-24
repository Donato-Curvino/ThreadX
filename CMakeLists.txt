cmake_minimum_required(VERSION 4.1)
project(ThreadX)

set(CMAKE_CXX_STANDARD 20)

#add_library(ThreadX Lockable.h)

add_executable(tests main.cpp Lockable.h
        Waitable.h)
