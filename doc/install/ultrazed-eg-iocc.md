## UltraZed EG IOCC

### Downlowd from github

```console
shell$ git clone git://github.com/ikwzm/ZynqMP-FPGA-Linux
shell$ cd ZynqMP-FPGA-Linux
shell$ git checkout v2018.2.0
shell$ git lfs pull
```

### File Description

 * tareget/UltraZed-EG-IOCC/
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - image-4.14.0-xlnx-v2018.2-zynqmp-fpga                       : Linux Kernel Image       (use Git LFS)
     - devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-uz3eg-iocc.dtb   : Linux Device Tree Blob   
     - devicetree-4.14.0-xlnx-v2018.2-zynqmp-fpga-uz3eg-iocc.dts   : Linux Device Tree Source
 * debian9-rootfs-vanilla.tgz                                      : Debian9 Root File System (use Git LFS)
 * linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb   : Linux Image Package      (use Git LFS)
 * linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb : Linux Headers Package    (use Git LFS)
 * fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb       : fclkcfg Device Driver and Services Package
 * udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb       : udmabuf Device Driver and Services Package
 
### Format SD-Card

````console
shell# fdisk /dev/sdc
   :
   :
   :
shell# mkfs-vfat /dev/sdc1
shell# mkfs.ext3 /dev/sdc2
````

### Write to SD-Card

````console
shell# mount /dev/sdc1 /mnt/usb1
shell# mount /dev/sdc2 /mnt/usb2
shell# cp target/UltraZed-EG-IOCC/boot/*                                  /mnt/usb1
shell# tar xfz debian9-rootfs-vanilla.tgz -C                              /mnt/usb2
shell# mkdir                                                              /mnt/usb2/home/fpga/debian
shell# cp linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb        /mnt/usb2/home/fpga/debian
shell# cp udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb        /mnt/usb2/home/fpga/debian
shell# umount mnt/usb1
shell# umount mnt/usb2
````

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

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# sudo dpkg -i linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb
(Reading database ... 24872 files and directories currently installed.)
Preparing to unpack linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb ...
Unpacking linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga (4.14.0-xlnx-v2018.2-zynqmp-fpga-1) over (4.14.0-xlnx-v2018.2-zynqmp-fpga-1) ...
Setting up linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga (4.14.0-xlnx-v2018.2-zynqmp-fpga-1) ...
```

#### Install Linux Headers Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# sudo dpkg -i linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb
Selecting previously unselected package linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga.
(Reading database ... 24872 files and directories currently installed.)
Preparing to unpack linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb ...
Unpacking linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga (4.14.0-xlnx-v2018.2-zynqmp-fpga-1) ...
Setting up linux-headers-4.14.0-xlnx-v2018.2-zynqmp-fpga (4.14.0-xlnx-v2018.2-zynqmp-fpga-1) ...
```

#### Install fclkcfg Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# sudo dpkg -i fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb
Selecting previously unselected package fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga.
(Reading database ... 43458 files and directories currently installed.)
Preparing to unpack fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb ...
Unpacking fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga (0.0.1-1) ...
Setting up fclkcfg-4.14.0-xlnx-v2018.2-zynqmp-fpga (0.0.1-1) ...
Created symlink /etc/systemd/system/multi-user.target.wants/fpga-clock.service  → /etc/systemd/system/fpga-clock.service.
```

#### Install udmabuf Device Driver and Services Package

```console
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# dpkg -i udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb
Selecting previously unselected package udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga.
(Reading database ... 43465 files and directories currently installed.)
Preparing to unpack udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga_0.0.1-1_arm64.deb ...
Unpacking udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga (0.0.1-1) ...
Setting up udmabuf-4.14.0-xlnx-v2018.2-zynqmp-fpga (0.0.1-1) ...
Created symlink /etc/systemd/system/multi-user.target.wants/udmabuf.service → /etc/systemd/system/udmabuf.service.
```

