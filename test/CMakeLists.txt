add_executable(unit_tests image.cpp)
target_link_libraries(unit_tests PRIVATE gtest_main)

add_test(unit_tests unit_tests)
