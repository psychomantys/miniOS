#!/bin/bash

cd build

export PDCLIB_TOOLCHAIN=gcc
export PDCLIB_PLATFORM=example

jam

