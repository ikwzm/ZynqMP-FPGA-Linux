#!/bin/bash

ARCH=arm64
LINUX_KERNEL_VERSION="5.10.120"
LINUX_LOCAL_VERSION="zynqmp-fpga-generic"
LINUX_BUILD_VERSION=0
LINUX_ROOT_ARCHIVE="debian11-rootfs-vanilla.tgz.files"
DEVICE_DRIVER_LIST=("u-dma-buf" "fclkcfg")
SCRIPTS_DIR=$(cd $(dirname $0); pwd)

. $SCRIPTS_DIR/install-sd.sh

