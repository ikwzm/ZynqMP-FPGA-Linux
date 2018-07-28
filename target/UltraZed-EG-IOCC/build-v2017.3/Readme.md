Build Boot Loader for UltraZed-EG-IOCC
====================================================================================

## Files that you can to build

* target/UltraZed-EG-IOCC/
  - build/
    + zynqmp_fsbl.elf  (FSBL)
    + zynqmp_pmufw.elf (PMU Firmware)
    + bl31.elf         (ARM Trusted Firmware Boot Loader state 3-1)
    + u-boot.elf       (U-Boot)
  - boot/
    + boot.bin

Build UltraZed-EG-IOCC Sample FPGA
------------------------------------------------------------------------------------

### Requirement

* Vivado 2017.2

### File Description

* target/UltraZed-EG-IOCC/build/
  - fpga/
    + create_project.tcl
    + design_1_bd.tcl
    + implementation.tcl
    + export_hardware.tcl


### Create Project

```console
vivado% vivado -mode batch -source create_project.tcl
```

### Implementation

```console
vivado% vivado -mode batch -source implementation.tcl
```

### Export Hadware to project.sdk

```console
vivado% vivado -mode batch -source export_hardware.tcl
```

Build FSBL
------------------------------------------------------------------------------------

### Requirement

* Vivado SDK 2017.2

### File Description

* target/UltraZed-EG-IOCC/build/
  - fpga/
    + build_zynqmp_fsbl.hsi
  - zynqmp_fsbl.elf

### Build zynqmp_fsbl.elf

```console
vivado% cd target/UltraZed-EG-IOCC/build/fpga/
vivado% hsi -mode tcl -source build_zynqmp_fsbl.hsi
```

Build PMU Firmware
------------------------------------------------------------------------------------

### Requirement

* Vivado SDK 2017.2

### File Description

* target/UltraZed-EG-IOCC/build/
  - fpga/
    + build_zynqmp_pmufw.hsi
    + project.sdk/
  - zynqmp_pmufw.elf

### Build zynqmp_pmufw.elf

```console
vivado% cd target/UltraZed-EG-IOCC/build/fpga/
vivado% hsi -mode tcl -source build_zynqmp_pmufw.hsi
```

Build ARM Trusted Firmware
------------------------------------------------------------------------------------

### Requirement

* Vivado SDK 2017.2 or gcc-aarch64-linux-gnu

### Fetch Sources

```console
vivado% cd target/UltraZed-EG-IOCC/build
vivado% git clone https://github.com/Xilinx/arm-trusted-firmware.git
```

### Build

```console
vivado% cd arm-trusted-firmware
vivado% make CROSS_COMPILE=aarch64-linux-gnu- PLAT=zynqmp RESET_TO_BL31=1
```

If you get the following error and you can not compile

```console
Building zynqmp
  AS      bl31/aarch64/runtime_exceptions.S
bl31/aarch64/runtime_exceptions.S: Assembler messages:
bl31/aarch64/runtime_exceptions.S:157: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:165: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:170: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:175: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:189: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:193: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:197: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:201: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:215: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:219: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:223: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:231: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:245: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:249: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:253: Error: non-constant expression in ".if" statement
bl31/aarch64/runtime_exceptions.S:261: Error: non-constant expression in ".if" statement
Makefile:597: recipe for target 'build/zynqmp/release/bl31/runtime_exceptions.o' failed
make: *** [build/zynqmp/release/bl31/runtime_exceptions.o] Error 1
```

please build using tool chain of Vivado SDK.

```console
vivado% cd arm-trusted-firmware
vivado% make CROSS_COMPILE="<SDK PATH>/gnu/aarch64/<lin or nt>/aarch64-linux/bin/aarch64-linux-gnu-" PLAT=zynqmp RESET_TO_BL31=1
```

```<SDK PATH>``` is Vivado SDK Installed PATH.
```<lin or nt>``` is platform. lin=linux, nt=windows

### Copy bl31.elf

```console
vivado% cp build/zynqmp/release/bl31/bl31.elf ../bl31.elf
```

Build U-Boot
------------------------------------------------------------------------------------

### Requirement

* gcc-aarch64-linux-gnu

### Download U-Boot Source

#### Clone from u-boot-xlnx.git

```console
vivado% cd target/UltraZed-EG-IOCC/build
vivado% git clone https://github.com/Xilinx/u-boot-xlnx.git u-boot-xlnx-v2017.3
```

#### Checkout xilinx-v2017.3

```console
vivado% cd u-boot-xlnx-v2017.3
vivado% git checkout -b xilinx-v2017.3-ultrazed-eg-iocc refs/tags/xilinx-v2017.3
```

### Patch for UltraZed-EG-IOCC

```console
vivado% patch -p0 < ../../../../files/u-boot-xlnx-v2017.3-ultrazed-eg-iocc.diff
vivado% git add arch/arm/dts/zynqmp-uz3eg-iocc.dts
vivado% git add board/xilinx/zynqmp/zynqmp-uz3eg-iocc/psu_init_gpl.c
vivado% git add board/xilinx/zynqmp/zynqmp-uz3eg-iocc/psu_init_gpl.h
vivado% git add configs/xilinx_zynqmp_uz3eg_iocc_defconfig
vivado% git add include/configs/xilinx_zynqmp_uz3eg_iocc.h
vivado% git add --update
vivado% git commit -m "patch for UltraZed-EG-IOCC"
vivado% git tag -a xilinx-v2017.3-ultrazed-eg-iocc-0 -m "release xilinx-v2017.3-ultrazed-eg-iocc release 0"
```

### Patch fix bug for I2C freeze when reboot

```console
vivado% patch -p0 < ../../../../files/u-boot-xlnx-v2017.3-ultrazed-eg-iocc-i2c-patch.diff
vivado% git add --update
vivado% git commit -m "[fix] bug for I2C freeze when reboot"
vivado% git tag -a xilinx-v2017.3-ultrazed-eg-iocc-1 -m "release xilinx-v2017.3-ultrazed-eg-iocc release 1"
```


### Setup for Build 

```console
vivado% cd u-boot-xlnx-v2017.3
vivado% export ARCH=arm
vivado% export CROSS_COMPILE=aarch64-linux-gnu-
vivado% make xilinx_zynqmp_uz3eg_iocc_defconfig
```

### Build u-boot.elf

```console
vivado% make
```

### Copy u-boot.elf

```console
vivado% cp u-boot.elf ../u-boot.elf
```

Build boot.bin
------------------------------------------------------------------------------------

## Requirement

* Vivado SDK 2017.2

### File Description

* target/UltraZed-EG-IOCC/
  - build/
    + zynqmp_fsbl.elf  (Built by "Build FSBL")
    + zynqmp_pmufw.elf (Built by "Build PMU Firmware")
    + bl31.elf         (Built by "Build ARM Trusted Firmware")  
    + u-boot.elf       (Built by "Build U-Boot")
    + boot.bif
  - boot/
    + boot.bin

### Build boot.bin

```console
vivado% cd target/UltraZed-EG-IOCC/build
vivado% bootgen -arch zynqmp -image boot.bif -w -o ../boot/boot.bin
```

