### Build UltraZed-EG-IOCC PMU Firmware

#### Overview of PMU Firmware

The Platform Management Unit (PMU) in Zynq MPSoC has a Microblaze with 32 KB of ROM and 128 KB of RAM. The ROM is pre-loaded with PMU Boot ROM (PBR) which performs pre-boot tasks and enters a service mode. For more details on PMU, PBR and PMUFW load sequence, refer to Platform Management Unit (Chapter-6) in Zynq MPSoC TRM (UG1085). PMU RAM can be loaded with a firmware (PMU Firmware) at run-time and can be used to extend or customize the functionality of PMU. Some part of the RAM is reserved for PBR, leaving around 125.7 KB for PMU Firmware.

The PMU Firmware provided in SDK, comes with a default linker script which aligns with this memory layout and there is no specific customization required from user.

#### Build UltraZed-EG-IOCC Hardware

[Build UltraZed-EG-IOCC Hardware](./ultrazed-eg-iocc-hardware.md)

#### Build zynqmp_pmufw.elf

```
shell$ cd target/UltraZed-EG-IOCC/vivado-project/
shell$ hsi -mode tcl -source build_zynqmp_pmufw.hsi
```

