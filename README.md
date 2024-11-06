# FDCL-VICON

This repository includes minimal working codes for reading the position and attitude message from the Vicon server with C++.
This example simply prints the read data to the terminal.
If you need data to be used in a different section of your code, add an external callback to the `fdcl::vicon::callback` function.
Please note that this external callback needs to be a static function.

## Dependencies

### VRPN
```
git submodule update --init
cd libraries

cd vrpn/quat
(edit Makefile with your editor, and uncomment the appropriate line eg: "HW_OS := pc_linux" for 32 bit systems or "HW_OS := pc_linux64" for 64 bit systems)
make
sudo make install
```

Repeat the above procedures, at the `vrpn` directory, the `vrpn/server_src` directory, and the `vrpn/client_src` directory.