cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

include(CTest)
add_subdirectory(third_party)

add_library(rf INTERFACE)
target_include_directories(rf INTERFACE include/)

add_subdirectory(examples)
