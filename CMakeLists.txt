cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(RGBD-RF)

# Find OpenCV library
find_package(OpenCV REQUIRED)

add_library(stdc++fs UNKNOWN IMPORTED)
set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a")


include_directories (include)
add_executable (load src/load.cpp)
target_link_libraries (load ${OpenCV_LIBRARIES} stdc++fs)

set_property(TARGET load PROPERTY CXX_STANDARD 17)
