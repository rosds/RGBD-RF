cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

include(CTest)
add_subdirectory(third_party)
find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs)

add_subdirectory(rf)
add_subdirectory(ds)


add_executable(train_forest src/main.cpp)
target_include_directories(
  train_forest
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>)

target_link_libraries(train_forest
                      rf
                      ds
                      yaml-cpp)
