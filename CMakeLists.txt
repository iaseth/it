cmake_minimum_required(VERSION 3.10)
project(tti C)

set(CMAKE_C_STANDARD 99)

add_executable(tti
	src/tti.c
	src/tree.c
	src/utils.c
	src/ignore.c
)
