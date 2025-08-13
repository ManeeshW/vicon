import os
import sys
import subprocess
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
    # Fallback to common paths for macOS and Linux
    eigen_include_dir = '/opt/homebrew/include/eigen3' if sys.platform == 'darwin' else '/usr/include/eigen3'

# Define library directories for VRPN and Quat (consistent with CMake)
library_dirs = [
    build_dir,  # Where libfdcl_vicon.a is built
    '/opt/homebrew/lib' if sys.platform == 'darwin' else '/usr/local/lib',  # VRPN and Quat libraries
    get_config_var('LIBDIR'),  # Python library directory
]

# Define the C++ extension
ext_modules = [
    Extension(
        'fdcl_vicon_module',
        ['bindings/vicon_bindings.cpp'],
        include_dirs=[
            pybind11.get_include(),  # Pybind11 headers
            os.path.join(project_root, 'include'),  # Project-specific includes
            eigen_include_dir,  # Eigen include directory
        ],
        libraries=['fdcl_vicon', 'vrpn', 'quat'],  # Link against fdcl_vicon, vrpn, and quat
        library_dirs=library_dirs,
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
    install_requires=['pybind11>=2.6'],
    python_requires='>=3.6',
    zip_safe=False,
)