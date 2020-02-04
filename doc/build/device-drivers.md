Build Device Drivers and Services Package
====================================================================================

udmabuf-4.19.0-xlnx-v2019.2-zynqmp-fpga_1.4.6-0_arm64.deb
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v1.4.6 git://github.com/ikwzm/udmabuf-kmod-dpkg
shell$ cd udmabuf-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2019.2-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=4.19.0-xlnx-v2019.2-zynqmp-fpga kernel_src_dir=../../linux-xlnx-v2019.2-zynqmp-fpga binary
    :
    :
    :
shell$ file ../udmabuf-4.19.0-xlnx-v2019.2-zynqmp-fpga_1.4.6-0_arm64.deb
../udmabuf-4.19.0-xlnx-v2019.2-zynqmp-fpga_1.4.6-0_arm64.deb: Debian binary package (format 2.0)
```

u-dma-buf-4.19.0-xlnx-v2019.2-zynqmp-fpga_2.1.3-0_arm64.deb
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v2.1.3 git://github.com/ikwzm/u-dma-buf-kmod-dpkg
shell$ cd u-dma-buf-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2019.2-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=4.19.0-xlnx-v2019.2-zynqmp-fpga kernel_src_dir=../../linux-xlnx-v2019.2-zynqmp-fpga binary
    :
    :
    :
shell$ file ../u-dma-buf-4.19.0-xlnx-v2019.2-zynqmp-fpga_2.1.3-0_arm64.deb
../u-dma-buf-4.19.0-xlnx-v2019.2-zynqmp-fpga_2.1.3-0_arm64.deb: Debian binary package (format 2.0)
```

fclkcfg-4.19.0-xlnx-v2019.2-zynqmp-fpga_1.3.0-1_arm64.deb
------------------------------------------------------------------------------------

### Download repository

```console
shell$ git clone --recursive --depth=1 -b v1.3.0 git://github.com/ikwzm/fclkcfg-kmod-dpkg
shell$ cd fclkcfg-kmod-dpkg
```

### Cross Compile for linux-xlnx-v2019.2-zynqmp-fpga

```console
shell$ sudo debian/rules arch=arm64 deb_arch=arm64 kernel_release=4.19.0-xlnx-v2019.2-zynqmp-fpga kernel_src_dir=../../linux-xlnx-v2019.2-zynqmp-fpga binary
    :
    :
    :
shell$ file ../fclkcfg-4.19.0-xlnx-v2019.2-zynqmp-fpga_1.3.0-1_arm64.deb
../fclkcfg-4.19.0-xlnx-v2019.2-zynqmp-fpga_1.3.0-1_arm64.deb: Debian binary package (format 2.0)
```


