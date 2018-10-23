ZynqMP-FPGA-Linux
====================================================================================

Overview
------------------------------------------------------------------------------------

### Introduction

This Repository provides a Linux Boot Image(U-boot, Kernel, Root-fs) for Zynq MPSoC.

### Features

* Hardware
  + UltraZed-EG-IOCC : Xilinx Zynq UltraScale+ MPSoC Starter Kit by Avnet.
  + Ultra96 : Xilinx Zynq UltraScale+ MPSoC development board based on the Linaro 96Boards specification. 
* Boot Loader
  + FSBL(First Stage Boot Loader for ZynqMP)
  + PMU Firmware(Platform Management Unit Firmware)
  + BL31(ARM Trusted Firmware Boot Loader stage 3-1)
  + U-Boot xilinx-v2018.2 (customized)
* Linux Kernel Version v4.14.0
  + [linux-xlnx](https://github.com/Xilinx/linux-xlnx) tag=xilinx-v2018.2
  + Enable Device Tree Overlay with Configuration File System
  + Enable FPGA Manager
  + Enable FPGA Bridge
  + Enable FPGA Reagion
* Debian9(stretch) Root File System
  + Installed build-essential
  + Installed device-tree-compiler
  + Installed ruby ruby-msgpack ruby-serialport
  + Installed python python3 msgpack-rpc-python
  + Installed u-boot-tools
  + Installed Other package list -> [files/dpkg-list.txt](files/dpkg-list.txt)
* FPGA Device Drivers and Services
  + [fclkcfg    (FPGA Clock Configuration Device Driver)](https://github.com/ikwzm/fclkcfg)
  + [udmabuf    (User space mappable DMA Buffer)](https://github.com/ikwzm/udmabuf)

Install
------------------------------------------------------------------------------------

* Install U-Boot and Linux to SD-Card
  + [UltraZed-EG-IOCC](doc/install/ultrazed-eg-iocc.md)
  + [Ultra96](doc/install/ultra96.md)

Build 
------------------------------------------------------------------------------------

* [Build Boot Loader for UltraZed-EG-IOCC](target/UltraZed-EG-IOCC/build-v2018.2/Readme.md)
* [Build Boot Loader for Ultra96](target/Ultra96/build-v2018.2/Readme.md)
* [Build Linux Kernel](doc/build/linux-xlnx-v2018.2-zynqmp-fpga.md)
* [Build Debian9 RootFS](doc/build/debian9-rootfs.md)
* [Build Device Drivers](doc/build/device-drivers.md)
