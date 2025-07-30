cmake_minimum_required(VERSION 3.15)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

# Set path to SDL3
set(SDL3_ROOT "${CMAKE_SOURCE_DIR}/external/SDL3")

# Add SDL3 include
include_directories(${SDL3_ROOT}/include)

# Link the SDL3 .lib files
link_directories(${SDL3_ROOT}/lib/x64)

# Your executable
add_executable(raytracer src/main.cpp)

# Link SDL3
target_link_libraries(raytracer SDL3)
