# RockHopper
RockHopper is a cross-platform event-driven multi-thread logic and rendering framework, written for C++ 3D modeling applications (or games).

## Example Usage
Please see the included `Sandbox` application for examples of how RockHopper is used. This is indecently where I'm also testing new features as I add them.

A wiki will hopefully come later when I have the time.

## Building
RockHopper uses CMake as its meta-build system, and a local clone of vcpkg to manage it's dependencies. Running `bootstrap.sh` will locally set-up the build environment.
```bash
./bootstrap.sh
mkdir build && cd $_
cmake .. && make <target>
```
Please see the make `target`s below.
* The default target will make everything.
* Binaries;
    * `RockHopper` will make the main RockHopper dynamic library.
    * `RockHopper-Tests` will make the RockHopper test suite.
    * `RockHopper-Sandbox` will make the Sandbox application.
* Commands;
    * `run_sandbox` will make and run the Sandbox application.
    * `test` will run the test suite using cmake-tests.
    * `gtest` will run the test suite application.

## Development
To setup the development environment; please clone the `develop` branch of this repository.

RockHopper as the following build variables;

* `CMAKE_BUILD_TYPE=(MinSizeRel|Release|RelWithDebInfo|Debug)`
<br> This controls any debug or other build symbols. It will also control the logging by default.
* `ROCKHOPPER_LOG_LEVEL=<integer>`
<br> This manually sets the logging level as an integer (`0` for no logging). (See `RockHopper/include/RockHopper/Logging/logger.hh` for details)
<br> <i>While development is ongoing, this is set as default to `4`.</i>
