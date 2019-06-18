add_library(ds "src/RgbdWashington.cpp")
target_include_directories(ds
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(ds PUBLIC rf PRIVATE opencv_core opencv_highgui opencv_imgcodecs)
