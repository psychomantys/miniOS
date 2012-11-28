#!/bin/bash

wget -c ftp://sources.redhat.com/pub/newlib/newlib-1.20.0.tar.gz

wget -c 'http://coderepos.org/share/browser/lang/perl/PerlMachine/trunk/include/newlib/stddef.h?rev=12453&format=txt'

tar -xvf newlib-1.20.0.tar.gz

ln -s newlib-1.20.0 build

