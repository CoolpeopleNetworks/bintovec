cmake_policy(SET CMP0048 NEW)

project(bintovec VERSION 1.0.5)
cmake_minimum_required(VERSION 3.18)

include(FetchContent)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG        v2.1
)

FetchContent_MakeAvailable(argparse)

add_executable(bintovec bintovec.cpp)

target_link_libraries(bintovec argparse)

set_property(TARGET bintovec PROPERTY CXX_STANDARD 20)
set_property(TARGET bintovec PROPERTY CXX_STANDARD_REQUIRED ON)
