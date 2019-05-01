cmake_minimum_required(VERSION 3.1)

add_executable(Heap TestHeap.cpp)

target_link_libraries(Heap ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})