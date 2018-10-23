
#### Setup APT

distro=stretch
export LANG=C

/debootstrap/debootstrap --second-stage

cat <<EOT > /etc/apt/sources.list
deb     http://ftp.jp.debian.org/debian            stretch         main contrib non-free
deb-src http://ftp.jp.debian.org/debian            stretch         main contrib non-free
deb     http://ftp.jp.debian.org/debian            stretch-updates main contrib non-free
deb-src http://ftp.jp.debian.org/debian            stretch-updates main contrib non-free
deb     http://security.debian.org/debian-security stretch/updates main contrib non-free
deb-src http://security.debian.org/debian-security stretch/updates main contrib non-free
EOT

cat <<EOT > /etc/apt/apt.conf.d/71-no-recommends
APT::Install-Recommends "0";
APT::Install-Suggests   "0";
EOT

apt-get update  -y

#### Install Core applications

apt-get install -y locales dialog
dpkg-reconfigure locales
apt-get install -y net-tools openssh-server ntpdate resolvconf sudo less hwinfo ntp tcsh zsh

#### Setup hostname

echo "debian-fpga" > /etc/hostname

#### Set root password

echo Set root password
passwd

cat <<EOT >> /etc/securetty
# Seral Port for Xilinx Zynq
ttyPS0
EOT

#### Add fpga user

echo Add fpga user
adduser fpga
echo "fpga ALL=(ALL:ALL) ALL" > /etc/sudoers.d/fpga

#### Setup sshd config

sed -i -e 's/#PasswordAuthentication/PasswordAuthentication/g' /etc/ssh/sshd_config

#### Setup Time Zone

dpkg-reconfigure tzdata

#### Setup fstab

cat <<EOT > /etc/fstab
none		/config		configfs	defaults	0	0
EOT

#### Setup Network Interface

cat <<EOT > /etc/network/interfaces.d/eth0
allow-hotplug eth0
iface eth0 inet dhcp
EOT

#### Setup /lib/firmware

mkdir /lib/firmware
mkdir /lib/firmware/ti-connectivity

#### Install Development applications

apt-get install -y build-essential
apt-get install -y git
apt-get install -y u-boot-tools
apt-get install -y socat
apt-get install -y ruby ruby-msgpack ruby-serialport
gem install rake

apt-get install -y python  python-dev  python-setuptools  python-wheel  python-pip  
apt-get install -y python3 python3-dev python3-setuptools python3-wheel python3-pip
apt-get install -y python-numpy python3-numpy
pip3 install msgpack-rpc-python

#### Install Device Tree Compiler (supported symbol version)

apt-get install -y flex bison
cd root
mkdir src
cd src
git clone -b v1.4.7 https://git.kernel.org/pub/scm/utils/dtc/dtc.git dtc
cd dtc
make
make HOME=/usr/local install-bin
cd /

#### Install Wireless tools and firmware

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

#### Install Other applications

apt-get install -y avahi-daemon
apt-get install -y samba

#### Install Linux Modules

dpkg -i linux-image-4.14.0-xlnx-v2018.2-zynqmp-fpga_4.14.0-xlnx-v2018.2-zynqmp-fpga-1_arm64.deb

#### Clean Cache

apt-get clean

##### Create Debian Package List

dpkg -l > dpkg-list.txt
