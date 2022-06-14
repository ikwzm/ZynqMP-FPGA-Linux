Build Device Drivers and Services Package
====================================================================================

u-dma-buf-5.10.120-zynqmp-fpga-generic_3.2.4-0_arm64.deb
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v3.2.4 git://github.com/ikwzm/u-dma-buf-kmod-dpkg
shell$ cd u-dma-buf-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2020.1-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=5.10.120-zynqmp-fpga-generic kernel_src_dir=../../linux-5.10.120-zynqmp-fpga-generic binary
    :
    :
    :
shell$ file ../u-dma-buf-5.10.120-zynqmp-fpga-generic_3.2.4-0_arm64.deb 
../u-dma-buf-5.10.120-zynqmp-fpga-generic_3.2.4-0_arm64.deb: Debian binary package (format 2.0)
```

fclkcfg-5.10.120-zynqmp-fpga-generic_1.7.2-1_arm64.deb 
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v1.7.2 git://github.com/ikwzm/fclkcfg-kmod-dpkg
shell$ cd fclkcfg-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2020.1-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=5.10.120-zynqmp-fpga-generic kernel_src_dir=../../linux-5.10.120-zynqmp-fpga-generic binary
    :
    :
    :
shell$ file ../fclkcfg-5.10.120-zynqmp-fpga-generic_1.7.2-1_arm64.deb 
../fclkcfg-5.10.120-zynqmp-fpga-generic_1.7.2-1_arm64.deb: Debian binary package (format 2.0)
```


