RGBD-RF
=======

Implementation of a Randomized Decision Forest for pose detection in depth
images from the Kinect sensor.


To build this project you need to have installed:

* cmake
* openmpi

Compile the library
-------------------

To compile the library simply execute the next lines in the project
directory:

    mkdir build
    cd build
    cmake ..
    make

Documentation
-------------

To build the documentation you need to have Doxygen installed. The you
just need to execute this:

    make documentation

in the build directory.
