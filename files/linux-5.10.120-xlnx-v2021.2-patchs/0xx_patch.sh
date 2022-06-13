PATCH_DIR=$(cd $(dirname $0); pwd)

echo patch  $PATCH_DIR/001_arch-arm-boot-dts.patch
patch -p1 < $PATCH_DIR/001_arch-arm-boot-dts.patch
git add --all
git commit -m "[update] arch/arm/boot/dts"

echo patch  $PATCH_DIR/002_arch-arm-configs.patch
patch -p1 < $PATCH_DIR/002_arch-arm-configs.patch
git add --all
git commit -m "[add] arch/arm/configs/xilinx_zynq_defconfig"

echo patch  $PATCH_DIR/003_arch-arm-mach-zynq.patch
patch -p1 < $PATCH_DIR/003_arch-arm-mach-zynq.patch
git add --all
git commit -m "[update] arch/arm/mach-zynq"

echo patch  $PATCH_DIR/004_arch-arm64-boot-dts.patch
patch -p1 < $PATCH_DIR/004_arch-arm64-boot-dts.patch
git add --all
git commit -m "[update] arch/arm64/boot/dts"

echo patch  $PATCH_DIR/005_arch-arm64-configs.patch
patch -p1 < $PATCH_DIR/005_arch-arm64-configs.patch
git add --all
git commit -m "[update] arch/arm64/configs"

echo patch  $PATCH_DIR/006_cpuhotplug.patch
patch -p1 < $PATCH_DIR/006_cpuhotplug.patch
git add --all
git commit -m "[update] include/linux/cpuhotplug.h"

echo patch  $PATCH_DIR/010_drivers-ata.patch
patch -p1 < $PATCH_DIR/010_drivers-ata.patch
git add --all
git commit -m "[update] drivers/ata"

echo patch  $PATCH_DIR/011_drivers-bluetooth.patch
patch -p1 < $PATCH_DIR/011_drivers-bluetooth.patch
git add --all
git commit -m "[update] drivers/bluetooth"

echo patch  $PATCH_DIR/012_drivers-clk.patch
patch -p1 < $PATCH_DIR/012_drivers-clk.patch
git add --all
git commit -m "[update] drivers/clk"

echo patch  $PATCH_DIR/013_drivers-crypto.patch
patch -p1 < $PATCH_DIR/013_drivers-crypto.patch
git add --all
git commit -m "[update] drivers/crypto"

echo patch  $PATCH_DIR/014_drivers-dma.patch
patch -p1 < $PATCH_DIR/014_drivers-dma.patch
git add --all
git commit -m "[update] drivers/dma"

echo patch  $PATCH_DIR/015_drivers-edac.patch
patch -p1 < $PATCH_DIR/015_drivers-edac.patch
git add --all
git commit -m "[update] drivers/edac"

echo patch  $PATCH_DIR/016_drivers-firmware.patch
patch -p1 < $PATCH_DIR/016_drivers-firmware.patch
git add --all
git commit -m "[update] drivers/firmware"

echo patch  $PATCH_DIR/017_drivers-fpga.patch
patch -p1 < $PATCH_DIR/017_drivers-fpga.patch
git add --all
git commit -m "[update] drivers/fpga"

echo patch  $PATCH_DIR/018_drivers-gpio.patch
patch -p1 < $PATCH_DIR/018_drivers-gpio.patch
git add --all
git commit -m "[update] drivers/gpio"

echo patch  $PATCH_DIR/019_drivers-gpu-drm.patch
patch -p1 < $PATCH_DIR/019_drivers-gpu-drm.patch
git add --all
git commit -m "[update] drivers/gpu/drm"

echo patch  $PATCH_DIR/020_drivers-hwmon.patch
patch -p1 < $PATCH_DIR/020_drivers-hwmon.patch
git add --all
git commit -m "[update] drivers/hwmon"

echo patch  $PATCH_DIR/021_drivers-i2c.patch
patch -p1 < $PATCH_DIR/021_drivers-i2c.patch
git add --all
git commit -m "[update] drivers/i2c"

echo patch  $PATCH_DIR/022_drivers-iio.patch
patch -p1 < $PATCH_DIR/022_drivers-iio.patch
git add --all
git commit -m "[update] drivers/iio"

echo patch  $PATCH_DIR/023_drivers-irqchip.patch
patch -p1 < $PATCH_DIR/023_drivers-irqchip.patch
git add --all
git commit -m "[update] drivers/irqchip"

echo patch  $PATCH_DIR/024_drivers-media-i2c.patch
patch -p1 < $PATCH_DIR/024_drivers-media-i2c.patch
git add --all
git commit -m "[update] drivers/media/i2c"

