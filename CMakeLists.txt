# 指定使用 CMake 的最低版本号
cmake_minimum_required(VERSION 3.0)

# 设置项目名称
project(ryan_skip_list C CXX)

# 添加搜索路径
include_directories(
    ${PROJECT_SOURCE_DIR}/skiplist_ryan
    )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 包含这些目录下的.cc文件
# 下面这两个会把目录及其子目录的所有.cc文件都包括
#aux_source_directory(${PROJECT_SOURCE_DIR}/ryan_network/logger SRC_LOGGER)
#aux_source_directory(${PROJECT_SOURCE_DIR}/ryan_network/logger SRC_LOGGER)
#如果想要不包含子目录下的文件，则使用file
file(GLOB SRC_BASIC_TEST ${PROJECT_SOURCE_DIR}/skiplist_ryan/test/basic_test.cc)
file(GLOB SRC_STRESS_TEST ${PROJECT_SOURCE_DIR}/skiplist_ryan/test/stress_test.cc)
file(GLOB SRC_INSERT_TEST ${PROJECT_SOURCE_DIR}/skiplist_ryan/test/insert_test.cc)
file(GLOB SRC_SEARCH_TEST ${PROJECT_SOURCE_DIR}/skiplist_ryan/test/search_test.cc)

# 设置编译选项
set(CXX_FLAGS
    -g
    -Wall
    -std=c++11
    -O0
    )

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -std=c++11")


#find_package(Boost REQUIRED)
#find_library(BOOST_TEST_LIBRARY NAMES boost_unit_test_framework)
#include_directories(${Boost_INCLUDE_DIRS})


add_executable(basic_test ${SRC_BASIC_TEST})

add_executable(insert_test ${SRC_INSERT_TEST})

add_executable(search_test ${SRC_SEARCH_TEST})

add_executable(stress_test ${SRC_STRESS_TEST})
target_link_libraries(stress_test pthread)


