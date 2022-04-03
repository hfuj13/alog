cmake_minimum_required(VERSION 3.0.2)
project(altest CXX)

#add_library(pthread)
add_executable(
  altest
  src/gtest-all.cc
  main.cc
  alog_test1.cc
  alog_test2.cc
)
target_include_directories(
  altest PUBLIC
  ./include
  .
  ..
)
target_link_libraries(altest pthread)
target_compile_options(altest PUBLIC -Wall)
target_compile_features(altest PUBLIC cxx_std_14)
