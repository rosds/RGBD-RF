cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(RGBD-RF)

##############################################################################
#   Required packages
##############################################################################

# Find OpenCV library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# Find MPICH
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# For use of mpi compiler instead of gcc
#if(MPI_FOUND)
#include_directories(${MPI_INCLUDE_PATH})
#set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} 
                            #${MPI_COMPILE_FLAGS})
#set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} 
                         #${MPI_LINK_FLAGS})
#endif(MPI_FOUND)

##############################################################################
#   Random Forest limrary generation
##############################################################################

set(HFILES 
    include/rdf/common.h
    include/rdf/Image.h
    include/rdf/ImagePool.h
    include/rdf/Node.h
    include/rdf/Offset.h
    include/rdf/PixelInfo.h
    include/rdf/RandomForest.h
    include/rdf/TrainData.h
)

set(CPPFILES 
    src/common.cpp
    src/FloodFill.cpp
    src/Image.cpp
    src/ImagePool.cpp
    src/Node.cpp
    src/Offset.cpp
    src/parseTreeArgs.cpp
    src/PixelInfo.cpp
    src/RandomForest.cpp
    src/TrainData.cpp
)

include_directories(include)

add_library(rdf SHARED ${HFILES} ${CPPFILES})

##############################################################################
#   Requires C++11 standard support.
##############################################################################

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set_property(TARGET rdf APPEND_STRING PROPERTY COMPILE_FLAGS 
        "-std=c++11 ")
elseif(COMPILER_SUPPORTS_CXX0X)
    set_property(TARGET rdf APPEND_STRING PROPERTY 
        COMPILE_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 
    support. Please use a different C++ compiler.")
endif() 

##############################################################################
#   Doxygen documentation
##############################################################################

# adds custom command 'make doc' to generate the documentation.
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
