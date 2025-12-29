cmake_minimum_required(VERSION 3.20)
project(Projects)

find_package(GTest REQUIRED)
message(STATUS "GoogleTest version: ${GTest_VERSION}")

add_executable(test test.cpp)

target_include_directories(test PRIVATE
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(test GTest::gtest GTest::gtest_main)