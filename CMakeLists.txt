cmake_minimum_required(VERSION 3.0.0)

# set the project name
project(RACER VERSION 0.1.0)

# Define the directories in which to search for the included headers
include_directories(
    source/common
    vendor/glm/
)

# add the executable
add_executable(RACER source/main.cpp)