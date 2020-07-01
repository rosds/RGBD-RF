add_library(rf "src/label.cpp" "src/Image.cpp" "src/LabelImage.cpp")

target_include_directories(
  rf
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>)

target_compile_features(rf PUBLIC cxx_std_17)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
