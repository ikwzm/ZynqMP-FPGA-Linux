### Build UltraZed-EG-IOCC ZynqMP FSBL

#### What is FSBL

First Stage Bootloader (FSBL) for Zynq UltraScale+ MPSoC configures the FPGA with hardware bitstream (if it exists) and loads the Operating System (OS) Image or Standalone (SA) Image or 2nd Stage Boot Loader image from the non-volatile memory (NAND/SD/eMMC/QSPI) to Memory (DDR/TCM/OCM) and takes A53/R5 out of reset. It supports multiple partitions, and each partition can be a code image or a bitstream. Each of these partitions, if required, will be authenticated and/or decrypted.
FSBL is loaded into OCM and handed off by CSU BootROM after authenticating and/or decrypting (as required) FSBL.

#### Build UltraZed-EG-IOCC Hardware

[Build UltraZed-EG-IOCC Hardware](./ultrazed-eg-iocc-hardware.md)

#### Build zynqmp_fsbl.elf

```
shell$ cd target/UltraZed-EG-IOCC/vivado-project/
shell$ hsi -mode tcl -source build_zynqmp_fsbl.hsi
```

