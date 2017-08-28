cmake_minimum_required(VERSION 3.8)
project(group_detection)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Connection.cpp Connection.h Node.cpp Node.h GroupDetection.cpp GroupDetection.h GroupMember.cpp GroupMember.h)
add_executable(group_detection ${SOURCE_FILES})