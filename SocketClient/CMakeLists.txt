cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 23)

project(XSocketClient)

set(CMAKE_BUILD_TYPE "Debug")

aux_source_directory(./source SRC_LIST)

include_directories(./include)

add_executable(run ${SRC_LIST})
