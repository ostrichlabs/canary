# canary CMake
# should probably change canary to something else per project

cmake_minimum_required(VERSION 3.1)

project(canary VERSION 0.1
	           DESCRIPTION "Minesweeper clone"
	           LANGUAGES CXX)

# global settings
# Always use standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Raspberry Pi cross-compilation settings
# some may have to be manually modified
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT )
set(CMAKE_STAGING_PREFIX )

set(tools C:\dev\SysGCC\raspberry) # point this to your cross-compiler
set(CMAKE_C_COMPILER ${tools}\bin\arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}\bin\arm-linux-gnueabihf-g++)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)