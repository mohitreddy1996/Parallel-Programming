cmake_minimum_required(VERSION 2.8.4)
project(Parallel)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Prime_Numbers/Prime_Numbers.cpp)
SET(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
add_executable(Parallel ${SOURCE_FILES})
LINK_LIBRARIES(Parallel cppunit)
