# Vicon Tracker

This project provides a C++ library and Python module for interfacing with a Vicon motion capture system, retrieving position and rotation data. It includes a C++ executable (`test_vicon_tracker`) and a Python script (`test_vicon.py`) for tracking Vicon objects.

## Prerequisites

Ensure the following dependencies are installed on your system (macOS or Linux):

- **CMake** (>= 3.10): Build system for compiling the project.
- **C++ Compiler**: Supporting C++17 (e.g., `g++` or `clang++`).
- **Eigen3** (>= 3.3): Linear algebra library.
- **VRPN**: Virtual Reality Peripheral Network library for Vicon communication.
- **Quat**: Quaternion library for VRPN.
- **Python** (>= 3.6): For the Python module and script.
- **pybind11**: For creating Python bindings.

### Installing Dependencies

#### macOS (using Homebrew)
```bash
brew install cmake eigen vrpn python pybind11
```

#### Linux (using apt on Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake g++ libeigen3-dev
pip3 install pybind11
```

**Notes**:
- On Linux, ensure `libvrpn-dev` includes the Quat library. If not, follow the VRPN installation instructions below.
- Python's `numpy` is required for the Python script. Install it via:
  ```bash
  pip3 install numpy
  ```

### VRPN Installation for Vicon Tracker 4

If VRPN and Quat are not available via package managers or you need a specific version for Vicon Tracker 4, follow these steps.

#### Recommended Method (Linux)
```bash
sudo apt install libusb-1.0-0-dev libpthread-stubs0-dev
sudo apt install -y build-essential cmake git libusb-1.0-0-dev libjpeg-dev libglfw3-dev libglew-dev
git clone https://github.com/vrpn/vrpn.git
cd vrpn
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local  
(optional -DVRPN_BUILD_QUATLIB=ON )
make -j$(nproc)
sudo make install
```

#### Deprecated Method (Linux)
This older method builds VRPN and Quat separately and is not recommended unless the above fails.

```bash
git clone https://github.com/vrpn/vrpn.git
cd vrpn
git submodule update --init
cd quat
# Edit Makefile with your editor, and uncomment the appropriate line:
# For 32-bit systems: "HW_OS := pc_linux"
# For 64-bit systems: "HW_OS := pc_linux64"
make
sudo make install
cd ../
make
sudo make install
cd server_src
make
sudo make install
cd ../client_src
make
sudo make install
```

**macOS Notes**:
- For macOS, use Homebrew (`brew install vrpn`) as the primary method. If you need to build VRPN manually, follow the recommended method above, ensuring dependencies like `libusb`, `libjpeg`, `libglfw3`, and `libglew` are installed via Homebrew:
  ```bash
  brew install libusb libjpeg glfw glew
  ```
- The deprecated method may require additional configuration for macOS (e.g., adjusting `Makefile` for `HW_OS := pc_macosx`).

## Setup and Installation

1. **Create a Build Directory**:
   ```bash
   mkdir build && cd build
   ```

2. **Run CMake**:
   ```bash
   cmake ..
   ```

3. **Build the Project**:
   ```bash
   make
   ```

4. **Install the Python Module**:
   ```bash
   pip install .
   ```

This will compile the C++ library, the `test_vicon_tracker` executable, and the `vicon_tracker` Python module, and copy `config.cfg` and `vicon_tracker.py` to the build directory.

## Configuration

The project uses a `config.cfg` file to specify the Vicon object name. The default configuration is:
```
VICON:
 object: "OriginsX@192.168.10.1"
```
Edit `config.cfg` in the build directory to change the Vicon object name if needed.

## Usage

### Running the C++ Executable
From the build directory:
```bash
./test_vicon_tracker
```
This runs the C++ program, which loads the Vicon object from `config.cfg` and prints position (`x_v`) and rotation (`R_vm`) data.

### Running the Python Script
From the build directory:
```bash
python3 test_vicon.py
```
This runs the Python script, which loads the Vicon object from `config.cfg`, retrieves tracking data, and constructs a 4x4 homogeneous transformation matrix.

**Note**: Ensure the Vicon system is running and accessible at the specified IP address (e.g., `192.168.10.1`).

## Directory Structure
```
vicon/
  ├── bindings/
  │   └── vicon_bindings.cpp  # Python bindings
  ├── include/
  │   └── vicon.hpp           # Vicon class header
  ├── src/
  │   ├── main.cpp
  │   └── vicon.cpp          # Vicon class implementation
  ├── config.cfg              # Configuration file
  ├── vicon_tracker.py        # Python script for tracking
  └── CMakeLists.txt          # Build configuration
```

## Troubleshooting
- **VRPN/Quat not found**: Ensure VRPN and Quat libraries are installed in `/opt/homebrew/lib` (macOS) or `/usr/local/lib` (Linux). Use the VRPN installation steps above if needed.
- **Config file errors**: Verify `config.cfg` exists in the build directory and contains a valid `object:` line.
- **Python module errors**: Ensure `pip install .` was successful and `numpy` is installed.