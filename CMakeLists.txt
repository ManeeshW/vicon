cmake_minimum_required(VERSION 3.10)  # maneesh_vrpn2025: Bumped to 3.10 for better features
project(VICON_TRACKER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Python using modern FindPython module
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

# Find Eigen3
find_package(Eigen3 3.3 REQUIRED NO_MODULE)                     

# maneesh_vrpn2025: Explicitly find VRPN and Quat libraries
find_library(VRPN_LIB vrpn HINTS /opt/homebrew/lib /usr/local/lib)
find_library(QUAT_LIB quat HINTS /opt/homebrew/lib /usr/local/lib)
if (NOT VRPN_LIB)
    message(FATAL_ERROR "VRPN library not found. Please install VRPN.")
endif()
if (NOT QUAT_LIB)
    message(FATAL_ERROR "Quat library not found. Please install quatlib.")
endif()

# Source files for vicon_tracker library
set(vicon_src
    src/vicon.cpp
)

# Create static library vicon_tracker_static
add_library(vicon_tracker_static STATIC ${vicon_src})
target_include_directories(vicon_tracker_static PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(vicon_tracker_static PUBLIC ${VRPN_LIB} ${QUAT_LIB} Eigen3::Eigen)  # maneesh_vrpn2025: Link VRPN, Quat, and Eigen to library

# Create Python module
pybind11_add_module(vicon_tracker bindings/vicon_bindings.cpp)
target_link_libraries(vicon_tracker PRIVATE vicon_tracker_static ${VRPN_LIB})  # maneesh_vrpn2025: Link Python module

# Add executable and set compile options
add_executable(test_vicon_tracker src/main.cpp)
target_link_libraries(test_vicon_tracker PRIVATE vicon_tracker_static ${VRPN_LIB} ${QUAT_LIB})  # maneesh_vrpn2025: Use variables for consistency
target_compile_options(test_vicon_tracker PRIVATE -Wall -O3 -Wno-class-memaccess)  # maneesh_vrpn2025: Suppress warning