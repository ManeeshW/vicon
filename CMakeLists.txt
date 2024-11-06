cmake_minimum_required(VERSION 3.5)
project(FDCL-VICON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

# Find pybind11
find_package(pybind11 REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libraries/eigen)

#the VRPN library is installed path
link_directories("/usr/local/lib/")

# Source files for fdcl_vicon library
set(fdcl_vicon_src
    src/fdcl_vicon.cpp
    src/fdcl_matrix_utils.cpp
)

# Create static library fdcl_vicon
add_library(fdcl_vicon STATIC ${fdcl_vicon_src})

# Create Python module
pybind11_add_module(fdcl_vicon_module bindings/vicon_bindings.cpp)
target_link_libraries(fdcl_vicon_module PRIVATE fdcl_vicon vrpn)

# Add executable and set compile options
add_executable(test_fdcl_vicon main.cpp)
target_link_libraries(test_fdcl_vicon PRIVATE fdcl_vicon vrpn quat)
