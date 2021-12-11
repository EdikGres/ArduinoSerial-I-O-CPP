cmake_minimum_required(VERSION 3.20)
project(ArduinoCppConnect)

set(CMAKE_CXX_STANDARD 17)
add_executable(ArduinoCppConnect main.cpp Serial.h Serial.cpp)
