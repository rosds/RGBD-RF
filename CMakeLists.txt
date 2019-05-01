cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs)

add_library(rf INTERFACE)
target_include_directories(rf
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(load src/load.cpp)
target_link_libraries(load
    PRIVATE
        opencv_core
        opencv_highgui
        opencv_imgcodecs
        rf
)
