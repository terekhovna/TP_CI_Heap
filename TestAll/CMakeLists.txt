cmake_minimum_required(VERSION 3.1)

add_executable(TestAll TestAll.cpp)

target_link_libraries(TestAll ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})