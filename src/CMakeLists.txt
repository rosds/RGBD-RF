cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(RF_SRC common.cpp
           Offset.cpp
           PixelInfo.cpp
           Image.cpp
           ImagePool.cpp
           main.cpp
           Node.cpp
           RandomForest.cpp
           TrainData.cpp
           anyoption.cpp
           parseTreeArgs.cpp
           ../../parser/src/Sparse.cpp
           FloodFill.cpp)

set(RF_SRC2 common.cpp
           Offset.cpp
           PixelInfo.cpp
           Image.cpp
           ImagePool.cpp
           main2.cpp
           Node.cpp
           RandomForest.cpp
           TrainData.cpp
           anyoption.cpp
           parseTreeArgs.cpp
           ../../parser/src/Sparse.cpp
           FloodFill.cpp)


add_executable(rf ${RF_SRC})
target_link_libraries(rf m pthread glut GLU freenect IlmThread IlmImf
    Iex Half Imath z ${OpenCV_LIBS} ${MPI_LIBRARIES})

add_executable(rf2 ${RF_SRC2})
target_link_libraries(rf2 m pthread glut GLU freenect IlmThread IlmImf
    Iex Half Imath z ${OpenCV_LIBS} ${MPI_LIBRARIES})

add_executable(camshift camshiftdemo.cpp)
target_link_libraries(camshift m pthread glut GLU freenect IlmThread IlmImf 
    Iex Half Imath z ${OpenCV_LIBS} ${MPI_LIBRARIES})

add_library(rdf ${RF_SRC})
