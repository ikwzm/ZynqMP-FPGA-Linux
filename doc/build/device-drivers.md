Build Device Drivers and Services Package
====================================================================================

u-dma-buf-5.4.0-xlnx-v2020.1-zynqmp-fpga_3.0.1-0_arm64.deb 
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v3.0.1 git://github.com/ikwzm/u-dma-buf-kmod-dpkg
shell$ cd u-dma-buf-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2020.1-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=5.4.0-xlnx-v2020.1-zynqmp-fpga kernel_src_dir=../../linux-xlnx-v2020.1-zynqmp-fpga binary
    :
    :
    :
shell$ file ../u-dma-buf-5.4.0-xlnx-v2020.1-zynqmp-fpga_3.0.0-1_arm64.deb 
../u-dma-buf-5.4.0-xlnx-v2020.1-zynqmp-fpga_3.0.1-0_arm64.deb: Debian binary package (format 2.0)
```

fclkcfg-5.4.0-xlnx-v2020.1-zynqmp-fpga_1.3.0-1_arm64.deb
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v1.3.0 git://github.com/ikwzm/fclkcfg-kmod-dpkg
shell$ cd fclkcfg-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2020.1-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=5.4.0-xlnx-v2020.1-zynqmp-fpga kernel_src_dir=../../linux-xlnx-v2020.1-zynqmp-fpga binary
    :
    :
    :
shell$ file ../fclkcfg-5.4.0-xlnx-v2020.1-zynqmp-fpga_1.3.0-1_arm64.deb 
../fclkcfg-5.4.0-xlnx-v2020.1-zynqmp-fpga_1.3.0-1_arm64.deb: Debian binary package (format 2.0)
```


