#!/bin/bash

QEMU_BIN=qemu-system-i386

BOOT_TIME=6s

KERNEL_IMAGE="${KERNEL_IMAGE:=miniOS}"

if [ "${1}" ] ; then
	KERNEL_IMAGE="./miniOS"
fi

if ! which "${QEMU_BIN}" &>/dev/null ; then
	QEMU_BIN=qemu
fi

( sleep "${BOOT_TIME}" ; echo info registers ) | ${QEMU_BIN} -kernel "${KERNEL_IMAGE}" -display sdl -monitor stdio -no-shutdown -no-reboot $*

