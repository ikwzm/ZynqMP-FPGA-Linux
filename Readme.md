ZynqMP-FPGA-Linux
====================================================================================

Overview
------------------------------------------------------------------------------------

### Introduction

This Repository provides a Linux Boot Image(U-boot, Kernel, Root-fs) for Zynq MPSoC.

### Features

* Hardware
  + UltraZed-EG-IOCC : Xilinx Zynq UltraScale+ MPSoC Starter Kit by Avnet.
  + Ultra96    : Xilinx Zynq UltraScale+ MPSoC development board based on the Linaro 96Boards specification. 
  + Ultra96-V2 : updates and refreshes the Ultra96 product that was released in 2018.
* Boot Loader
  + FSBL(First Stage Boot Loader for ZynqMP)
  + PMU Firmware(Platform Management Unit Firmware)
  + BL31(ARM Trusted Firmware Boot Loader stage 3-1)
  + U-Boot xilinx-v2019.1 (customized)
* Linux Kernel Version v4.19.0
  + [linux-xlnx](https://github.com/Xilinx/linux-xlnx) tag=xilinx-v2019.1
  + Enable Device Tree Overlay with Configuration File System
  + Enable FPGA Manager
  + Enable FPGA Bridge
  + Enable FPGA Reagion
* Debian10(buster) Root File System
  + Installed build-essential
  + Installed device-tree-compiler
  + Installed ruby ruby-msgpack ruby-serialport
  + Installed python python3 msgpack-rpc-python
  + Installed u-boot-tools
  + Installed Other package list -> [files/debian10-dpkg-list.txt](files/debian10-dpkg-list.txt)
* FPGA Device Drivers and Services
  + [fclkcfg    (FPGA Clock Configuration Device Driver)](https://github.com/ikwzm/fclkcfg)
  + [udmabuf    (User space mappable DMA Buffer)](https://github.com/ikwzm/udmabuf)

Install
------------------------------------------------------------------------------------

* Install U-Boot and Linux to SD-Card
  + [UltraZed-EG-IOCC](doc/install/ultrazed-eg-iocc.md)
  + [Ultra96](doc/install/ultra96.md)
  + [Ultra96-V2](doc/install/ultra96v2.md)

Build 
------------------------------------------------------------------------------------

* [Build Boot Loader for UltraZed-EG-IOCC](target/UltraZed-EG-IOCC/build-v2019.1/Readme.md)
* [Build Boot Loader for Ultra96](target/Ultra96/build-v2019.1/Readme.md)
* [Build Boot Loader for Ultra96-V2](target/Ultra96-V2/build-v2019.1/Readme.md)
* [Build Linux Kernel](doc/build/linux-xlnx-v2019.1-zynqmp-fpga.md)
* [Build Debian10 RootFS](doc/build/debian10-rootfs.md)
* [Build Device Drivers](doc/build/device-drivers.md)
