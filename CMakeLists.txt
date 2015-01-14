cmake_minimum_required(VERSION 2.8)

set(NAME dungeon-time)

project(${NAME})

file (GLOB SOURCES *.cpp)

add_definitions(-O3 -std=c++1y)
add_executable(${NAME} ${SOURCES})

