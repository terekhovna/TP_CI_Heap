cmake_minimum_required(VERSION 3.1)

add_executable(Vector TestVector.cpp)

target_link_libraries(Vector ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})