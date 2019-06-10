SimpleCPP
=========

This has the simplecpp package.

Please do not distribute.

Dependencies:
------------

Requires `cmake`, `pkgconfig`, `glew`, `fribidi`, `freetype` and `fontconfig`. Install with `brew install cmake pkgconfig glew fribidi freetype fontconfig` on a Mac, or `apt-get` etc. on Linux.

Instructions for installing the simplecpp package:
1. Change the directory to this (`cd SimpleCPP`), create a build directory inside this directory (`mkdir build`), and change to the build directory (`cd build`).
2. Run CMake (`cmake ..`)
3. Run Make (`make -jN`) substitute `N` for the number of processors you have.
4. Install (`make install` on Mac, or `sudo make install` on Linux).

This installs a shell script s++ (and the required libraries).

s++ is the compiler for use with simplecpp.

To use simplecpp, your source files should contain
`#include <simplecpp>`. To compile, do `s++ myprogram.cpp`.

------

Simplecpp directory contains following subdirectories:

       include : contains C++ include files

       lib     : contains libsprite.a  

       src     : source files.  

-----
