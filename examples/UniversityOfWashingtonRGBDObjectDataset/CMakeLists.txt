add_executable(example src/main.cpp src/image_pool.cpp)
target_link_libraries(example yaml-cpp rf)
set_target_properties(example 
  PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES)
