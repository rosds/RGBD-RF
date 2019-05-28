add_executable(unit_tests
    Image.cpp
    LabelImage.cpp
)

target_link_libraries(unit_tests PRIVATE rf gtest_main)

add_test(unit_tests unit_tests)
