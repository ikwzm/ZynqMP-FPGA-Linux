
prefix         ?= ./
curr_dir       ?= $(shell pwd)
source_dir     ?= $(shell pwd)/udmabuf
lib_dir        ?= $(prefix)/lib/modules/4.9.0-xlnx-v2017.3-zynqmp-fpga/ikwzm
kernel_src_dir ?= $(curr_dir)/../linux-xlnx-v2017.3-zynqmp-fpga
ARCH           ?= arm64

kmod_objects += udmabuf/udmabuf.ko
kmod_subdirs  = $(dir $(kmod_objects))

.PHONY: all install

all:
	cd $(source_dir) ; $(MAKE) ARCH=$(ARCH) KERNEL_SRC_DIR=$(kernel_src_dir) all   ; cd $(curr_dir) ;

clean:
	cd $(source_dir) ; $(MAKE) ARCH=$(ARCH) KERNEL_SRC_DIR=$(kernel_src_dir) clean ; cd $(curr_dir) ;

install: all
	install -d $(lib_dir)
	install -d $(prefix)/usr/bin
	install -d $(prefix)/etc/systemd/system
	install -m 0644 $(source_dir)/udmabuf.ko   $(lib_dir)
	install -m 0644 systemd/udmabuf.service $(prefix)/etc/systemd/system/
