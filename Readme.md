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
  + U-Boot xilinx-v2019.2 (customized)
* Linux Kernel Version v5.4.0
  + [linux-xlnx](https://github.com/Xilinx/linux-xlnx) tag=xilinx-v2020.2
  + Enable Device Tree Overlay with Configuration File System
  + Enable FPGA Manager
  + Enable FPGA Bridge
  + Enable FPGA Reagion
  + Enable ATWILC3000 Linux Driver for Ultra96-V2
  + Enable CIFS (Common Internet File System)
  + Enable Xilinx APF Accelerator driver
  + Enable Xilinx APF DMA engines support
* Debian10.9(buster) Root File System
  + Installed build-essential
  + Installed device-tree-compiler
  + Installed ruby ruby-msgpack ruby-serialport
  + Installed python python3 msgpack-rpc-python
  + Installed u-boot-tools
  + Installed Other package list -> [files/debian10-dpkg-list.txt](files/debian10-dpkg-list.txt)
* FPGA Device Drivers and Services
  + [fclkcfg    (FPGA Clock Configuration Device Driver)](https://github.com/ikwzm/fclkcfg)
  + [u-dma-buf  (User space mappable DMA Buffer)](https://github.com/ikwzm/udmabuf)

Install
------------------------------------------------------------------------------------

* Install U-Boot and Linux to SD-Card
  + [UltraZed-EG-IOCC](doc/install/ultrazed-eg-iocc.md)
  + [Ultra96](doc/install/ultra96.md)
  + [Ultra96-V2](doc/install/ultra96v2.md)

Build 
------------------------------------------------------------------------------------

* [Build Boot Loader for UltraZed-EG-IOCC](target/UltraZed-EG-IOCC/build-v2019.2/Readme.md)
* [Build Boot Loader for Ultra96](target/Ultra96/build-v2019.2/Readme.md)
* [Build Boot Loader for Ultra96-V2](target/Ultra96-V2/build-v2019.2/Readme.md)
* [Build Linux Kernel](doc/build/linux-xlnx-v2020.2-zynqmp-fpga.md)
* [Build Debian10 RootFS](doc/build/debian10-rootfs.md)
* [Build Device Drivers](doc/build/device-drivers.md)

Other Projects
------------------------------------------------------------------------------------

* https://github.com/ikwzm/ZynqMP-FPGA-Ubuntu18.04-Ultra96
  + Linux Boot Image(U-boot, Kernel, Ubuntu 18.04 Desktop) for Ultra96/Ultra96-V2
* https://github.com/ikwzm/ZynqMP-FPGA-Xserver
  + The X-Window server Debian Package for ZynqMP-FPGA-Linux
* https://github.com/ikwzm/ZynqMP-FPGA-XRT
  + The XRT(Xilinx Runtime) Debian Package for ZynqMP-FPGA-Linux


Examples
------------------------------------------------------------------------------------

* https://github.com/ikwzm/ZynqMP-FPGA-XRT-Example-1-Ultra96
  + Example for ZynqMP-FPGA-XRT
* https://github.com/ikwzm/ArgSort-Ultra96
  + ArgSort for Ultra96
* https://github.com/ikwzm/ZynqMP-FPGA-Linux-Example-2-Ultra96
  + ZynqMP-FPGA-Linux Example (2) binary and test code for Ultra96
* https://github.com/ikwzm/ZynqMP-FPGA-Linux-Example-0-UltraZed
  + ZynqMP-FPGA-Linux Example (0) binary and test code for UltraZed-EG-IOCC
* https://github.com/ikwzm/ZynqMP-FPGA-Linux-Example-2-UltraZed
  + ZynqMP-FPGA-Linux Example (2) binary and test code for UltraZed-EG-IOCC
* https://github.com/ikwzm/ZynqMP-FPGA-Linux-Example-3-UltraZed
  + ZynqMP-FPGA-Linux Example (3) binary and test code for UltraZed-EG-IOCC
