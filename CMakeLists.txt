cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

add_subdirectory(third_party)
find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs)

add_library(rf
            "include/rf/Image.h"
            "include/rf/ImageFileLoader.hpp"
            "include/rf/LabelImage.h"
            "include/rf/Pixel.h"
            "src/rf/Image.cpp"
            "src/rf/LabelImage.cpp")

target_compile_features(rf PUBLIC cxx_std_17)

target_include_directories(
  rf
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>)

target_link_libraries(rf PUBLIC opencv_core opencv_highgui opencv_imgcodecs)

add_executable(train_forest src/main.cpp)
target_include_directories(
  train_forest
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>)
target_link_libraries(train_forest PUBLIC yaml-cpp)

include(CTest)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
