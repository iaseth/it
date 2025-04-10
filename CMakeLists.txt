cmake_minimum_required(VERSION 3.10)
project(it C)

set(CMAKE_C_STANDARD 99)

add_executable(it
	src/main.c
	src/args.c
	src/analysis.c
	src/ignore.c
	src/stringutils.c
	src/tree.c
	src/utils.c
)
