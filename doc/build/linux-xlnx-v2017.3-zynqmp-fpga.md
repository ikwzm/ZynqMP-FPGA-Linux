### Build Linux Kernel

There are two ways

1. run scripts/build-linux-xlnx-v2017.3-zynqmp-fpga.sh (easy)
2. run this chapter step-by-step (annoying)

#### Download Linux Kernel Source

##### Clone from linux-xlnx.git

```console
shell$ git clone https://github.com/Xilinx/linux-xlnx.git linux-xlnx-v2017.3-zynqmp-fpga
```

##### Checkout xilinx-v2017.3

```console
shell$ cd linux-xlnx-v2017.3-zynqmp-fpga
shell$ git checkout -b linux-xlnx-v2017.3-zynqmp-fpga refs/tags/xilinx-v2017.3
```

#### Patch for linux-xlnx-v2017.3-zynqmp-fpga

```console
shell$ patch -p0 < ../files/linux-xlnx-v2017.3-zynqmp-fpga.diff
shell$ git add --update
shell$ git add arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dts
shell$ git commit -m "[patch] for linux-xlnx-v2017.3-zynqmp-fpga."
```

#### Patch for linux-xlnx-v2017.3-builddeb

```console
shell$ patch -p0 < ../files/linux-xlnx-v2017.3-builddeb.diff
shell$ git add --update
shell$ git commit -m "[fix] build wrong architecture debian package when ARCH=arm64 and cross compile."
```

#### Patch for linux-xlnx-v2017.3-builddeb2

```console
shell$ patch -p0 < ../files/linux-xlnx-v2017.3-builddeb2.diff
shell$ git add --update
shell$ git commit -m "[update] scripts/package/builddeb to add tools/include and postinst script to header package"
```

###

##### Create tag and .version

```console
shell$ git tag -a xilinx-v2017.3-zynqmp-fpga -m "release xilinx-v2017.3-zynqmp-fpga"
shell$ echo 1 > .version
```

#### Setup for Build 

````console
shell$ cd linux-xlnx-v2017.3-zynqmp-fpga
shell$ export ARCH=arm64
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ make xilinx_zynqmp_defconfig
````

#### Build Linux Kernel and device tree

````console
shell$ export DTC_FLAGS=--symbols
shell$ make deb-pkg
````

#### Build uImage and devicetree to target/zybo-pynqz1/boot/

```console
shell$ cp arch/arm64/boot/Image ../target/UltraZed-EG-IOCC/boot/image-4.9.0-xlnx-v2017.3-fpga
shell$ cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-fpga-zynqmp-uz3eg-iocc.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-fpga-zynqmp-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-fpga-zynqmp-uz3eg-iocc.dtb
```
