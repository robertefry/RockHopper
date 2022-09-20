# RockHopper
RockHopper is a cross-platform event-driven multi-thread logic and rendering framework, written for C++ 3D modeling applications (or games).

RockHopper's Trello board can be found here: https://trello.com/b/0tjpxdqC/rockhopper

**NOTICE**: RockHopper is currently undergoing an overhaul to streamline it's subsystems. The overhaul branch is named `overhaul/master`, which will later be merged into `master` when completed.

## Example Usage
Please see the included `Sandbox` application for examples of how RockHopper is used.
This is indecently where I'm testing new features as I add them, while a better testing environment is still pending.

A wiki will hopefully come later when I have the time.

## Building
RockHopper uses CMake as its meta-build system, and a local clone of vcpkg to manage it's dependencies. Running `bootstrap.sh` will locally set-up the build environment.
```bash
./bootstrap.sh
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -B build -G Ninja
cmake --build build --config Debug --target all
```
Built binaries are found in the `output` directory.

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