echo patch  $PATCH_DIR/025_drivers-media-platform-xilinx.patch
patch -p1 < $PATCH_DIR/025_drivers-media-platform-xilinx.patch
git add --all
git commit -m "[update] drivers/media/platform/xilinx"

echo patch  $PATCH_DIR/026_drivers-memory.patch
patch -p1 < $PATCH_DIR/026_drivers-memory.patch
git add --all
git commit -m "[update] drivers/memory"

echo patch  $PATCH_DIR/027_drivers-misc.patch
patch -p1 < $PATCH_DIR/027_drivers-misc.patch
git add --all
git commit -m "[update] drivers/misc"

echo patch  $PATCH_DIR/028_drivers-mmc.patch
patch -p1 < $PATCH_DIR/028_drivers-mmc.patch
git add --all
git commit -m "[update] drivers/mmc"

echo patch  $PATCH_DIR/029_drivers-mtd.patch
patch -p1 < $PATCH_DIR/029_drivers-mtd.patch
git add --all
git commit -m "[update] drivers/mtd"

echo patch  $PATCH_DIR/030_drivers-net-can.patch
patch -p1 < $PATCH_DIR/030_drivers-net-can.patch
git add --all
git commit -m "[update] drivers/net/can"

echo patch  $PATCH_DIR/031_drivers-net-ethernet-cadence.patch
patch -p1 < $PATCH_DIR/031_drivers-net-ethernet-cadence.patch
git add --all
git commit -m "[update] drivers/net/ethernet/cadence"

echo patch  $PATCH_DIR/032_drivers-net-ethernet-xilinx.patch
patch -p1 < $PATCH_DIR/032_drivers-net-ethernet-xilinx.patch

git add --all
git commit -m "[update] drivers/net/ethernet/xilinx"

echo patch  $PATCH_DIR/033_drivers-net-phy.patch
patch -p1 < $PATCH_DIR/033_drivers-net-phy.patch
git add --all
git commit -m "[update] drivers/net/phy"

echo patch  $PATCH_DIR/034_drivers-nvmem.patch
patch -p1 < $PATCH_DIR/034_drivers-nvmem.patch
git add --all
git commit -m "[update] drivers/nvmem"

echo patch  $PATCH_DIR/035_drivers-of.patch
patch -p1 < $PATCH_DIR/035_drivers-of.patch
git add --all
git commit -m "[update] drivers/of"

echo patch  $PATCH_DIR/036_drivers-pci.patch
patch -p1 < $PATCH_DIR/036_drivers-pci.patch
git add --all
git commit -m "[update] drivers/pci"

echo patch  $PATCH_DIR/037_drivers-phy.patch
patch -p1 < $PATCH_DIR/037_drivers-phy.patch
git add --all
git commit -m "[update] drivers/phy"

echo patch  $PATCH_DIR/038_drivers-pinctrl.patch
patch -p1 < $PATCH_DIR/038_drivers-pinctrl.patch
git add --all
git commit -m "[update] drivers/pinctrl"

echo patch  $PATCH_DIR/039_drivers-ptp.patch
patch -p1 < $PATCH_DIR/039_drivers-ptp.patch
git add --all
git commit -m "[update] drivers/ptp"

echo patch  $PATCH_DIR/040_drivers-regulator.patch
patch -p1 < $PATCH_DIR/040_drivers-regulator.patch
git add --all
git commit -m "[update] drivers/regulator"

echo patch  $PATCH_DIR/041_drivers-remoteproc.patch
patch -p1 < $PATCH_DIR/041_drivers-remoteproc.patch
git add --all
git commit -m "[update] drivers/remoteproc"

echo patch  $PATCH_DIR/042_drivers-rtc.patch
patch -p1 < $PATCH_DIR/042_drivers-rtc.patch
git add --all
git commit -m "[update] drivers/rtc"

echo patch  $PATCH_DIR/043_drivers-soc-xilinx.patch
patch -p1 < $PATCH_DIR/043_drivers-soc-xilinx.patch
git add --all
git commit -m "[update] drivers/soc/xilinx"

echo patch  $PATCH_DIR/044_drivers-spi.patch
patch -p1 < $PATCH_DIR/044_drivers-spi.patch
git add --all
git commit -m "[update] drivers/spi"

echo patch  $PATCH_DIR/045_drivers-tty.patch
patch -p1 < $PATCH_DIR/045_drivers-tty.patch
git add --all
git commit -m "[update] drivers/tty"

echo patch  $PATCH_DIR/046_drivers-uio.patch
patch -p1 < $PATCH_DIR/046_drivers-uio.patch
git add --all
git commit -m "[update] drivers/uio"

