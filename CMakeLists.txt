cmake_minimum_required(VERSION 3.16)
project(SP_4)
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Client)
add_subdirectory(Server)