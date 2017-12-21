#!/bin/bash

CURRENT_DIR=`pwd`
LINUX_BUILD_DIR=linux-xlnx-v2017.3-zynqmp-fpga

### Download Linux Kernel Source
git clone https://github.com/Xilinx/linux-xlnx.git $LINUX_BUILD_DIR
cd $LINUX_BUILD_DIR
git git checkout -b linux-xlnx-v2017.3-zynqmp-fpga refs/tags/xilinx-v2017.3

### Patch for linux-xlnx-v2017.3-zynqmp-fpga
patch -p0 < ../files/linux-xlnx-v2017.3-zynqmp-fpga.diff
git add --update
git add arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dts
git commit -m "[patch] for linux-xlnx-v2017.3-zynqmp-fpga."

### Patch for linux-xlnx-v2017.3-builddeb
patch -p0 < ../files/linux-xlnx-v2017.3-builddeb.diff
git add --update
git commit -m "[fix] build wrong architecture debian package when ARCH=arm64 and cross compile."

### Create tag and .version
git tag -a xilinx-v2017.3-zynqmp-fpga -m "release xilinx-v2017.3-zynqmp-fpga"
echo 0 > .version

### Setup for Build 
export ARCH=arm64
export export CROSS_COMPILE=aarch64-linux-gnu-
make xilinx_zynqmp_defconfig

### Build Linux Kernel and device tree
export DTC_FLAGS=--symbols
make deb-pkg

#### Build uImage and devicetree to target/zybo-pynqz1/boot/

mkimage -A arm64 -O linux -a 0x80000 -e 0x80000 -n 'Linux-4.9.0-xlnx-v2017.3-fpga' -d arch/arm64/boot/Image.gz ../target/UltraZed-EG-IOCC/boot/uImage-4.9.0-xlnx-v2017.3-zynqmp-fpga
cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dtb
dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dtb

cd ..
