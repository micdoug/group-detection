cmake_minimum_required(VERSION 3.8)
project(group_detection)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES
    "*.h"
    "*.cpp"
    "../libs/boost/src/*.cpp"
)

include_directories("../libs")

add_executable(group-detection ${SOURCE_FILES})

find_package (Threads)
target_link_libraries (group-detection ${CMAKE_THREAD_LIBS_INIT})
