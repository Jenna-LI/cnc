cmake_minimum_required(VERSION 3.10)
project(cnc)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Find OpenCV
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# Collect source files
file(GLOB SOURCES "src/*.cpp")

# Define executable
add_executable(hello ${SOURCES})

# Link OpenCV libraries
target_link_libraries(hello ${OpenCV_LIBS})