echo patch  $PATCH_DIR/047_drivers-watchdog.patch
patch -p1 < $PATCH_DIR/047_drivers-watchdog.patch
git add --all
git commit -m "[update] drivers/watchdog"

echo patch  $PATCH_DIR/048_drivers-xen.patch
patch -p1 < $PATCH_DIR/048_drivers-xen.patch
git add --all
git commit -m "[update] drivers/xen"

echo patch  $PATCH_DIR/050_drivers-staging-android-ion.patch
patch -p1 < $PATCH_DIR/050_drivers-staging-android-ion.patch
git add --all
git commit -m "[update] drivers/staging/android/ion"

echo patch  $PATCH_DIR/051_drivers-staging-apf.patch
patch -p1 < $PATCH_DIR/051_drivers-staging-apf.patch
git add --all
git commit -m "[update] drivers/staging/apf"

echo patch  $PATCH_DIR/052_drivers-staging-fclk.patch
patch -p1 < $PATCH_DIR/052_drivers-staging-fclk.patch
git add --all
git commit -m "[update] drivers/staging/fclk"

echo patch  $PATCH_DIR/053_drivers_staging-xlnx_tsmux.patch
patch -p1 < $PATCH_DIR/053_drivers_staging-xlnx_tsmux.patch
git add --all
git commit -m "[update] drivers/staging/xlnx_tsmux"

echo patch  $PATCH_DIR/054_drivers-staging-xlnxsync.patch
patch -p1 < $PATCH_DIR/054_drivers-staging-xlnxsync.patch
git add --all
git commit -m "[update] drivers/staging/xlnxsync"

echo patch  $PATCH_DIR/055_drivers-staging-xroeframer.patch
patch -p1 < $PATCH_DIR/055_drivers-staging-xroeframer.patch
git add --all
git commit -m "[update] drivers/staging/xroeframer"

echo patch  $PATCH_DIR/056_drivers-staging-xroetrafficgen.patch
patch -p1 < $PATCH_DIR/056_drivers-staging-xroetrafficgen.patch
git add --all
git commit -m "[update] drivers/staging/xroetrafficgen"

echo patch  $PATCH_DIR/059_drivers-staging.patch
patch -p1 < $PATCH_DIR/059_drivers-staging.patch
git add --all
git commit -m "[update] drivers/staging"

echo patch  $PATCH_DIR/060_drivers-usb-chipidea.patch
patch -p1 < $PATCH_DIR/060_drivers-usb-chipidea.patch
git add --all
git commit -m "[update] drivers/usb/chipidea"

echo patch  $PATCH_DIR/061_drivers-usb-dwc3.patch
patch -p1 < $PATCH_DIR/061_drivers-usb-dwc3.patch
git add --all
git commit -m "[update] drivers/usb/dwc3"

echo patch  $PATCH_DIR/062_drivers-usb-gadget.patch
patch -p1 < $PATCH_DIR/062_drivers-usb-gadget.patch
git add --all
git commit -m "[update] drivers/usb/gadget"

echo patch  $PATCH_DIR/063_drivers-usb-host.patch
patch -p1 < $PATCH_DIR/063_drivers-usb-host.patch
git add --all
git commit -m "[update] drivers/usb/host"

echo patch  $PATCH_DIR/064_drivers-usb-misc.patch
patch -p1 < $PATCH_DIR/064_drivers-usb-misc.patch
git add --all
git commit -m "[update] drivers/usb/misc"

echo patch  $PATCH_DIR/065_drivers-usb-phy.patch
patch -p1 < $PATCH_DIR/065_drivers-usb-phy.patch
git add --all
git commit -m "[update] drivers/usb/phy"

echo patch  $PATCH_DIR/066_drivers-usb-strage.patch
patch -p1 < $PATCH_DIR/066_drivers-usb-strage.patch
git add --all
git commit -m "[update] drivers/usb/strage"

echo patch  $PATCH_DIR/070_misc.patch
patch -p1 < $PATCH_DIR/070_misc.patch
git add --all
git commit -m "[update] misc"

echo patch  $PATCH_DIR/071_mm-page_alloc.patch
patch -p1 < $PATCH_DIR/071_mm-page_alloc.patch
git add --all
git commit -m "[update] mm/page_alloc.c"

echo patch  $PATCH_DIR/072_sound-soc-xilinx.patch
patch -p1 < $PATCH_DIR/072_sound-soc-xilinx.patch
git add --all
git commit -m "[update] sound/soc/xilinx"

echo patch  $PATCH_DIR/073_xilinx-hls.patch
patch -p1 < $PATCH_DIR/073_xilinx-hls.patch
git add --all
git commit -m "[update] include/uapi/linux/xilinx-hls.h"

