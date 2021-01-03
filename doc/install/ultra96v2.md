## Ultra96-V2

### Downlowd from github

**Note: Downloading the entire repository is time consuming, so download only the branch you need.**

```console
shell$ git clone --depth=1 --branch v2020.2.1 git://github.com/ikwzm/ZynqMP-FPGA-Linux
shell$ cd ZynqMP-FPGA-Linux
shell$ git lfs pull
```

### File Description

 * target/Ultra96-V2
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - image-5.4.0-xlnx-v2020.2-zynqmp-fpga                        : Linux Kernel Image       (use Git LFS)
     - devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dtb     : Linux Device Tree Blob   
     - devicetree-5.4.0-xlnx-v2020.2-zynqmp-fpga-ultra96v2.dts     : Linux Device Tree Source
 * debian10-rootfs-vanilla.tgz                                     : Debian10 Root File System (use Git LFS)
 * linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb   : Linux Image Package      (use Git LFS)
 * linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb : Linux Headers Package    (use Git LFS)
 * fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb        : fclkcfg(1.7.2) Device Driver and Services Package
 * u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.2.4-0_arm64.deb      : u-dma-buf(3.2.4) Device Driver and Services Package
 
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
```

#### Make RootFS Partition

```console
shell# tar xfz debian10-rootfs-vanilla.tgz -C                             /mnt/usb2
shell# mkdir                                                              /mnt/usb2/home/fpga/debian
shell# cp linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.3.0-1_arm64.deb       /mnt/usb2/home/fpga/debian
shell# cp u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.0.1-0_arm64.deb     /mnt/usb2/home/fpga/debian
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

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb
Selecting previously unselected package linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga.
(Reading database ... 181094 files and directories currently installed.)
Preparing to unpack linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb ...
Unpacking linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga (5.4.0-xlnx-v2020.2-zynqmp-fpga-1) ...
Setting up linux-image-5.4.0-xlnx-v2020.2-zynqmp-fpga (5.4.0-xlnx-v2020.2-zynqmp-fpga-1) ...
```

#### Install Linux Headers Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb
Selecting previously unselected package linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga.
(Reading database ... 181094 files and directories currently installed.)
Preparing to unpack linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga_5.4.0-xlnx-v2020.2-zynqmp-fpga-1_arm64.deb ...
Unpacking linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga (5.4.0-xlnx-v2020.2-zynqmp-fpga-1) ...
Setting up linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga (5.4.0-xlnx-v2020.2-zynqmp-fpga-1) ...
make: Entering directory '/usr/src/linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga'
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
  HOSTLD  scripts/kconfig/conf
scripts/kconfig/conf  --syncconfig Kconfig
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
  HOSTCC  scripts/conmakehash
  HOSTCC  scripts/sortextable
  HOSTCC  scripts/asn1_compiler
  HOSTCC  scripts/extract-cert
  CC      scripts/mod/empty.o
  HOSTCC  scripts/mod/mk_elfconfig
  MKELF   scripts/mod/elfconfig.h
  HOSTCC  scripts/mod/modpost.o
  CC      scripts/mod/devicetable-offsets.s
  HOSTCC  scripts/mod/file2alias.o
  HOSTCC  scripts/mod/sumversion.o
  HOSTLD  scripts/mod/modpost
make: Leaving directory '/usr/src/linux-headers-5.4.0-xlnx-v2020.2-zynqmp-fpga'
```

#### Install fclkcfg Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb
Selecting previously unselected package fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga.
(Reading database ... 181893 files and directories currently installed.)
Preparing to unpack fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga_1.7.2-1_arm64.deb ...
Unpacking fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga (1.7.2-1) ...
Setting up fclkcfg-5.4.0-xlnx-v2020.2-zynqmp-fpga (1.7.2-1) ...
```

#### Install u-dma-buf Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.2.4-0_arm64.deb
Selecting previously unselected package u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga.
(Reading database ... 181899 files and directories currently installed.)
Preparing to unpack u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga_3.2.4-0_arm64.deb ...
Unpacking u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga (3.2.4-0) ...
Setting up u-dma-buf-5.4.0-xlnx-v2020.2-zynqmp-fpga (3.2.4-0) ...
```

