#!/bin/bash

git submodule update --init --recursive
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
