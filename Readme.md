ZynqMP-FPGA-Linux
====================================================================================

Overview
------------------------------------------------------------------------------------

### Introduction

This Repository provides a Linux Boot Image(U-boot, Kernel, Root-fs) for Zynq MPSoC.

### Features

* Hardware
  + UltraZed-EG-IOCC : Xilinx Zynq UltraScale+ MPSoC
* Linux Kernel Version v4.9 
  + [linux-xlnx](https://github.com/Xilinx/linux-xlnx) tag=xilinx-v2017.3
  + Enable Device Tree Overlay with Configuration File System
  + Enable FPGA Manager
  + Enable FPGA Bridge
  + Enable FPGA Reagion

Install
------------------------------------------------------------------------------------

COMING SOON.

Build 
------------------------------------------------------------------------------------

* [Build Boot Loader for UltraZed-EG-IOCC](target/UltraZed-EG-IOCC/build/Readme.md)
* [Build Linux Kernel](doc/build/linux-xlnx-v2017.3-zynqmp-fpga.md)
* [Build Debian9 RootFS](doc/build/debian9-rootfs.md)

