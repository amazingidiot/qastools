# About

QasTools is a collection of Qt-based mixer and setup tools for the
Linux sound system ALSA.


## Copying

QasTools is distributed under the terms in the COPYING text file that
comes with this package.


## Installation

QasTools uses the CMake build system.

For a system wide installation type:

```
tar -xjf qastools_X.Y.Z.bz2
cd qastools_X.Y.Z
mkdir build
cd build
cmake ..
make -j8
sudo make install
```

For a local build instead call:

```
tar -xjf qastools_X.Y.Z.bz2
cd qastools_X.Y.Z
mkdir install
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
make -j8
make install
```


Authors
-------

QasTools was written by:

Sebastian Holtermann <sebholt@web.de>
