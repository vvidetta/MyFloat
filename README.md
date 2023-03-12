# MyFloat #

This repo contains a (very incomplete) hobby implementation of the 64-bit floating point type defined by IEEE-754. I make no claims about completeness, accuracy, merchantability, fitness-for-a-particular-use or any other quality; I can't be bothered to pay $100 for [the standard](https://standards.ieee.org/ieee/754/6210/) :upside_down_face:

## Build requirements ##

* Currently Windows-only
* Visual Studio 2022
* GoogleTest
* CMake (Required for building GoogleTest, at time of writing)

## Building ##

This project requires prebuilt GoogleTest binaries. VV 2023-03-12: I might redo this as a git submodule at some stage, but am more interested in implementing the standard right now rather than optimising my consumption of dependencies.

1. Clone the GoogleTest repo from GitHub into a separate directory.
2. Build Debug x64 and Release x64 GoogleTest binaries using CMake and/or Visual Studio. Ensure that you
3. Set environment variables MYFLOAT_GOOGLETEST_REPO to the path of your cloned GoogleTest repo and MYFLOAT_GOOGLETEST_BIN to the base of the directory hierarchy containing the results of your CMake build (this will commonly just be %MYFLOAT_GOOGLETEST_REPO%\build)
4. Open the Visual Studio solution in this repo (ensuring that it inherits the GOOGLETEST_REPO evar set above).
5. Build and run the test executable project using the standard Visual Studio UI.
