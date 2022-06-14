Build Debian11 RootFS
====================================================================================

## Setup parameters 

```console
shell$ apt-get install qemu-user-static debootstrap binfmt-support
shell$ export targetdir=debian11-rootfs
shell$ export distro=bullseye
```

## Build Debian RootFS first-step in $targetdir(=debian11-rootfs)

```console
shell$ mkdir                                               $PWD/$targetdir
shell$ sudo chown root                                     $PWD/$targetdir
shell$ sudo debootstrap --arch=arm64 --foreign $distro     $PWD/$targetdir
shell$ sudo cp /usr/bin/qemu-aarch64-static                $PWD/$targetdir/usr/bin
shell$ sudo cp /etc/resolv.conf                            $PWD/$targetdir/etc
shell$ sudo cp scripts/build-debian11-rootfs-with-qemu.sh  $PWD/$targetdir
shell$ sudo cp linux-image-5.10.120-zynqmp-fpga-generic_5.10.120-zynqmp-fpga-generic-0_arm64.deb     $PWD/$targetdir
````

## Build Debian RootFS second-step with QEMU

### Change Root to debian11-rootfs

```console
shell$ sudo chroot $PWD/$targetdir
```

There are two ways

1. run build-debian11-rootfs-with-qemu.sh (easy)
2. run this chapter step-by-step (annoying)

### Run debootstrap second stage

```console
debian11-rootfs# distro=bullseye
debian11-rootfs# export LANG=C
debian11-rootfs# /debootstrap/debootstrap --second-stage
```

#### NOTE

If the version of qemu-user-static is old, you may get the following error.

```console
debian11-rootfs# /debootstrap/debootstrap --second-stage
W: Failure trying to run:  /sbin/ldconfig
W: See //debootstrap/debootstrap.log for details
```

In this case, please update qemu-user-static.
I used 'qemu-user-static_6.1+dfsg-8_amd64.deb'.

### Setup APT

```console
debian11-rootfs# cat <<EOT > /etc/apt/sources.list
deb     http://ftp.jp.debian.org/debian  bullseye           main contrib non-free
deb-src http://ftp.jp.debian.org/debian  bullseye           main contrib non-free
deb     http://ftp.jp.debian.org/debian  bullseye-updates   main contrib non-free
deb-src http://ftp.jp.debian.org/debian  bullseye-updates   main contrib non-free
deb     http://security.debian.org       bullseye-security  main contrib non-free
deb-src http://security.debian.org       bullseye-security  main contrib non-free
EOT
```

```console
debian11-rootfs# cat <<EOT > /etc/apt/apt.conf.d/71-no-recommends
APT::Install-Recommends "0";
APT::Install-Suggests   "0";
EOT
```

```console
debian11-rootfs# apt-get update -y
debian11-rootfs# apt-get upgrade -y
```

### Install Locales

```console
debian11-rootfs# apt-get install -y locales dialog
debian11-rootfs# dpkg-reconfigure locales
```

or if noninteractive set to LANG=C.UTF-8

```console
debian11-rootfs# apt-get install -y locales dialog
debian11-rootfs# sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen
debian11-rootfs# echo 'LANG="C.UTF-8"' > /etc/default/locale
debian11-rootfs# dpkg-reconfigure --frontend=noninteractive locales
debian11-rootfs# update-locale LANG=C.UTF-8
```

### Install applications

```console
debian11-rootfs# apt-get install -y net-tools openssh-server ntpdate resolvconf sudo less hwinfo ntp tcsh zsh file
```

### Setup hostname

```console
debian11-rootfs# echo debian-fpga > /etc/hostname
```

### Setup root password

```console
debian11-rootfs# passwd
```

This time, we set the "admin" at the root' password.

To be able to login as root from ZynqMP serial port.

```console
debian11-rootfs# cat <<EOT >> /etc/securetty
# Serial Port for Xilinx ZynqMP
ttyPS0
ttyPS1
EOT
```

### Add a new guest user

```console
debian11-rootfs# adduser fpga
```

This time, we set the "fpga" at the fpga'password.

```console
debian11-rootfs# echo "fpga ALL=(ALL:ALL) ALL" > /etc/sudoers.d/fpga
```

### Setup sshd config

```console
debian11-rootfs# sed -i -e 's/#PasswordAuthentication/PasswordAuthentication/g' /etc/ssh/sshd_config
```

### Setup Time Zone

```console
debian11-rootfs# dpkg-reconfigure tzdata
```

or if noninteractive set to Asia/Tokyo

```console
debian11-rootfs# echo "Asia/Tokyo" > /etc/timezone
debian11-rootfs# dpkg-reconfigure -f noninteractive tzdata
```


### Setup fstab

```console
debian11-rootfs# cat <<EOT > /etc/fstab
none		/config		configfs	defaults	0	0
EOT
````

