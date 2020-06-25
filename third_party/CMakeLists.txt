include(FetchContent)

FetchContent_Declare(
  extern_yaml
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        yaml-cpp-0.6.3)

FetchContent_GetProperties(extern_yaml)
if(NOT extern_yaml_POPULATED)
  FetchContent_Populate(extern_yaml)
  set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml-cpp tests")
  add_subdirectory(
    ${extern_yaml_SOURCE_DIR}
    ${extern_yaml_BINARY_DIR})
endif()

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
