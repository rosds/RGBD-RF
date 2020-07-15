cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

add_library(rf INTERFACE)
target_include_directories(rf INTERFACE include/)

option(BUILD_EXAMPLES "Build examples" OFF)
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif (BUILD_EXAMPLES)
