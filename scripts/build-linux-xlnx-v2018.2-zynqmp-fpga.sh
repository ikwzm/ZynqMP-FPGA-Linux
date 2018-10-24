#!/bin/bash

CURRENT_DIR=`pwd`
LINUX_BUILD_DIR=linux-xlnx-v2018.2-zynqmp-fpga

### Download Linux Kernel Source
git clone --depth 1 -b xilinx-v2018.2 https://github.com/Xilinx/linux-xlnx.git $LINUX_BUILD_DIR
cd $LINUX_BUILD_DIR
git checkout -b linux-xlnx-v2018.2-zynqmp-fpga

### Patch for linux-xlnx-v2018.2-zynqmp-fpga
patch -p1 < ../files/linux-xlnx-v2018.2-zynqmp-fpga.diff
git add --update
git add arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dts
git add arch/arm64/boot/dts/xilinx/zynqmp-ultra96.dts 
git commit -m "[patch] for linux-xlnx-v2018.2-zynqmp-fpga."

### Patch for linux-xlnx-v2018.2-builddeb
patch -p1 < ../files/linux-xlnx-v2018.2-builddeb.diff
git add --update
git commit -m "[fix] build wrong architecture debian package when ARCH=arm64 and cross compile."

### Patch for linux-xlnx-v2018.2-zynqmp-fpga-patch

patch -p1 < ../files/linux-xlnx-v2018.2-zynqmp-fpga-patch.diff
git add --update
git commit -m "[patch] drivers/fpga/zynqmp-fpga.c for load raw file format"

### Create tag and .version
git tag -a xilinx-v2018.2-zynqmp-fpga -m "release xilinx-v2018.2-zynqmp-fpga"
echo 0 > .version

### Setup for Build 
export ARCH=arm64
export export CROSS_COMPILE=aarch64-linux-gnu-
make xilinx_zynqmp_defconfig

### Build Linux Kernel and device tree
export DTC_FLAGS=--symbols
make deb-pkg

#### Build kernel image and devicetree to target/UltraZed-EG-IOCC/boot/

cp arch/arm64/boot/Image ../target/UltraZed-EG-IOCC/boot/image-4.14.0-xlnx-v2018.2-zynqmp-fpga
cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-uz3eg-iocc.dtb
./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-uz3eg-iocc.dtb

#### Build kernel image and devicetree to target/Ultra96/boot/

cp arch/arm64/boot/Image ../target/Ultra96/boot/image-4.14.0-xlnx-v2018.2-zynqmp-fpga
cp arch/arm64/boot/dts/xilinx/zynqmp-ultra96.dtb ../target/Ultra96/boot/devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-ultra96.dtb
./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Ultra96/boot/devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-ultra96.dts ../target/Ultra96/boot/devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-ultra96.dtb

cd ..
