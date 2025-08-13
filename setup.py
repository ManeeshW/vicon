import os
import sys
import subprocess
import platform
from setuptools import setup, Extension
import pybind11
from distutils.sysconfig import get_config_var

# Define paths
project_root = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(project_root, 'build')

# Try to find Eigen include directory using pkg-config
try:
    eigen_include_dir = subprocess.check_output(['pkg-config', '--cflags-only-I', 'eigen3']).decode().strip().replace('-I', '')
except (subprocess.CalledProcessError, FileNotFoundError):
    # Fallback to common paths for macOS (Homebrew) and Linux
    possible_paths = [
        '/opt/homebrew/include/eigen3',  # macOS Homebrew
        '/usr/local/include/eigen3',     # Common on both
        '/usr/include/eigen3',           # Linux default
    ]
    for path in possible_paths:
        if os.path.exists(path):
            eigen_include_dir = path
            break
    else:
        raise RuntimeError("Eigen3 include directory not found. Please install Eigen3.")

# Define library directories for VRPN, Quat, and the built static lib (consistent with CMake)
library_dirs = [
    build_dir,  # Where libvicon_tracker_static.a is built
    '/opt/homebrew/lib' if platform.system() == 'Darwin' else '/usr/local/lib',  # VRPN and Quat libraries on macOS/Linux
    '/usr/lib',  # Additional common Linux path
    get_config_var('LIBDIR'),  # Python library directory
]

# Define the C++ extension
ext_modules = [
    Extension(
        'vicon_tracker',
        ['bindings/vicon_bindings.cpp'],
        include_dirs=[
            pybind11.get_include(),  # Pybind11 headers
            os.path.join(project_root, 'include'),  # Project-specific includes
            eigen_include_dir,  # Eigen include directory
        ],
        libraries=['vicon_tracker_static', 'vrpn', 'quat'],  # Link against vicon_tracker_static, vrpn, and quat
        library_dirs=library_dirs,
        extra_compile_args=['-std=c++17', '-Wall', '-O3'],  # Match CMake flags
        language='c++',
    ),
]

# Define the setup
setup(
    name='vicon_tracker',
    version='1.0',
    author='Maneesha Wickramasuriya',
    author_email='maneesh@gwu.edu',
    description='Python bindings for the VICON tracker library',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.6'],
    python_requires='>=3.6',
)