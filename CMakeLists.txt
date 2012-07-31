cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(RandomForest)

# For use of mpi compiler instead of gcc
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} 
                            ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} 
                         ${MPI_LINK_FLAGS})

include_directories(${MPI_INCLUDE_PATH})

find_package(OpenCV REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(include /usr/local/include/libfreenect
    /usr/local/include/OpenEXR /usr/local
    /usr/local/include/opencv2/video/
    ../parser/include)

add_definitions("-Wall -g")

add_subdirectory(src)

FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(Doxyfile.in
        ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)

    add_custom_target (documentation
                       COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                       SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
endif()
