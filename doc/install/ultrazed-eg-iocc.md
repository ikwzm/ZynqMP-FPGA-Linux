## UltraZed EG IOCC

### Downlowd from github

**Note: Downloading the entire repository is time consuming, so download only the branch you need.**

```console
shell$ git clone --depth=1 --branch v2021.1.1-rc2 git://github.com/ikwzm/ZynqMP-FPGA-Linux
shell$ cd ZynqMP-FPGA-Linux
shell$ git lfs pull
```

### File Description

 * target/UltraZed-EG-IOCC/
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - image-5.4.0-xlnx-v2020.2-zynqmp-fpga                        : Linux Kernel Image       (use Git LFS)
     - devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-uz3eg-iocc.dtb    : Linux Device Tree Blob   
     - devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-uz3eg-iocc.dts    : Linux Device Tree Blob   
     - image-5.10.0-xlnx-v2021.1-zynqmp-fpga                       : Linux Kernel Image       (use Git LFS)
     - devicetree-5.10.0-xlnx-v2021.1-zynqmp-fpga-uz3eg-iocc.dtb   : Linux Device Tree Blob   
     - devicetree-5.10.0-xlnx-v2021.1-zynqmp-fpga-uz3eg-iocc.dts   : Linux Device Tree Source
 * debian11-rootfs-vanilla.tgz                                     : Debian11 Root File System (use Git LFS)
 * linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb     : Linux Image Package      (use Git LFS)
 * linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb   : Linux Headers Package    (use Git LFS)
 * fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb        : fclkcfg(1.7.2) Device Driver and Services Package
 * u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.2.4-0_arm64.deb      : u-dma-buf(3.2.4) Device Driver and Services Package
 * linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb   : Linux Image Package      (use Git LFS)
 * linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb : Linux Headers Package    (use Git LFS)
 * fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga_1.7.2-1_arm64.deb       : fclkcfg(1.7.2) Device Driver and Services Package
 * u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb     : u-dma-buf(3.2.4) Device Driver and Services Package
 
### Format SD-Card

[./doc/install/format-disk-zynq.md](format-disk-zynq.md)

### Write to SD-Card

#### Mount SD-Card

```console
shell# mount /dev/sdc1 /mnt/usb1
shell# mount /dev/sdc2 /mnt/usb2
```
#### Make Boot Partition

```console
shell# cp target/UltraZed-EG-IOCC/boot/*                                  /mnt/usb1
```

#### Make RootFS Partition

```console
shell# tar xfz debian11-rootfs-vanilla.tgz -C                             /mnt/usb2
shell# mkdir                                                              /mnt/usb2/home/fpga/debian
shell# cp linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb     /mnt/usb2/home/fpga/debian
shell# cp linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb        /mnt/usb2/home/fpga/debian
shell# cp linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga_1.7.2-1_arm64.deb       /mnt/usb2/home/fpga/debian
shell# cp u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb     /mnt/usb2/home/fpga/debian
```

#### Add boot partition mount position to /etc/fstab

```console
shell# mkdir /mnt/usb2/mnt/boot
shell# cat <<EOT >> /mnt/usb2/etc/fstab
/dev/mmcblk1p1	/mnt/boot	auto	defaults	0	0
EOT
```

#### Unmount SD-Card

```console
shell# umount /mnt/usb1
shell# umount /mnt/usb2
```

### Install Debian Packages

#### Boot UltraZed-EG-IOCC and login fpga or root user

fpga'password is "fpga".

```console
debian-fpga login: fpga
Password:
fpga@debian-fpga:~$
```

root'password is "admin".

```console
debian-fpga login: root
Password:
root@debian-fpga:~#
```

#### Install Linux Image Package

Since linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb is already pre-installed in debian11-rootfs.tgz, this The process can be omitted.

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb
Selecting previously unselected package linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 117341 files and directories currently installed.)
Preparing to unpack linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb ...
Unpacking linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-2) ...
Setting up linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-2) ...
```

#### Install Linux Headers Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# pkg -i linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb
Selecting previously unselected package linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 117613 files and directories currently installed.)
Preparing to unpack linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-2_arm64.deb ...
Unpacking linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-2) ...
Setting up linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-2) ...
make: Entering directory '/usr/src/linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga'
  SYNC    include/config/auto.conf.cmd
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/kconfig/conf.o
  HOSTCC  scripts/kconfig/confdata.o
  HOSTCC  scripts/kconfig/expr.o
  LEX     scripts/kconfig/lexer.lex.c
  YACC    scripts/kconfig/parser.tab.[ch]
  HOSTCC  scripts/kconfig/lexer.lex.o
  HOSTCC  scripts/kconfig/parser.tab.o
  HOSTCC  scripts/kconfig/preprocess.o
  HOSTCC  scripts/kconfig/symbol.o
  HOSTCC  scripts/kconfig/util.o
  HOSTLD  scripts/kconfig/conf
*
* Restart config...
*
*
* ARMv8.5 architectural features
*
Branch Target Identification support (ARM64_BTI) [Y/n/?] y
Enable support for E0PD (ARM64_E0PD) [Y/n/?] y
Enable support for random number generation (ARCH_RANDOM) [Y/n/?] y
Memory Tagging Extension support (ARM64_MTE) [Y/n/?] (NEW)
*
* KASAN: runtime memory debugger
*
KASAN: runtime memory debugger (KASAN) [N/y/?] (NEW)
  HOSTCC  scripts/dtc/dtc.o
  HOSTCC  scripts/dtc/flattree.o
  HOSTCC  scripts/dtc/fstree.o
  HOSTCC  scripts/dtc/data.o
  HOSTCC  scripts/dtc/livetree.o
  HOSTCC  scripts/dtc/treesource.o
  HOSTCC  scripts/dtc/srcpos.o
  HOSTCC  scripts/dtc/checks.o
  HOSTCC  scripts/dtc/util.o
  HOSTCC  scripts/dtc/dtc-lexer.lex.o
  HOSTCC  scripts/dtc/dtc-parser.tab.o
  HOSTLD  scripts/dtc/dtc
  HOSTCC  scripts/kallsyms
  HOSTCC  scripts/sorttable
  HOSTCC  scripts/asn1_compiler
  HOSTCC  scripts/extract-cert
  CC      scripts/mod/empty.o
  HOSTCC  scripts/mod/mk_elfconfig
  MKELF   scripts/mod/elfconfig.h
  CC      scripts/mod/devicetable-offsets.s
  HOSTCC  scripts/mod/modpost.o
  HOSTCC  scripts/mod/file2alias.o
  HOSTCC  scripts/mod/sumversion.o
  HOSTLD  scripts/mod/modpost
scripts/Makefile.build:414: warning: overriding recipe for target 'modules.order'
Makefile:1405: warning: ignoring old recipe for target 'modules.order'
make: Leaving directory '/usr/src/linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga'
```

#### Install fclkcfg Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga_1.7.2-1_arm64.deb
Selecting previously unselected package fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 134024 files and directories currently installed.)
Preparing to unpack fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga_1.7.2-1_arm64.deb ...
Unpacking fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga (1.7.2-1) ...
Setting up fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga (1.7.2-1) ...
```

#### Install u-dma-buf Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb
Selecting previously unselected package u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 134030 files and directories currently installed.)
Preparing to unpack u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb ...
Unpacking u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga (3.2.4-0) ...
Setting up u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga (3.2.4-0) ...
```

