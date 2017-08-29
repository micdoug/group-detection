cmake_minimum_required(VERSION 3.8)
project(group_detection)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCE_FILES
    "*.h"
    "*.cpp"
)
add_executable(group_detection ${SOURCE_FILES})