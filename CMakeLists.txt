cmake_minimum_required(VERSION 3.24)
project(elfdoc)

set(CMAKE_CXX_STANDARD 20)

add_executable(elfdoc main.cpp ParseELF.h ExceptionHandler.h ExceptionHandler.cpp ParseELF.cpp Detection.cpp Detection.h)
