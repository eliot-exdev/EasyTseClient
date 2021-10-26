cmake_minimum_required(VERSION 3.18)
project(EasyTseClient)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(tseclient_lib)
add_subdirectory(test_socket)
add_subdirectory(test_tse)
