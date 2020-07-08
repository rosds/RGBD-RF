cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

include(CTest)
add_subdirectory(third_party)
find_package(OpenCV REQUIRED COMPONENTS opencv_core opencv_imgcodecs)

add_subdirectory(rf)
add_subdirectory(ds)
add_subdirectory(examples)
