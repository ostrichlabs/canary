cmake_minimum_required(VERSION 3.1)

project(canary VERSION 1.0
	           DESCRIPTION "Minesweeper clone"
	           LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
	           
target_include_directories(canary PUBLIC 