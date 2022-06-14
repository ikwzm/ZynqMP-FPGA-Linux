# Build Linux Kernel

There are two ways

1. run scripts/build-linux-5.10.120-zynqmp-fpga-generic.sh (easy)
2. run this chapter step-by-step (annoying)

## Download Linux Kernel Source

### Clone from linux-stable.git

```console
shell$ git clone --depth 1 -b v5.10.120 git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git linux-5.10.120-zynqmp-fpga-generic
```

### Make Branch linux-5.10.120-zynqmp-fpga-generic

```console
shell$ cd linux-5.10.120-zynqmp-fpga-generic
shell$ git checkout -b 5.10.120-zynqmp-fpga-generic refs/tags/v5.10.120
```

## Patch to Linux Kernel

### Patch for linux-xlnx-v2021.2

```console
shell$ sh ../files/linux-5.10.120-xlnx-v2021.2-patchs/0xx_patch.sh
```

### Patch for builddeb

```console
shell$ patch -p1 < ../files/linux-5.10.120-builddeb.diff 
shell$ git add --all
shell$ git commit -m "[update] scripts/package/builddeb to add tools/include and postinst script to header package."
```

### Add ATWILC3000 Linux Driver for Ultra96-V2

```console
shell$ rm -rf drivers/staging/wilc3000
shell$ cp -r ../files/microchip-wilc-driver/wilc1000 drivers/staging/wilc3000
shell$ patch -d drivers/staging/wilc3000 < ../files/microchip-wilc-driver/0001-ultra96-modifications-15.5.patch
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-wilc3000.diff
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-pwrseq-wilc.diff
shell$ git add --all
shell$ git commit -m "[add] drivers/staging/wilc3000"
```

### Patch for zynqmp-fpga

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga.diff 
shell$ git add --all
shell$ git commit -m "[patch] for linux-5.10.120-zynqmp-fpga."
```

### Patch for Ultra96

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-ultra96.diff
shell$ git add --all
shell$ git commit -m "[patch] for Ultra96."
```

### Patch for UltraZed-EG IO Carrier Card

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-uz3eg-iocc.diff 
shell$ git add --all
shell$ git commit -m "[patch] for UltraZed-EG IO Carrier Card."
```

### Patch for Ultra96-V2

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-ultra96v2.diff 
shell$ git add --all
shell$ git commit -m "[patch] for Ultra96-V2."
```

### Patch for Kria KV260

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-kv260.diff
shell$ git add --all
shell$ git commit -m "[patch] for Kria KV260."
```

### Patch for SMB3 and CIFS

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-cifs.diff 
shell$ git add --update
shell$ git commit -m "[add] SMB3 and CIFS."
```

### Patch for Xilinx APF Driver

```console
shell$ patch -p1 < ../files/linux-5.10.120-zynqmp-fpga-apf.diff
shell$ git add --update
shell$ git commit -m "[add] Xilinx APF driver."
```

### Add zynqmp_fpga_generic_defconfig

```console
shell$ cp ../files/zynqmp_fpga_generic_defconfig arch/arm64/configs/
shell$ git add arch/arm64/configs/zynqmp_fpga_generic_defconfig
shell$ git commit -m "[add] zynqmp_fpga_generic_defconfig to arch/arm64/configs"
```


### Create tag and .version

```console
shell$ git tag -a 5.10.120-zynqmp-fpga-generic-0 -m "release 5.10.120-zynqmp-fpga-generic-0"
shell$ echo 0 > .version
```

## Build

### Setup for Build 

```console
shell$ cd linux-5.10.120-zynqmp-fpga-generic
shell$ export ARCH=arm64
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ make zynqmp_fpga_generic_defconfig
```

### Build Linux Kernel and device tree

```console
shell$ export DTC_FLAGS=--symbols
shell$ rm -rf debian
shell$ make deb-pkg
```

### Install kernel image to this repository

```console
shell$ cp arch/arm64/boot/Image.gz ../vmlinuz-5.10.120-zynqmp-fpga-generic-0
shell$ cp .config ../files/config-5.10.120-zynqmp-fpga-generic-0
```

### Install devicetree to target/UltraZed-EG-IOCC/boot/

```console
shell$ cp arch/arm64/boot/dts/xilinx/zynqmp-uz3eg-iocc.dtb ../target/UltraZed-EG-IOCC/boot/devicetree-5.10.120-zynqmp-fpga-generic-uz3eg-iocc.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/UltraZed-EG-IOCC/boot/devicetree-5.10.120-zynqmp-fpga-generic-uz3eg-iocc.dts ../target/UltraZed-EG-IOCC/boot/devicetree-5.10.120-zynqmp-fpga-generic-uz3eg-iocc.dtb
```

### Install devicetree to target/Ultra96/boot/

```console
shell$ cp arch/arm64/boot/dts/xilinx/avnet-ultra96-rev1.dtb ../target/Ultra96/boot/devicetree-5.10.120-zynqmp-fpga-generic-ultra96.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Ultra96/boot/devicetree-5.10.120-zynqmp-fpga-generic-ultra96.dts ../target/Ultra96/boot/devicetree-5.10.120-zynqmp-fpga-generic-ultra96.dtb
```

### Install devicetree to target/Ultra96-V2/boot/

```console
shell$ cp arch/arm64/boot/dts/xilinx/avnet-ultra96v2-rev1.dtb ../target/Ultra96-V2/boot/devicetree-5.10.120-zynqmp-fpga-generic-ultra96v2.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Ultra96-V2/boot/devicetree-5.10.120-zynqmp-fpga-generic-ultra96v2.dts ../target/Ultra96-V2/boot/devicetree-5.10.120-zynqmp-fpga-generic-ultra96v2.dtb
```

### Install devicetree to target/Kv260/boot/

```console
shell$ cp arch/arm64/boot/dts/xilinx/zynqmp-kv260-revB.dtb ../target/Kv260/boot/devicetree-5.10.120-zynqmp-fpga-generic-kv260-revB.dtb
shell$ ./scripts/dtc/dtc -I dtb -O dts --symbols -o ../target/Kv260/boot/devicetree-5.10.120-zynqmp-fpga-generic-kv260-revB.dts ../target/Kv260/boot/devicetree-5.10.120-zynqmp-fpga-generic-kv260-revB.dtb
```

