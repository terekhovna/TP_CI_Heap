cmake_minimum_required(VERSION 3.1)

add_executable(Binomial_Heap TestBinomial_Heap.cpp)

target_link_libraries(Binomial_Heap ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})