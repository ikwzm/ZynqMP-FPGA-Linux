## UltraZed EG IOCC

### Downlowd from github

```
shell$ git clone git://github.com/ikwzm/ZynqMP-FPGA-Linux
shell$ cd ZynqMP-FPGA-Linux
shell$ git checkout v0.1.0
shell$ git lfs pull
```

#### File Description

 * tareget/UltraZed-EG-IOCC/
   + boot/
     - boot.bin                                                    : Stage 1 Boot Loader
     - uEnv.txt                                                    : U-Boot environment variables for linux boot
     - image-4.9.0-xlnx-v2017.3-fpga                               : Linux Kernel Image       (use Git LFS)
     - devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dtb    : Linux Device Tree Blob   
     - devicetree-4.9.0-xlnx-v2017.3-zynqmp-fpga-uz3eg-iocc.dts    : Linux Device Tree Source
 * debian9-rootfs-vanilla.tgz                                      : Debian9 Root File System (use Git LFS)
 * linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb   : Linux Image Package      (use Git LFS)
 * linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb : Linux Headers Package    (use Git LFS)

#### Format SD-Card

````
shell# fdisk /dev/sdc
   :
   :
   :
shell# mkfs-vfat /dev/sdc1
shell# mkfs.ext3 /dev/sdc2
````

#### Write to SD-Card

````
shell# mount /dev/sdc1 /mnt/usb1
shell# mount /dev/sdc2 /mnt/usb2
shell# cp target/UltraZed-EG-IOCC/boot/*                                  /mnt/usb1
shell# tar xfz debian9-rootfs-vanilla.tgz -C                              /mnt/usb2
shell# mkdir                                                              /mnt/usb2/home/fpga/debian
shell# cp linux-image-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb   /mnt/usb2/home/fpga/debian
shell# cp linux-headers-4.9.0-xlnx-v2017.3-zynqmp-fpga_4.9.0-xlnx-v2017.3-zynqmp-fpga-1_arm64.deb /mnt/usb2/home/fpga/debian
shell# umount mnt/usb1
shell# umount mnt/usb2
````

