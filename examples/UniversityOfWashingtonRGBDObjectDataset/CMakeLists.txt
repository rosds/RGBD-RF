add_executable(example src/image.cpp src/image_pool.cpp src/main.cpp )
target_link_libraries(example yaml-cpp opencv_core opencv_imgcodecs
  opencv_highgui rf)
set_target_properties(example 
  PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES)
