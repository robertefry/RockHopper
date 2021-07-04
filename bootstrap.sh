#!/bin/sh

# download git submodules
git submodule update --init --recursive

# bootstrap vcpkg dependencies
./RockHopper/vcpkg/bootstrap-vcpkg.sh -disableMetrics
./RockHopper/vcpkg/vcpkg install fmt spdlog glfw3 glad
