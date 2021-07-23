project(bintovec)
cmake_minimum_required(VERSION 3.18)

add_executable(bintovec bintovec.cpp)

set_property(TARGET bintovec PROPERTY CXX_STANDARD 20)
set_property(TARGET bintovec PROPERTY CXX_STANDARD_REQUIRED ON)
