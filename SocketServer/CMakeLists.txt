cmake_minimum_required(VERSION 3.30.3)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(XSocketServer)

set(CMAKE_BUILD_TYPE "Debug")

set(x_socket_include_dir ${PROJECT_SOURCE_DIR}/include)
include_directories(${x_socket_include_dir})

set(x_socket_source_dir ${PROJECT_SOURCE_DIR}/source)
file(GLOB_RECURSE x_socket_source_file ${x_socket_source_dir}/*.cpp)
add_library(x_socket_shared SHARED ${x_socket_source_file})

set(x_socket_test_dir ${PROJECT_SOURCE_DIR}/test)
file(GLOB_RECURSE x_socket_test_file ${x_socket_test_dir}/*.cpp)
foreach(i_test ${x_socket_test_file})
	get_filename_component(x_socket_test_filename ${i_test} NAME_WE)

	add_executable(${x_socket_test_filename} EXCLUDE_FROM_ALL ${i_test})

	target_link_libraries(${x_socket_test_filename} x_socket_shared)

endforeach(i_test ${x_socket_test_file})
