#!/bin/bash

CURRENT_DIR=`pwd`
LINUX_BUILD_DIR=linux-xlnx-v2020.2-zynqmp-fpga

## Download ATWILC3000 Linux Driver for Ultra96-V2

if [ ! -d ./u96v2-wilc-driver ]; then
    git clone https://github.com/Avnet/u96v2-wilc-driver.git
fi

## Download Linux Kernel Source
git clone --depth 1 -b xilinx-v2020.2 https://github.com/Xilinx/linux-xlnx.git $LINUX_BUILD_DIR
cd $LINUX_BUILD_DIR
git checkout -b linux-xlnx-v2020.2-zynqmp-fpga refs/tags/xilinx-v2020.2

## Patch for linux-xlnx-v2020.2-zynqmp-fpga
patch -p1 < ../files/linux-xlnx-v2020.2-zynqmp-fpga.diff
git add --update
git add arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dts
git commit -m "[patch] for linux-xlnx-v2020.2-zynqmp-fpga."

## Patch for linux-xlnx-v2020.2-builddeb
patch -p1 < ../files/linux-xlnx-v2020.2-builddeb.diff
git add --update
git commit -m "[update] scripts/package/builddeb to add tools/include and postinst script to header package."

## Add ATWILC3000 Linux Driver for Ultra96-V2

cp -r ../u96v2-wilc-driver/wilc drivers/staging/wilc3000/
patch -p1 < ../files/linux-xlnx-v2020.2-zynqmp-fpga-wilc3000.diff
git add --update
git add drivers/staging/wilc3000
git commit -m "[add] drivers/staging/wilc3000"

## Patch for Ultra96-V2

patch -p1 < ../files/linux-xlnx-v2020.2-zynqmp-fpga-ultra96v2.diff
git add --update
git add arch/arm64/boot/dts/xilinx/avnet-ultra96v2-rev1.dts 
git commit -m "[add] devicetree for Ultra96-V2."

## Patch for SMB3 and CIFS

patch -p1 < ../files/linux-xlnx-v2020.2-zynqmp-fpga-cifs.diff
git add --update
git commit -m "[add] SMB3 and CIFS."

## Patch for Xilinx APF Driver

patch -p1 < ../files/linux-xlnx-v2020.2-zynqmp-fpga-apf.diff
git add --update
git commit -m "[add] Xilinx APF driver."

## Create tag and .version
git tag -a xilinx-v2020.2-zynqmp-fpga-3 -m "release xilinx-v2020.2-zynqmp-fpga-3"
echo 3 > .version

## Setup for Build 
export ARCH=arm64
export export CROSS_COMPILE=aarch64-linux-gnu-
make xilinx_zynqmp_defconfig

## Build Linux Kernel and device tree
export DTC_FLAGS=--symbols
make deb-pkg

## Build kernel image and devicetree to target/UltraZed-EG-IOCC/boot/
cp arch/arm64/boot/Image ../target/UltraZed-EG-IOCC/boot/image-5.4.0-xlnx-v2020.2-zynqmp-fpga
cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-uz3eg-iocc.dtb
./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-uz3eg-iocc.dtb

## Build kernel image and devicetree to target/Ultra96/boot/
cp arch/arm64/boot/Image ../target/Ultra96/boot/image-5.4.0-xlnx-v2020.2-zynqmp-fpga
cp arch/arm64/boot/dts/xilinx/avnet-ultra96-rev1.dtb ../target/Ultra96/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96.dtb
./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Ultra96/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96.dts ../target/Ultra96/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96.dtb

## Build kernel image and devicetree to target/Ultra96-V2/boot/
cp arch/arm64/boot/Image ../target/Ultra96-V2/boot/image-5.4.0-xlnx-v2020.2-zynqmp-fpga
cp arch/arm64/boot/dts/xilinx/avnet-ultra96v2-rev1.dtb ../target/Ultra96-V2/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dtb
./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Ultra96-V2/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dts ../target/Ultra96-V2/boot/devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dtb

cd ..
