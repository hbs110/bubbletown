#!/bin/sh

mkdir -p linux-build
cd linux-build
cmake ..
make
