
# RockHopper

RockHopper is a cross-platform event-driven multi-thread logic and rendering framework, written for C++ 3D modeling applications (or games).

RockHopper's Trello board can be found here: https://trello.com/b/0tjpxdqC/rockhopper

## Example Usage
Please see the included `RockHopper-Sandbox` application for examples of how RockHopper is used.
This is indecently where I'm testing new features as I add them, while a better testing environment is still pending.

A wiki will hopefully come later when I have the time.

# Building

RockHopper uses CMake as both its meta-build system and dependency management system, merged from my [initial-cpp](https://github.com/robertefry/initial-cpp) starter template. Building is as simple as running cmake.
```
$ cmake -S . -B .build -G Ninja
$ cmake --build .build --target RockHopper
```
* Using `CMAKE_EXPORT_COMPILE_COMMANDS` is necessary to properly enable clang-tidy.
* Using `CMAKE_BUILD_TYPE` is necessary to specify a non-default build type.

## Build Targets

* The default target will make everything.
* Binaries;
    * `RockHopper` will make the main RockHopper dynamic library.
    * `RockHopper-Tests` will make the RockHopper test suite.
    * `RockHopper-Sandbox` will make the Sandbox application.
* Commands;
    * `test-RockHopper` will run the test suite using cmake-tests.
    * `run-RockHopper-Sandbox` will make and run the Sandbox application.

# Development

To setup the development environment; please clone the `develop` branch of this repository.

The build-system will default to using a `Release` build, so we will need to specify the `Debug` build type. Furthermore, for clang-tidy to work correctly, we will need to export our build commands.

```
$ cmake -S . -B .build -G Ninja \
    -D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -D CMAKE_BUILD_TYPE:STRING=Debug
$ cmake --build .build --target all
```

RockHopper as the following build variables;

* `CMAKE_BUILD_TYPE=(MinSizeRel|Release|RelWithDebInfo|Debug)`
<br> This controls any debug or other build symbols. It will also control the logging by default.
* `ROCKHOPPER_LOG_LEVEL=<integer>`
<br> This manually sets the logging level as an integer (`0` for no logging). (See `RockHopper/include/RockHopper/Logging/logger.hh` for details)
<br> <i>While development is ongoing, this is set as default to `4`.</i>
