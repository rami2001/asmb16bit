cmake_minimum_required(VERSION 3.21)
project (assembleur)

set(CMAKE_CXX_STANDARD 17)

add_executable(assembleur main.cpp asm.cpp Cpu.cpp Cpu.h)
