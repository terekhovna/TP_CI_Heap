cmake_minimum_required(VERSION 3.1)

add_executable(heap TestVector.cpp)

target_link_libraries(heap ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})