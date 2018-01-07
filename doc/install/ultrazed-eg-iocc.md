## UltraZed EG IOCC

### Downlowd from github

```
shell$ git clone git://github.com/ikwzm/ZynqMP-FPGA-Linux
shell$ cd ZynqMP-FPGA-Linux
shell$ git checkout v0.1.3
shell$ git lfs pull
```

### File Description

 * tareget/UltraZed-EG-IOCC/
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - image-4.9.0-xlnx-v2017.3-fpga                               : Linux Kernel Image       (use Git LFS)
     - devicetree-4.9.0-xlnx-v2017.3-fpga-zynqmp-uz3eg-iocc.dtb    : Linux Device Tree Blob   
     - devicetree-4.9.0-xlnx-v2017.3-fpga-zynqmp-uz3eg-iocc.dts    : Linux Device Tree Source
 * debian9-rootfs-vanilla.tgz                                      : Debian9 Root File System (use Git LFS)
 * linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb   : Linux Image Package      (use Git LFS)
 * linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb : Linux Headers Package    (use Git LFS)
 * fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb        : fclkcfg Device Driver and Services Package
 * udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb        : udmabuf Device Driver and Services Package
 
### Format SD-Card

````
shell# fdisk /dev/sdc
   :
   :
   :
shell# mkfs-vfat /dev/sdc1
shell# mkfs.ext3 /dev/sdc2
````

### Write to SD-Card

````
shell# mount /dev/sdc1 /mnt/usb1
shell# mount /dev/sdc2 /mnt/usb2
shell# cp target/UltraZed-EG-IOCC/boot/*                                  /mnt/usb1
shell# tar xfz debian9-rootfs-vanilla.tgz -C                              /mnt/usb2
shell# mkdir                                                              /mnt/usb2/home/fpga/debian
shell# cp linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb /mnt/usb2/home/fpga/debian
shell# cp fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb        /mnt/usb2/home/fpga/debian
shell# cp udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb        /mnt/usb2/home/fpga/debian
shell# umount mnt/usb1
shell# umount mnt/usb2
````

### Install Debian Packages

#### Boot UltraZed-EG-IOCC and login fpga or root user

fpga'password is "fpga".

```
debian-fpga login: fpga
Password:
fpga@debian-fpga:~$
```

root'password is "admin".

```
debian-fpga login: root
Password:
root@debian-fpga:~#
```

#### Install Linux Image Package

```
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# sudo dpkg -i linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb
(Reading database ... 24872 files and directories currently installed.)
Preparing to unpack linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb ...
Unpacking linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga (4.9.0-xlnx-v2017.3-zynqmp-fpga-1) over (4.9.0-xlnx-v2017.3-zynqmp-fpga-1) ...
Setting up linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga (4.9.0-xlnx-v2017.3-zynqmp-fpga-1) ...
```

#### Install Linux Headers Package

```
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# sudo dpkg -i linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb
Selecting previously unselected package linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga.
(Reading database ... 24872 files and directories currently installed.)
Preparing to unpack linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb ...
Unpacking linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga (4.9.0-xlnx-v2017.3-zynqmp-fpga-1) ...
Setting up linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga (4.9.0-xlnx-v2017.3-zynqmp-fpga-1) ...
```

#### Install fclkcfg Device Driver and Services Package

```
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# sudo dpkg -i fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb
Selecting previously unselected package fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga.
(Reading database ... 43458 files and directories currently installed.)
Preparing to unpack fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb ...
Unpacking fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga (0.0.1-1) ...
Setting up fclkcfg-4.9.0-xlnx-v2017.3-zynqmp-fpga (0.0.1-1) ...
Created symlink /etc/systemd/system/multi-user.target.wants/fpga-clock.service  → /etc/systemd/system/fpga-clock.service.
[  345.865653] systemd[1]: apt-daily.timer: Adding 1h 45min 592.785ms random time.
[  345.874409] systemd[1]: apt-daily-upgrade.timer: Adding 28min 35.939743s random time.
```

#### Install udmabuf Device Driver and Services Package

```
root@debian-fpga:~# cd /home/fpga/debian
root@debian-fpga:~# dpkg -i udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb
Selecting previously unselected package udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga.
(Reading database ... 43465 files and directories currently installed.)
Preparing to unpack udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga_0.0.1-1_arm64.deb ...
Unpacking udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga (0.0.1-1) ...
Setting up udmabuf-4.9.0-xlnx-v2017.3-zynqmp-fpga (0.0.1-1) ...
Created symlink /etc/systemd/system/multi-user.target.wants/udmabuf.service → /etc/systemd/system/udmabuf.service.
[   77.496889] systemd[1]: apt-daily.timer: Adding 8h 53min 37.349217s random time.
```

