ZynqMP-FPGA-Linux
====================================================================================

Overview
------------------------------------------------------------------------------------

### Introduction

This Repository provides a Linux Boot Image(U-boot, Kernel, Root-fs) for Zynq MPSoC.

### Notice

This repository is currently no longer being updated.
The reason is that this repository contains many binary files and has become difficult to maintain.
We have decided to provide a separate repository for each distribution from now on.
Please refer to the following repositories

 * Debian11
   - https://github.com/ikwzm/ZynqMP-FPGA-Debian11
 * Debian12
   - https://github.com/ikwzm/ZynqMP-FPGA-Debian12
 * Ubuntu22.04
   - https://github.com/ikwzm/ZynqMP-FPGA-Ubuntu22.04-Console
   - https://github.com/ikwzm/ZynqMP-FPGA-Ubuntu22.04-Desktop

### Features

* Hardware
  + UltraZed-EG-IOCC : Xilinx Zynq UltraScale+ MPSoC Starter Kit by Avnet.
  + Ultra96    : Xilinx Zynq UltraScale+ MPSoC development board based on the Linaro 96Boards specification. 
  + Ultra96-V2 : updates and refreshes the Ultra96 product that was released in 2018.
  + KV260 : Kria KV260 Vision AI Startar Kit.
* Boot Loader
  + FSBL(First Stage Boot Loader for ZynqMP)
  + PMU Firmware(Platform Management Unit Firmware)
  + BL31(ARM Trusted Firmware Boot Loader stage 3-1)
  + U-Boot xilinx-v2019.2 (customized)
* Linux Kernel
  + [linux-xlnx](https://github.com/Xilinx/linux-xlnx)
  + Enable Device Tree Overlay with Configuration File System
  + Enable FPGA Manager
  + Enable FPGA Bridge
  + Enable FPGA Reagion
  + Enable ATWILC3000 Linux Driver for Ultra96-V2
* Debian Root File System
  + Installed build-essential
  + Installed device-tree-compiler
  + Installed ruby ruby-msgpack ruby-serialport
  + Installed python python3 msgpack-rpc-python
  + Installed u-boot-tools

Release History
------------------------------------------------------------------------------------

**Note: Downloading the entire repository takes time, so download the source code from https://github.com/ikwzm/ZynqMP-FPGA-Linux/releases.**

| Release    | Released   | Debian Version | Linux Kernel Version            | Release Tag |
|:-----------|:-----------|:---------------|:--------------------------------|:------------|
| v2021.1.2  | 2022-7-27  | Debian 11.1    | 5.10.0-xlnx-v2021.1-zynqmp-fpga | [v2021.1.2](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2021.1.2) 
| v2021.1.1  | 2022-11-25 | Debian 11.1    | 5.10.0-xlnx-v2021.1-zynqmp-fpga | [v2021.1.1](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2021.1.1) 
| v2020.2.1  | 2021-3-30  | Debian 10.9    | 5.4.0-xlnx-v2020.2-zynqmp-fpga  | [v2020.2.1](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2020.2.1) 
| v2020.1.1  | 2020-6-22  | Debian 10.2    | 5.4.0-xlnx-v2020.1-zynqmp-fpga  | [v2020.1.1](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2020.1.1) 
| v2019.2.1  | 2020-1-4   | Debian 10.2    | 4.19.0-xlnx-v2019.2-zynqmp-fpga | [v2019.2.1](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2019.2.1) 
| v2019.1.2  | 2019-8-18  | Debian 10.0    | 4.19.0-xlnx-v2019.1-zynqmp-fpga | [v2019.1.2](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2019.1.2) 
| v2018.2.1  | 2018-11-4  | Debian 9.5     | 4.14.0-xlnx-v2018.2-zynqmp-fpga | [v2018.2.1](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2018.2.1) 
| v2017.3.0  | 2018-7-28  | Debian 9       | 4.9.0-xlnx-v2017.3-zynqmp-fpga  | [v2017.3.0](https://github.com/ikwzm/ZynqMP-FPGA-Linux/tree/v2017.3.0) 

Examples
------------------------------------------------------------------------------------

* https://github.com/ikwzm/ZynqMP-FPGA-XRT-Example-1-Ultra96
  + Example for ZynqMP-FPGA-XRT
* https://github.com/ikwzm/ArgSort-Kv260
  + ArgSort for Kv260
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
