cmake_minimum_required(VERSION 3.1)

add_executable(Fibonachi_Heap TestFibonachi_Heap.cpp)

target_link_libraries(Fibonachi_Heap ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})