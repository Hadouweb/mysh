cmake_minimum_required(VERSION 3.5)
project(mysh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    astree.c
    astree.h
    astree.o
    command.c
    command.h
    command.o
    execute.c
    execute.h
    execute.o
    lexer.c
    lexer.h
    lexer.o
    LICENSE.md
    Makefile
    parser.c
    parser.h
    parser.o
    README.md
    shell
    shell.c
    shell.o)

add_executable(mysh ${SOURCE_FILES})