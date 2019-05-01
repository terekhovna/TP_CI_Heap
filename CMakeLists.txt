cmake_minimum_required(VERSION 3.1)

SET(CMAKE_CXX_FLAGS  "-O2 ${CMAKE_CXX_FLAGS}")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_subdirectory(Vector)
add_subdirectory(Simple_Heap)
add_subdirectory(Heap)
add_subdirectory(Binomial_Heap)
add_subdirectory(Fibonachi_Heap)
add_subdirectory(TestAll)