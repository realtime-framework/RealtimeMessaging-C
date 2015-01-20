## Realtime Cloud Messaging C SDK
Part of the [The Realtime® Framework](http://framework.realtime.co), Realtime Cloud Messaging (aka ORTC) is a secure, fast and highly scalable cloud-hosted Pub/Sub real-time message broker for web and mobile apps.

If your application has data that needs to be updated in the user’s interface as it changes (e.g. real-time stock quotes or ever changing social news feed) Realtime Cloud Messaging is the reliable, easy, unbelievably fast, “works everywhere” solution.


## API Reference
[http://messaging-public.realtime.co/documentation/c/2.1.0/index.html](http://messaging-public.realtime.co/documentation/c/2.1.0/index.html)

## Requirements

libortc requires:

	CMake >= 2.6 (http://cmake.org/)
	Libwebsockets >= 1.2 (http://libwebsockets.org/)
	libCURL (http://curl.haxx.se)
	Pthread library


## Building on Unix

###Generate the build files (default is Make files):

	cd /path/to/src
	mkdir build
	cd build
	cmake ..


NOTE #1: The build/ directory can have any name and be located
anywhere on your filesystem, and that the argument ".." gi-
ven to cmake is simply the source directory of libwebsockets
containing the CMakeLists.txt project file. All examples in
this file assumes you use ".."

NOTE #2: A common option you may want to give is to set the install
path, same as --prefix= with autotools.
It defaults to /usr/local. You can do this by, eg

	cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/usr

NOTE #3: On machines that want libraries in lib64, you can also add
the following to the `cmake line -DLIB_SUFFIX=64`

### Finally you can build using the generated Makefile:

	make

It should generate in your build directory two folders:

1. lib (containing libortc dynamic and static)
1. example (containing executable example of use the liborc)


## Building on Windows

Generate the Visual studio project by opening the Visual Studio cmd prompt:

	cd <path to src>
	md build
	cd build
	cmake -G "Visual Studio 10" ..

NOTE: There is also a cmake-gui available on Windows if you prefer that

Now you should have a generated Visual Studio Solution in your <path to src>/build directory, 
which can be used to build.

NOTE: On Windows you will probably need to install the following dependencies:



- libCURL (http://curl.haxx.se)
- Pthread library 

We encourage to use:
  [http://www.sourceware.org/pthreads-win32/](http://www.sourceware.org/pthreads-win32/)


## Authors
Realtime.co

