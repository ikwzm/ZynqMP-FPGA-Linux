
### Run debootstrap second stage

distro=bullseye
export LANG=C
/debootstrap/debootstrap --second-stage

### Setup APT

cat <<EOT > /etc/apt/sources.list
deb     http://ftp.jp.debian.org/debian  bullseye           main contrib non-free
deb-src http://ftp.jp.debian.org/debian  bullseye           main contrib non-free
deb     http://ftp.jp.debian.org/debian  bullseye-updates   main contrib non-free
deb-src http://ftp.jp.debian.org/debian  bullseye-updates   main contrib non-free
deb     http://security.debian.org       bullseye-security  main contrib non-free
deb-src http://security.debian.org       bullseye-security  main contrib non-free
EOT


cat <<EOT > /etc/apt/apt.conf.d/71-no-recommends
APT::Install-Recommends "0";
APT::Install-Suggests   "0";
EOT

apt-get update -y
apt-get upgrade -y

### Install Locales

apt-get install -y locales dialog
sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen
echo 'LANG="C.UTF-8"' > /etc/default/locale
dpkg-reconfigure --frontend=noninteractive locales
update-locale LANG=C.UTF-8

### Install Core applications

apt-get install -y net-tools openssh-server ntpdate resolvconf sudo less hwinfo ntp tcsh zsh file

### Setup hostname

echo "debian-fpga" > /etc/hostname

### Set root password

echo Set root password. please input "admin"
passwd

cat <<EOT >> /etc/securetty
# Serial Port for Xilinx ZynqMP
ttyPS0
ttyPS1
EOT

### Add fpga user

echo Add fpga user. please input "fpga"
adduser fpga
echo "fpga ALL=(ALL:ALL) ALL" > /etc/sudoers.d/fpga

### Setup sshd config

sed -i -e 's/#PasswordAuthentication/PasswordAuthentication/g' /etc/ssh/sshd_config

### Setup Time Zone

dpkg-reconfigure tzdata

### Setup fstab

cat <<EOT > /etc/fstab
none		/config		configfs	defaults	0	0
EOT

### Setup Network

apt-get install -y ifupdown
cat <<EOT > /etc/network/interfaces.d/eth0
allow-hotplug eth0
iface eth0 inet dhcp
EOT

### Setup /lib/firmware

mkdir /lib/firmware
mkdir /lib/firmware/ti-connectivity
mkdir /lib/firmware/mchp

### Install Development applications

apt-get install -y build-essential
apt-get install -y git git-lfs
apt-get install -y u-boot-tools device-tree-compiler
apt-get install -y libssl-dev
apt-get install -y socat
apt-get install -y ruby rake ruby-msgpack ruby-serialport
apt-get install -y python3 python3-dev python3-setuptools python3-wheel python3-pip python3-numpy
pip3 install msgpack-rpc-python
apt-get install -y flex bison pkg-config

### Install Wireless tools and firmware

apt-get install -y wireless-tools
apt-get install -y wpasupplicant
apt-get install -y firmware-realtek
apt-get install -y firmware-ralink

git clone git://git.ti.com/wilink8-wlan/wl18xx_fw.git
cp wl18xx_fw/wl18xx-fw-4.bin /lib/firmware/ti-connectivity
rm -rf wl18xx_fw/

git clone git://git.ti.com/wilink8-bt/ti-bt-firmware
cp ti-bt-firmware/TIInit_11.8.32.bts /lib/firmware/ti-connectivity
rm -rf ti-bt-firmware

git clone https://github.com/linux4wilc/firmware  linux4wilc-firmware  
cp linux4wilc-firmware/*.bin /lib/firmware/mchp
rm -rf linux4wilc-firmware  

### Install Other applications

apt-get install -y samba
apt-get install -y avahi-daemon

### Install haveged for Linux Kernel 4.19

apt-get install -y haveged

### Install Linux Modules

dpkg -i linux-image-5.10.120-zynqmp-fpga-generic_5.10.120-zynqmp-fpga-generic-0_arm64.deb

### Clean Cache

apt-get clean

### Create Debian Package List

dpkg -l > dpkg-list.txt
