add_library(utils utils.cpp)

add_library(AdventOfCode24::utils ALIAS utils)

target_include_directories(utils PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include)
target_include_directories(utils PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR})

set_target_properties(
  utils
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)
