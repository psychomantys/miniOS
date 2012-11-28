#!/bin/bash

(
cd build

./configure --prefix=/home/psycho/codigos/github/miniOS/src/libc/NewLib/root --target=i686-pc-linux-gnu
make

cd newlib

./configure
make

)

ln -s build/newlib/.libs/libc.a libc.a

