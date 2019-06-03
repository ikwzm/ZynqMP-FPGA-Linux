### Build Linux Kernel

There are two ways

1. run scripts/build-linux-xlnx-v2019.1-zynqmp-fpga.sh (easy)
2. run this chapter step-by-step (annoying)

#### Download Linux Kernel Source

##### Clone from linux-xlnx.git

```console
shell$ git clone --depth 1 -b xilinx-v2019.1 https://github.com/Xilinx/linux-xlnx.git linux-xlnx-v2019.1-zynqmp-fpga
```

##### Checkout xilinx-v2019.1

```console
shell$ cd linux-xlnx-v2019.1-zynqmp-fpga
shell$ git checkout -b linux-xlnx-v2019.1-zynqmp-fpga refs/tags/xilinx-v2019.1
```

#### Patch for linux-xlnx-v2019.1-zynqmp-fpga

```console
shell$ patch -p1 < ../files/linux-xlnx-v2019.1-zynqmp-fpga.diff
shell$ git add --update
shell$ git add arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dts
shell$ git add arch/arm64/boot/dts/xilinx/zynqmp-ultra96.dts 
shell$ git commit -m "[patch] for linux-xlnx-v2019.1-zynqmp-fpga."
```

#### Patch for linux-xlnx-v2019.1-builddeb

```console
shell$ patch -p1 < ../files/linux-xlnx-v2019.1-builddeb.diff
shell$ git add --update
shell$ git commit -m "[update] scripts/package/builddeb to add tools/include and postinst script to header package"
```

#### Patch for linux-xlnx-v2019.1-zynqmp-fpga-patch

```console
shell$ patch -p1 < ../files/linux-xlnx-v2019.1-zynqmp-fpga-patch.diff
shell$ git add --update
shell$ git commit -m "[patch] drivers/fpga/zynqmp-fpga.c for load raw file format"
```

###

##### Create tag and .version

```console
shell$ git tag -a xilinx-v2019.1-zynqmp-fpga -m "release xilinx-v2019.1-zynqmp-fpga"
shell$ echo 0 > .version
```

#### Setup for Build 

````console
shell$ cd linux-xlnx-v2019.1-zynqmp-fpga
shell$ export ARCH=arm64
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ make xilinx_zynqmp_defconfig
````

#### Build Linux Kernel and device tree

````console
shell$ export DTC_FLAGS=--symbols
shell$ make deb-pkg
````

#### Build kernel image and devicetree to target/UltraZed-EG-IOCC/boot/

````console
shell$ cp arch/arm64/boot/Image ../target/UltraZed-EG-IOCC/boot/image-4.19.0-xlnx-v2019.1-zynqmp-fpga
shell$ cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-uz3eg-iocc.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-uz3eg-iocc.dtb
````

#### Build kernel image and devicetree to target/Ultra96/boot/

````console
shell$ cp arch/arm64/boot/Image ../target/Ultra96/boot/image-4.19.0-xlnx-v2019.1-zynqmp-fpga
shell$ cp arch/arm64/boot/dts/xilinx/avnet-ultra96-rev1.dtb ../target/Ultra96/boot/devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-ultra96.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Ultra96/boot/devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-ultra96.dts ../target/Ultra96/boot/devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-ultra96.dtb
```
