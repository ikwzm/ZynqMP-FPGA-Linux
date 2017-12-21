### Build Linux Kernel

There are two ways

1. run scripts/build-linux-xlnx-v2017.3-zynqmp-fpga.sh (easy)
2. run this chapter step-by-step (annoying)

#### Download Linux Kernel Source

##### Clone from linux-xlnx.git

```
shell$ git clone https://github.com/Xilinx/linux-xlnx.git linux-xlnx-v2017.3-zynqmp-fpga
```

##### Checkout xilinx-v2017.3

```
shell$ cd linux-xlnx-v2017.3-zynqmp-fpga
shell$ git checkout -b linux-xlnx-v2017.3-zynqmp-fpga refs/tags/xilinx-v2017.3
```

#### Patch for linux-xlnx-v2017.3-zynqmp-fpga

```
shell$ patch -p0 < ../files/linux-xlnx-v2017.3-zynqmp-fpga.diff
shell$ git add --update
shell$ git add arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dts
shell$ git commit -m "[patch] for linux-xlnx-v2017.3-zynqmp-fpga."
```

#### Patch for linux-xlnx-v2017.3-builddeb

```
shell$ patch -p0 < ../files/linux-xlnx-v2017.3-builddeb.diff
shell$ git add --update
shell$ git commit -m "[fix] build wrong architecture debian package when ARCH=arm64 and cross compile."
```

###

##### Create tag and .version

```
shell$ git tag -a xilinx-v2017.3-zynqmp-fpga -m "release xilinx-v2017.3-zynqmp-fpga"
shell$ echo 0 > .version
```

#### Setup for Build 

````
shell$ cd linux-xlnx-v2017.3-zynqmp-fpga
shell$ export ARCH=arm64
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ make xilinx_zynqmp_defconfig
````

#### Build Linux Kernel and device tree

````
shell$ export DTC_FLAGS=--symbols
shell$ make deb-pkg
````

#### Build uImage and devicetree to target/zybo-pynqz1/boot/

```
shell$ mkimage -A arm64 -O linux -a 0x80000 -e 0x80000 -n 'Linux-4.9.0-xlnx-v2017.3-fpga' -d arch/arm64/boot/Image.gz ../target/UltraZed-EG-IOCC/boot/uImage-4.9.0-xlnx-v2017.3-zynqmp-fpga
shell$ cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dtb
shell$ dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dtb
```
