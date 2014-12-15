cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(RGBD-RF)

# For use of mpi compiler instead of gcc
find_package(OpenCV REQUIRED)
find_package(MPI)
FIND_PACKAGE(Doxygen)

if(MPI_FOUND)
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} 
                            ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} 
                         ${MPI_LINK_FLAGS})
endif(MPI_FOUND)


if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)

add_custom_target(samples
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating sample code" VERBATIM
        )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(include /usr/local/include/libfreenect
    /usr/local/include/OpenEXR /usr/local
    ${OpenCV_INCLUDE_DIRS}
    ../parser/include)

add_definitions("-Wall -g")
add_subdirectory(src)
