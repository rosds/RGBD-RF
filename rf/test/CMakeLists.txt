add_executable(rf_unit_tests "Image.cpp" "LabelImage.cpp")

target_link_libraries(rf_unit_tests PRIVATE rf gtest_main opencv_core)

add_test(rf_unit_tests rf_unit_tests)