### Setup Network

```console
debian11-rootfs# apt-get install -y ifupdown
debian11-rootfs# cat <<EOT > /etc/network/interfaces.d/eth0
allow-hotplug eth0
iface eth0 inet dhcp
EOT
````

### Setup /lib/firmware

```console
debian11-rootfs# mkdir /lib/firmware
debian11-rootfs# mkdir /lib/firmware/ti-connectivity
debian11-rootfs# mkdir /lib/firmware/mchp
```

### Install Development applications

```console
debian11-rootfs# apt-get install -y build-essential
debian11-rootfs# apt-get install -y git git-lfs
debian11-rootfs# apt-get install -y u-boot-tools device-tree-compiler
debian11-rootfs# apt-get install -y libssl-dev
debian11-rootfs# apt-get install -y socat
debian11-rootfs# apt-get install -y ruby rake ruby-msgpack ruby-serialport
debian11-rootfs# apt-get install -y python3 python3-dev python3-setuptools python3-wheel python3-pip
debian11-rootfs# apt-get purge   -y python3-pkg-resources
debian11-rootfs# apt-get install -y python3-pkg-resources
debian11-rootfs# apt-get install -y python3-numpy
debian11-rootfs# pip3 install msgpack-rpc-python
debian11-rootfs# apt-get install -y flex bison pkg-config
```

### Install Wireless tools and firmware

```console
debian11-rootfs# apt-get install -y wireless-tools
debian11-rootfs# apt-get install -y wpasupplicant
debian11-rootfs# apt-get install -y firmware-realtek
debian11-rootfs# apt-get install -y firmware-ralink
```

```console
debian11-rootfs# git clone git://git.ti.com/wilink8-wlan/wl18xx_fw.git
debian11-rootfs# cp wl18xx_fw/wl18xx-fw-4.bin /lib/firmware/ti-connectivity
debian11-rootfs# rm -rf wl18xx_fw/
```

```console
debian11-rootfs# git clone git://git.ti.com/wilink8-bt/ti-bt-firmware
debian11-rootfs# cp ti-bt-firmware/TIInit_11.8.32.bts /lib/firmware/ti-connectivity
debian11-rootfs# rm -rf ti-bt-firmware
```

```console
debian11-rootfs# git clone git://github.com/linux4wilc/firmware  linux4wilc-firmware  
debian11-rootfs# cp linux4wilc-firmware/*.bin /lib/firmware/mchp
debian11-rootfs# rm -rf linux4wilc-firmware  
```

### Install Other applications

```console
debian11-rootfs# apt-get install -y samba
debian11-rootfs# apt-get install -y avahi-daemon
```

### Install haveged for Linux Kernel 4.19

```console
debian11-rootfs# apt-get install -y haveged
```

### Install Linux Modules

```console
debian11-rootfs# dpkg -i linux-image-5.10.120-zynqmp-fpga-generic_5.10.120-zynqmp-fpga-generic-0_arm64.deb
```

### Clean Cache

```console
debian11-rootfs# apt-get clean
```

### Create Debian Package List

```console
debian11-rootfs# dpkg -l > dpkg-list.txt
```

### Finish

```console
debian10-rootfs# exit
shell$ sudo rm -f $PWD/$targetdir/usr/bin/qemu-aarch64-static
shell$ sudo rm -f $PWD/$targetdir/build-debian11-rootfs-with-qemu.sh
shell$ sudo rm -f $PWD/$targetdir/linux-image-5.10.120-zynqmp-fpga-generic_5.10.120-zynqmp-fpga-generic-0_arm64.deb
shell$ sudo mv    $PWD/$targetdir/dpkg-list.txt files/debian11-dpkg-list.txt
```

## Build debian11-rootfs-vanilla.tgz

```console
shell$ cd $PWD/$targetdir
shell$ sudo tar cfz ../debian11-rootfs-vanilla.tgz *
```

## Build debian11-rootfs-vanilla.tgz.files

```console
shell$ mkdir debian11-rootfs-vanilla.tgz.files
shell$ cd    debian11-rootfs-vanilla.tgz.files
shell$ split -d --bytes=40M ../debian11-rootfs-vanilla.tgz
shell$ cd ..
```
