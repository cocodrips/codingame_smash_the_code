cmake_minimum_required(VERSION 3.3)
project(smash_the_code)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES game.cpp)
add_executable(smash_the_code ${SOURCE_FILES})


set(TEST_FILES game.cpp test/test_vanish.cpp test/test_io.cpp input/all_direction_chain.txt test/test_simulator.cpp.cpp)
include_directories(gtest)
link_directories(/Users/cocodrips/Documents/AI/codingame/smash_the_code/gtest)
add_executable(gtest ${TEST_FILES})
target_link_libraries(gtest libgtest.a libgtest_main.a)
