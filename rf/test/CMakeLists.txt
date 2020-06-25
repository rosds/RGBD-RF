add_executable(rf_unit_tests "Image.cpp" "LabelImage.cpp")

target_link_libraries(rf_unit_tests gtest gtest_main rf opencv_core)

add_test(rf_unit_tests rf_unit_tests)
