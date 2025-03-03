from setuptools import setup, Extension
import pybind11
import os
import sys

# Define paths
eigen_include_dir = '/usr/include/eigen3'  # System-installed Eigen path from CMake
vrpn_lib_dir = '/usr/local/lib'  # VRPN library path (consistent with CMake)
project_root = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(project_root, 'build')

# Define the C++ extension
ext_modules = [
    Extension(
        'fdcl_vicon_module',
        ['bindings/vicon_bindings.cpp'],
        include_dirs=[
            pybind11.get_include(),  # Pybind11 headers
            os.path.join(project_root, 'include'),  # Project-specific includes
            eigen_include_dir,  # System-installed Eigen
        ],
        libraries=['fdcl_vicon', 'vrpn', 'quat'],  # Added 'quat' for completeness
        library_dirs=[
            build_dir,  # Where libfdcl_vicon.a is built
            vrpn_lib_dir,  # VRPN and quat libraries
        ],
        extra_compile_args=['-std=c++17', '-Wall', '-O3'],  # Match CMake flags
        language='c++',
    ),
]

# Define the setup
setup(
    name='fdcl_vicon',
    version='0.1.0',
    author='Your Name',
    author_email='maneesh@gwu.edu',
    description='Python bindings for the FDCL VICON library',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.6'],  # Ensure pybind11 is installed
    python_requires='>=3.6',  # Minimum Python version
    zip_safe=False,
)