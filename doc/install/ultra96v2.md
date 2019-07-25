## Ultra96-V2

### Downlowd from github

```console
shell$ git clone git://github.com/ikwzm/ZynqMP-FPGA-Linux
shell$ cd ZynqMP-FPGA-Linux
shell$ git checkout v2019.1.0
shell$ git lfs pull
```

### File Description

 * target/Ultra96-V2
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - image-4.19.0-xlnx-v2019.1-zynqmp-fpga                       : Linux Kernel Image       (use Git LFS)
     - devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-ultra96v2.dtb    : Linux Device Tree Blob   
     - devicetree-4.19.0-xlnx-v2019.1-zynqmp-fpga-ultra96v2.dts    : Linux Device Tree Source
 * debian10-rootfs-vanilla.tgz                                     : Debian10 Root File System (use Git LFS)
 * linux-image-4.19.0-xlnx-v2019.1-zynqmp-fpga_4.19.0-xlnx-v2019.1-zynqmp-fpga-2_arm64.deb   : Linux Image Package      (use Git LFS)
 * linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga_4.19.0-xlnx-v2019.1-zynqmp-fpga-2_arm64.deb : Linux Headers Package    (use Git LFS)
 * fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.1.0-1_arm64.deb       : fclkcfg Device Driver and Services Package
 * udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.4.2-0_arm64.deb       : udmabuf Device Driver and Services Package
 
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
shell# cp linux-image-4.19.0-xlnx-v2019.1-zynqmp-fpga_4.19.0-xlnx-v2019.1-zynqmp-fpga-2_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga_4.19.0-xlnx-v2019.1-zynqmp-fpga-2_arm64.deb /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.1.0-1_arm64.deb       /mnt/usb2/home/fpga/debian
shell# cp udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.4.2-0_arm64.deb       /mnt/usb2/home/fpga/debian
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

#### Install Linux Headers Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga_4.19.0-xlnx-v2019.1-zynqmp-fpga-2_arm64.deb
Selecting previously unselected package linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga.
(Reading database ... 26341 files and directories currently installed.)
Preparing to unpack linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga_4.19.0-xlnx-v2019.1-zynqmp-fpga-2_arm64.deb ...
Unpacking linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga (4.19.0-xlnx-v2019.1-zynqmp-fpga-2) ...
Setting up linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga (4.19.0-xlnx-v2019.1-zynqmp-fpga-2) ...
make: Entering directory '/usr/src/linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga'
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/kconfig/conf.o
  YACC    scripts/kconfig/zconf.tab.c
  LEX     scripts/kconfig/zconf.lex.c
  HOSTCC  scripts/kconfig/zconf.tab.o
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
  CC      scripts/mod/empty.o
  HOSTCC  scripts/mod/mk_elfconfig
  MKELF   scripts/mod/elfconfig.h
  HOSTCC  scripts/mod/modpost.o
  CC      scripts/mod/devicetable-offsets.s
  HOSTCC  scripts/mod/file2alias.o
  HOSTCC  scripts/mod/sumversion.o
  HOSTLD  scripts/mod/modpost
  HOSTCC  scripts/bin2c
  HOSTCC  scripts/kallsyms
  HOSTCC  scripts/conmakehash
  HOSTCC  scripts/sortextable
  HOSTCC  scripts/asn1_compiler
  HOSTCC  scripts/extract-cert
make: Leaving directory '/usr/src/linux-headers-4.19.0-xlnx-v2019.1-zynqmp-fpga'
```

#### Install fclkcfg Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.1.0-1_arm64.deb
Selecting previously unselected package fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga.
(Reading database ... 45308 files and directories currently installed.)
Preparing to unpack fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.1.0-1_arm64.deb ...
Unpacking fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga (1.1.0-1) ...
Setting up fclkcfg-4.19.0-xlnx-v2019.1-zynqmp-fpga (1.1.0-1) ...
```

#### Install udmabuf Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:/home/fpga/debian# dpkg -i udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.4.2-0_arm64.deb
Selecting previously unselected package udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga.
(Reading database ... 45308 files and directories currently installed.)
Preparing to unpack udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga_1.4.2-0_arm64.deb ...
Unpacking udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga (1.4.2-0) ...
Setting up udmabuf-4.19.0-xlnx-v2019.1-zynqmp-fpga (1.4.2-0) ...
```

