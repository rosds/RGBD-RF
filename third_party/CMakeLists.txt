# googletest
configure_file(googletest.CMakeLists.txt googletest-download/CMakeLists.txt)
execute_process(
  COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)

# yaml-cpp
configure_file(yaml-cpp.CMakeLists.txt yaml-cpp-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-download)
if(result)
  message(FATAL_ERROR "CMake step for yaml-cpp failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-download)
if(result)
  message(FATAL_ERROR "CMake step for yaml-cpp failed: ${result}")
endif()

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable yaml-cpp tests")
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-src
                 ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp-build)
