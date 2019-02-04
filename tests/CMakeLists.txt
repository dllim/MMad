cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

project(modularMadnessTests)

ADD_SUBDIRECTORY(googletest)
enable_testing()
include_directories(googletest)
include_directories(../include)
include_directories(../src)

add_executable(
	runModularMadnessTests
	test.cpp
	ModularMadnessTest.h
	moduleOperationsTests.h
	../src/module.cpp
	../src/moduleOperations.cpp
	../src/processor.cpp
)

target_link_libraries(runModularMadnessTests gtest)

add_test(runModularMadnessTests runModularMadnessTests)



	
	