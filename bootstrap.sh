#!/bin/bash

# download git submodules
git submodule update --init --recursive

# list dependencies
declare -a dependencies
dependencies+=(fmt spdlog)  # logging
dependencies+=(glm)         # maths
dependencies+=(glfw3 glad)  # graphics

# bootstrap vcpkg dependencies
./RockHopper/vcpkg/bootstrap-vcpkg.sh -disableMetrics
./RockHopper/vcpkg/vcpkg install ${dependencies[@]}
