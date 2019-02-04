cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(modularMadness)

include_directories(../include)

add_executable(
	modularMadness
	main.cpp
	module.cpp
	moduleOperations.cpp
	processor.cpp
	../include/module.h
	../include/moduleOperations.h
	../include/processor.h
)


