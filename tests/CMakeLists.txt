cmake_minimum_required(VERSION 3.8)
project(group_detection)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCE_FILES
    "*.h"
    "*.cpp"
    "../src/Node.*"
    "../src/Connection.*"
    "../src/GroupMember.*"
)

add_executable(group_detection_tests ${SOURCE_FILES})