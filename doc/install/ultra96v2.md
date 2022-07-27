## Ultra96-V2

### Downlowd from github

**Note: Downloading the entire repository takes time, so download the source code from https://github.com/ikwzm/ZynqMP-FPGA-Linux/releases.**

```console
shell$ wget https://github.com/ikwzm/ZynqMP-FPGA-Linux/archive/refs/tags/v2021.1.2.tar.gz
shell$ tar xfz v2021.1.2.tar.gz
shell$ cd ZynqMP-FPGA-Linux-2021.1.2
```

### File Description

 * target/Ultra96-V2
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dtb     : Linux Device Tree Blob   
     - devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dts     : Linux Device Tree Blob   
     - devicetree-5.10.0-xlnx-v2021.1-zynqmp-fpga-ultra96v2.dtb    : Linux Device Tree Blob   
     - devicetree-5.10.0-xlnx-v2021.1-zynqmp-fpga-ultra96v2.dts    : Linux Device Tree Source
 * image-5.4.0-xlnx-v2020.2-zynqmp-fpga                            : Linux Kernel Image
 * image-5.10.0-xlnx-v2021.1-zynqmp-fpga                           : Linux Kernel Image
 * debian11-rootfs-vanilla.tgz.files/                              : Debian11 Root File System
   + x00 .. x08                                                    : (splited files)
 * linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb     : Linux Image Package
 * linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb   : Linux Headers Package
 * fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb        : fclkcfg(1.7.2) Device Driver and Services Package
 * u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.2.4-0_arm64.deb      : u-dma-buf(3.2.4) Device Driver and Services Package
 * linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb   : Linux Image Package
 * linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb : Linux Headers Package
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
shell# cp target/Ultra96-V2/boot/*                                        /mnt/usb1
shell# cp image-5.4.0-xlnx-v2020.2-zynqmp-fpga                            /mnt/usb1
shell# cp image-5.10.0-xlnx-v2021.1-zynqmp-fpga                           /mnt/usb1
```

#### Make RootFS Partition

```console
shell# cat debian11-rootfs-vanilla.tgz.files/* | tar xfz - -C             /mnt/usb2
shell# mkdir                                                              /mnt/usb2/home/fpga/debian
shell# cp linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb     /mnt/usb2/home/fpga/debian
shell# cp linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-3_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb        /mnt/usb2/home/fpga/debian
shell# cp u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.2.4-0_arm64.deb      /mnt/usb2/home/fpga/debian
shell# cp linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga_1.7.2-1_arm64.deb       /mnt/usb2/home/fpga/debian
shell# cp u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb     /mnt/usb2/home/fpga/debian
```

#### Add boot partition mount position to /etc/fstab

```console
shell# mkdir /mnt/usb2/mnt/boot
shell# cat <<EOT >> /mnt/usb2/etc/fstab
/dev/mmcblk0p1	/mnt/boot	auto	defaults	0	0
EOT
```

#### Setup WiFi

  * ssid: ssssssss
  * passphrase: ppppppppp
  * psk: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

```console
shell# wpa_passphrase ssssssss ppppppppp
network={
	ssid="ssssssss"
	#psk="ppppppppp"
	psk=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
}
```

```console
shell# cat <<EOT > /mnt/usb2/etc/network/interfaces.d/wlan0

auto  wlan0
iface wlan0 inet dhcp
        wpa-ssid ssssssss
	wpa-psk  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
EOT
```

#### Unmount SD-Card

```console
shell# umount /mnt/usb1
shell# umount /mnt/usb2
```

### Install Debian Packages

#### Boot Ultra96 and login fpga or root user

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

Since linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb is already pre-installed in debian11-rootfs.tgz, this The process can be omitted.

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb
Selecting previously unselected package linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 117342 files and directories currently installed.)
Preparing to unpack linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb ...
Unpacking linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-4) ...
Setting up linux-image-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-4) ...
```

#### Install Linux Headers Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i linux-headers-5.10.0-xlnx-v2021.1-zy
nqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb
Selecting previously unselected package linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 117624 files and directories currently installed.)
Preparing to unpack linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga_5.10.0-xlnx-v2021.1-zynqmp-fpga-4_arm64.deb ...
Unpacking linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-4) ...
Setting up linux-headers-5.10.0-xlnx-v2021.1-zynqmp-fpga (5.10.0-xlnx-v2021.1-zynqmp-fpga-4) ...
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
(Reading database ... 134025 files and directories currently installed.)
Preparing to unpack fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga_1.7.2-1_arm64.deb ...
Unpacking fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga (1.7.2-1) ...
Setting up fclkcfg-5.10.0-xlnx-v2021.1-zynqmp-fpga (1.7.2-1) ...
```

#### Install u-dma-buf Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb
Selecting previously unselected package u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga.
(Reading database ... 134031 files and directories currently installed.)
Preparing to unpack u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga_3.2.4-0_arm64.deb ...
Unpacking u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga (3.2.4-0) ...
Setting up u-dma-buf-5.10.0-xlnx-v2021.1-zynqmp-fpga (3.2.4-0) ...
```

