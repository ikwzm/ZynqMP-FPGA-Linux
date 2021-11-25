/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2012 - 2018 Microchip Technology Inc., and its subsidiaries.
 * All rights reserved.
 */

#ifndef WILC_DEBUGFS_H
#define WILC_DEBUGFS_H

#include <linux/kern_levels.h>
#include <linux/version.h>
#if KERNEL_VERSION(3, 15, 0) > LINUX_VERSION_CODE
#include <linux/of_irq.h>
#endif

#define GENERIC_DBG		BIT(0)
#define HOSTAPD_DBG		BIT(1)
#define HOSTINF_DBG		BIT(2)
#define CORECONFIG_DBG		BIT(3)
#define CFG80211_DBG		BIT(4)
#define INT_DBG			BIT(5)
#define TX_DBG			BIT(6)
#define RX_DBG			BIT(7)
#define TCP_ENH			BIT(8)
#define INIT_DBG		BIT(9)
#define PWRDEV_DBG		BIT(10)
#define DBG_REGION_ALL		(BIT(11)-1)

extern atomic_t WILC_DEBUG_REGION;

#define PRINT_D(netdev, region, format, ...) do { \
	if (atomic_read(&WILC_DEBUG_REGION)&(region))\
		netdev_dbg(netdev, "DBG [%s: %d] "format, __func__, __LINE__,\
		   ##__VA_ARGS__); } \
	while (0)

#define PRINT_INFO(netdev, region, format, ...) do { \
	if (atomic_read(&WILC_DEBUG_REGION)&(region))\
		netdev_info(netdev, "INFO [%s]"format, __func__, \
		##__VA_ARGS__); } \
	while (0)

#define PRINT_WRN(netdev, region, format, ...) do { \
	if (atomic_read(&WILC_DEBUG_REGION)&(region))\
		netdev_warn(netdev, "WRN [%s: %d]"format, __func__, __LINE__,\
		    ##__VA_ARGS__); } \
	while (0)

#define PRINT_ER(netdev, format, ...) netdev_err(netdev, "ERR [%s:%d] "format,\
	__func__, __LINE__, ##__VA_ARGS__)

#ifdef WILC_DEBUGFS
int wilc_debugfs_init(void);
void wilc_debugfs_remove(void);
#endif

#if KERNEL_VERSION(3, 15, 0) > LINUX_VERSION_CODE
int of_irq_get(struct device_node *dev, int index);
#endif
#endif /* WILC_DEBUGFS_H */
