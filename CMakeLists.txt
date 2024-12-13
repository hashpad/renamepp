cmake_minimum_required(VERSION 3.30.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CXX_COMPILER)
	set(CMAKE_CXX_COMPILER "clang++")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()




project(rename VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


FILE(GLOB ALL_SRCS src/*)

add_executable(
	rename
	${ALL_SRCS}
)


find_package(CLI11 REQUIRED)
target_link_libraries(rename PRIVATE CLI11::CLI11)

find_package(Curses REQUIRED)
target_include_directories(rename PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(rename PRIVATE ${CURSES_LIBRARIES})


target_compile_options(rename PRIVATE -fstandalone-debug)
