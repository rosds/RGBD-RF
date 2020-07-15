cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

add_library(rf SHARED "src/label_distribution.cpp")
target_include_directories(rf PUBLIC include/)

option(BUILD_EXAMPLES "Build examples" OFF)
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif (BUILD_EXAMPLES)
