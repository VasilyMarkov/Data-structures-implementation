cmake_minimum_required(VERSION 3.20)
project(Projects)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi")

find_package(GTest REQUIRED)
message(STATUS "GoogleTest version: ${GTest_VERSION}")

# add_executable(main main.cpp)
add_executable(test test.cpp)


# target_include_directories(main PRIVATE
# 	${CMAKE_SOURCE_DIR}
# )

target_include_directories(test PRIVATE
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(test GTest::gtest GTest::gtest_main)