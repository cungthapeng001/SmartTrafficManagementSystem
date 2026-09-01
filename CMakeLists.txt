cmake_minimum_required(VERSION 3.10)
project(SmartTrafficManagementSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/Vehicle.cpp
    src/TrafficManagementSystem.cpp
)

# Main executable
add_executable(traffic_app src/main.cpp ${SOURCES})

# Test executable
add_executable(traffic_test tests/test_traffic.cpp ${SOURCES})
