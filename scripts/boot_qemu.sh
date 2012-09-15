#!/bin/bash

BOOT_TIME=2s

KERNEL_IMAGE="${KERNEL_IMAGE:=miniOS}"

if [ "${1}" ] ; then
	KERNEL_IMAGE="./miniOS"
fi

( sleep "${BOOT_TIME}" ; echo info registers ) | qemu-system-i386 -kernel "${KERNEL_IMAGE}" -monitor stdio

