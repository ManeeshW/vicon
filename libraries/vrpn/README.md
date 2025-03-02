# VRPN Libraries

Current veriosn: vrpn_07_30 - [link](ftp://ftp.cs.unc.edu/pub/packages/GRIP/vrpn/vrpn_07_30.zip)

This is cloned to FDCL so that all the FDCL codes which use these libraries will have a way to get it easily, and also as a backup in case the original sources are removed.

Vicon Tracker 3 has a built-in VRPN server that will stream data using VRPN ([see the link](https://www.vicon.com/faqs/software/how-does-tracker-work-with-vrpn)).
The official VRPN library is avaible [here](https://github.com/vrpn/vrpn/wiki). Unfortunately, it does not seem to be compatible with ubuntu. It is recommended to follow the [instructions](http://www.cs.unc.edu/~taylorr/cyberinfrastructure/vrpn_html/index.html) maintained by UNC.
The instruction for installation is available at http://www.cs.unc.edu/~taylorr/cyberinfrastructure/vrpn_html/vrpn_standard_stuff.html

Below are copied from the original instrauctions for installation at UNC.

```
git clone https://github.com/fdcl-gwu/vrpn
cd vrpn/quat
(edit Makefile with your editor, and uncomment the appropriate line eq: "HW_OS := pc_linux" for 32 bit systems or "HW_OS := pc_linux64" for 64 bit systems)
make
sudo make install
```
Repeat the above procedures, at the `vrpn` directory, the `vrpn/server_src` directory, and the `vrpn/client_src` directory.

When compiling codes using VRPN, add the following compiler option `-pthread -lvrpn -lquat`.


## Original ReadMe

IMPORTANT LEGAL INFORMATION for offsite use is in README.Legal
IMPORTANT compiling at other sites information in README.Compile

At UNC:
Compile using 'gmake'

NOTE:	See http://www.cs.unc.edu/Research/vrpn for
information on VRPN.

How to reduce latency on the tracker:
    In any case:
	Run in a way that there isn't swapping going on.
	Probably want to renice the process to a negative priority

    The hack:
	Edit /usr/src/linux/include/asm/params.h and change HZ from 100 to 1000
		to increase the rate at which tty_io.c:flush_to_ldisc() is
		called to move characters from the serial driver
	'setserial /dev/ttyS1 uart 16550' to disable input buffering on that
		serial port and force and interrupt each time a character
		arrives at the buffer
	These two together (absent swapping) cause the driver to read at most
		two characters at a time from the serial port at 19200 baud.

    The real solution:
	Write a new device driver for the tracker that buffers up characters
		in the top-half driver, timestamps the first one with the
		processor tick count, and sets a flag when it has a full
		report.  The read should grab directly from this buffer and
		cook the report in user time.

Why do we need to do any of this ...

Serial drivers have two parts:
1) back end: interrupt driven; it collects chars in buffer as they arrive
2) front end: os driven; every so often the os grabs from the back end
buffer and fills the read/write device buffer (ie, /dev/ttyd*)

On the sgi's, "every so often" is about every 20 ms, so if you do a serial
loopback test (even at 115k), data will not be available for 20 ms (on 
average).

Luckily on machines running linux we can change the "every so often" to
whatever we want (within reason).  We also need to tell linux to use
the 16550 uart driver rather than the general driver.  If we set the hz
up to 1000 and don't change the serial driver, then this doesn't really do
anything.  With both changes, serial loopbacks respond within tenths of a
ms.

(Russ Taylor, November 2000)
