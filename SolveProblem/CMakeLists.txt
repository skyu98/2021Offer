cmake_minimum_required(VERSION 3.1)

project(solveProblem)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread -g")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(CharInput_test CharInput_test.cc)
add_executable(ListInput_test ListInput_test.cc)
