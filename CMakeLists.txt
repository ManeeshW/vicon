cmake_minimum_required(VERSION 3.10)  # maneesh_vrpn2025: Bumped to 3.10 for better features
project(FDCL-VICON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

# Find pybind11
find_package(pybind11 REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIRS})
add_definitions(${EIGEN3_DEFINITIONS})

# maneesh_vrpn2025: Explicitly find VRPN and Quat libraries
find_library(VRPN_LIB vrpn HINTS /usr/local/lib)
find_library(QUAT_LIB quat HINTS /usr/local/lib)
if (NOT VRPN_LIB)
    message(FATAL_ERROR "VRPN library not found. Please install VRPN.")
endif()
if (NOT QUAT_LIB)
    message(FATAL_ERROR "Quat library not found. Please install quatlib.")
endif()

# Source files for fdcl_vicon library
set(fdcl_vicon_src
    src/fdcl_vicon.cpp
    src/fdcl_matrix_utils.cpp
)

# Create static library fdcl_vicon
add_library(fdcl_vicon STATIC ${fdcl_vicon_src})
target_link_libraries(fdcl_vicon PUBLIC ${VRPN_LIB} ${QUAT_LIB})  # maneesh_vrpn2025: Link VRPN and Quat to library

# Create Python module
pybind11_add_module(fdcl_vicon_module bindings/vicon_bindings.cpp)
target_link_libraries(fdcl_vicon_module PRIVATE fdcl_vicon ${VRPN_LIB})  # maneesh_vrpn2025: Use variable, removed redundant vrpn

# Add executable and set compile options
add_executable(test_fdcl_vicon main.cpp)
target_link_libraries(test_fdcl_vicon PRIVATE fdcl_vicon ${VRPN_LIB} ${QUAT_LIB})  # maneesh_vrpn2025: Use variables for consistency
target_compile_options(test_fdcl_vicon PRIVATE -Wall -O3 -Wno-class-memaccess)  # maneesh_vrpn2025: Suppress warning