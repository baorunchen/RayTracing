cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include .)

aux_source_directory(. DIR_SRCS)

add_executable(RayTracing ${DIR_SRCS})