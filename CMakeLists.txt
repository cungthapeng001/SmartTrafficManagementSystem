cmake_minimum_required(VERSION 3.10)
project(SmartTrafficManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(SmartTrafficManagementSystem 
    src/main.cpp
    src/Vehicle.cpp
    src/Road.cpp
    src/TrafficManager.cpp
    src/BackendProtocol.cpp
)
