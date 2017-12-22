### Build ARM Trusted Firmware

#### Fetch Sources

```
shell$ git clone https://github.com/Xilinx/arm-trusted-firmware.git
```

#### Build

```
shell$ cd arm-trusted-firmware
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ make PLAT=zynqmp RESET_TO_BL31=1
```

