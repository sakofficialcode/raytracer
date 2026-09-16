cmake_minimum_required(VERSION 3.15)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

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

add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_ROOT}/lib/x64/SDL3.dll"
        "$<TARGET_FILE_DIR:raytracer>"
)
