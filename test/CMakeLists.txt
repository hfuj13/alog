cmake_minimum_required(VERSION 3.0.2)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)
#SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
#SET(CMAKE_CXX_COMPILER /usr/bin/clang++-3.8)
add_definitions("-g -Wall -std=c++14 -pthread")
include_directories(..)
include_directories(.)
include_directories(include)

add_executable(
  altest
  src/gtest-all.cc
  main.cc
  alog_test1.cc
  alog_test2.cc
)
target_link_libraries(altest pthread)
