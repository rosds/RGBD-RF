cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(RF_SRC common.cpp
           Offset.cpp
           PixelInfo.cpp
           Image.cpp
           ImagePool.cpp
           Node.cpp
           RandomForest.cpp
           TrainData.cpp
           anyoption.cpp
           parseTreeArgs.cpp
           FloodFill.cpp
           RGBDImage.cpp)

add_library(rdf ${RF_SRC})
