cmake_minimum_required(VERSION 3.30.5)

if(NOT CMAKE_CXX_COMPILER)
	set(CMAKE_CXX_COMPILER "clang++")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_EXPORT_COMPILE_COMMAND YES)


project(rename VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

FILE(GLOB ALL_SRCS src/*)

add_executable(
	rename
	${ALL_SRCS}
)

target_link_libraries(rename ${CURSES_LIBRARIES})
target_compile_options(rename PRIVATE -fstandalone-debug)
