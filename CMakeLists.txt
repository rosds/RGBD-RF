cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

find_package(Threads REQUIRED)
add_library(rf SHARED "src/label_distribution.cpp")
target_include_directories(rf PUBLIC include/)
target_link_libraries(rf PUBLIC Threads::Threads)

option(BUILD_EXAMPLES "Build examples" OFF)
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif (BUILD_EXAMPLES)
