cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(RGBD-RF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs)

add_library(rf
    "include/rf/Image.h"
    "include/rf/ImageFileLoader.hpp"
    "include/rf/LabelImage.h"
    "include/rf/Pixel.h"
    "src/rf/Image.cpp"
    "src/rf/LabelImage.cpp"
)

target_include_directories(rf
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(rf
    PUBLIC
        opencv_core
        opencv_highgui
        opencv_imgcodecs
)

option(ENABLE_TESTING "Enable tests" OFF)

if(${ENABLE_TESTING})
    enable_testing()
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)

    add_subdirectory(test)
endif()

