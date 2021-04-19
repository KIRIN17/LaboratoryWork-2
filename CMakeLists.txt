cmake_minimum_required(VERSION 3.17)
project(UNIT)

set(CMAKE_CXX_STANDARD 14)

add_executable(UNIT main.cpp DynamicArray.cpp DynamicArray.h LinkedList.cpp LinkedList.h Matrix.cpp Matrix.h SquareMatrix.cpp SquareMatrix.h)