from setuptools import setup, Extension
import pybind11
import os

# Define the C++ extension
ext_modules = [
    Extension(
        'fdcl_vicon_module',
        ['bindings/vicon_bindings.cpp'],
        include_dirs=[
            pybind11.get_include(),
            'include',
            'libraries/eigen',
        ],
        libraries=['fdcl_vicon', 'vrpn'],  # Libraries to link against
        library_dirs=[
            os.path.join(os.path.dirname(__file__), 'build'),  # Path to the build directory
            '/usr/local/lib',  # Update this with the actual path to vrpn library
        ],
        language='c++',
    ),
]

# Define the setup
setup(
    name='fdcl_vicon',
    version='0.1.0',
    author='Your Name',
    author_email='your_email@example.com',
    description='Python bindings for the FDCL VICON library',
    ext_modules=ext_modules,
    zip_safe=False,
)
