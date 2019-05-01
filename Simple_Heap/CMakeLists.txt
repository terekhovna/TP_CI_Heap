cmake_minimum_required(VERSION 3.1)

add_executable(Simple_Heap TestSimple_Heap.cpp)

target_link_libraries(Simple_Heap ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})