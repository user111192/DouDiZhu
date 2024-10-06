cmake_minimum_required(VERSION 3.27)
project(DouDiZhu)

set(CMAKE_CXX_STANDARD 17)

add_executable(DouDiZhu main.cpp
        Player.cpp
        Player.h
        libs/doudizhu.h)
