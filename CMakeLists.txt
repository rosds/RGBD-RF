cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(RGBD-RF)

# Find OpenCV library
find_package(OpenCV REQUIRED)

include_directories (include)
add_executable (load src/load.cpp)
target_link_libraries (load ${OpenCV_LIBRARIES})
