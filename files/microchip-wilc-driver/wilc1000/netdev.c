// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2012 - 2018 Microchip Technology Inc., and its subsidiaries.
 * All rights reserved.
 */

#include <linux/irq.h>
#include <linux/kthread.h>
#include <linux/firmware.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>

#include "cfg80211.h"
#include "wlan_cfg.h"

#define WILC_MULTICAST_TABLE_SIZE	8

/* latest API version supported */
#define WILC1000_API_VER		1

#define WILC1000_FW_PREFIX		"mchp/wilc1000_wifi_firmware"
#define __WILC1000_FW(api)		WILC1000_FW_PREFIX #api ".bin"
#define WILC1000_FW(api)		__WILC1000_FW(api)

#define WILC3000_API_VER		1

#define WILC3000_FW_PREFIX		"mchp/wilc3000_wifi_firmware"
#define __WILC3000_FW(api)		WILC3000_FW_PREFIX #api ".bin"
#define WILC3000_FW(api)		__WILC3000_FW(api)

static int wilc_mac_open(struct net_device *ndev);
static int wilc_mac_close(struct net_device *ndev);

static int debug_running;
static int recovery_on;
int wait_for_recovery;
static int debug_thread(void *arg)
{
	struct wilc *wl = arg;
	struct wilc_vif *vif;
	signed long timeout;
	struct host_if_drv *hif_drv;
	int i = 0;

	complete(&wl->debug_thread_started);

	while (1) {
		int srcu_idx;
		int ret;

		if (!wl->initialized && !kthread_should_stop()) {
			msleep(1000);
			continue;
		} else if (!wl->initialized) {
			break;
		}
		ret = wait_for_completion_interruptible_timeout(
			&wl->debug_thread_started, msecs_to_jiffies(6000));
		if (ret > 0) {
			while (!kthread_should_stop())
				schedule();
			pr_info("Exit debug thread\n");
			return 0;
		}
		if (!debug_running || ret == -ERESTARTSYS)
			continue;

		pr_debug("%s *** Debug Thread Running ***cnt[%d]\n", __func__,
			 cfg_packet_timeout);

		if (cfg_packet_timeout < 5)
			continue;

		pr_info("%s <Recover>\n", __func__);
		cfg_packet_timeout = 0;
		timeout = 10;
		recovery_on = 1;
		wait_for_recovery = 1;

		srcu_idx = srcu_read_lock(&wl->srcu);
		list_for_each_entry_rcu(vif, &wl->vif_list, list) {
			/* close the interface only if it was open */
			if (vif->mac_opened) {
				wilc_mac_close(vif->ndev);
				vif->restart = 1;
			}
		}

		/* For Spi, clear 'is_init' flag so that protocol offset
		 * register can be send to FW to setup required crc mode after
		 * chip reset
		 */
		if (wl->io_type == WILC_HIF_SPI)
			wl->hif_func->hif_clear_init(wl);

		//TODO://Need to find way to call them in reverse
		i = 0;
		list_for_each_entry_rcu(vif, &wl->vif_list, list) {
			struct wilc_conn_info *info;

			/* Only open the interface manually closed earlier */
			if (!vif->restart)
				continue;
			i++;
			hif_drv = vif->priv.hif_drv;
			while (wilc_mac_open(vif->ndev) && --timeout)
				msleep(100);

			if (timeout == 0)
				PRINT_WRN(vif->ndev, GENERIC_DBG,
					  "Couldn't restart ifc %d\n", i);

			if (hif_drv->hif_state == HOST_IF_CONNECTED) {
				info = &hif_drv->conn_info;
				PRINT_INFO(vif->ndev, GENERIC_DBG,
					   "notify the user with the Disconnection\n");
				if (hif_drv->usr_scan_req.scan_result) {
					PRINT_INFO(vif->ndev, GENERIC_DBG,
						   "Abort the running OBSS Scan\n");
					del_timer(&hif_drv->scan_timer);
					handle_scan_done(vif,
							 SCAN_EVENT_ABORTED);
				}
				if (info->conn_result) {
					info->conn_result(CONN_DISCONN_EVENT_DISCONN_NOTIF,
							  0, info->arg);
				} else {
					PRINT_ER(vif->ndev,
						 "Connect result NULL\n");
				}
				eth_zero_addr(hif_drv->assoc_bssid);
				info->req_ies_len = 0;
				kfree(info->req_ies);
				info->req_ies = NULL;
				hif_drv->hif_state = HOST_IF_IDLE;
			}
			vif->restart = 0;
		}
		srcu_read_unlock(&wl->srcu, srcu_idx);
		recovery_on = 0;
	}
	return 0;
}

static void wilc_disable_irq(struct wilc *wilc, int wait)
{
	if (wait) {
		pr_info("%s Disabling IRQ ...\n", __func__);
		disable_irq(wilc->dev_irq_num);
	} else {
		pr_info("%s Disabling IRQ ...\n", __func__);
		disable_irq_nosync(wilc->dev_irq_num);
	}
}

static irqreturn_t host_wakeup_isr(int irq, void *user_data)
{
	return IRQ_HANDLED;
}

static irqreturn_t isr_uh_routine(int irq, void *user_data)
{
	struct wilc *wilc = user_data;

	if (wilc->close) {
		pr_err("%s: Can't handle UH interrupt\n", __func__);
		return IRQ_HANDLED;
	}
	return IRQ_WAKE_THREAD;
}

static irqreturn_t isr_bh_routine(int irq, void *userdata)
{
	struct wilc *wilc = userdata;

	if (wilc->close) {
		pr_err("%s: Can't handle BH interrupt\n", __func__);
		return IRQ_HANDLED;
	}

	wilc_handle_isr(wilc);

	return IRQ_HANDLED;
}

static int init_irq(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wl = vif->wilc;

	if (wl->dev_irq_num <= 0)
		return 0;

	if (wl->io_type == WILC_HIF_SPI ||
	    wl->io_type == WILC_HIF_SDIO_GPIO_IRQ) {
		if (request_threaded_irq(wl->dev_irq_num, isr_uh_routine,
					 isr_bh_routine,
					 IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND,
					 "WILC_IRQ", wl) < 0) {
			PRINT_ER(dev, "Failed to request IRQ [%d]\n",
				 wl->dev_irq_num);
			return -EINVAL;
		}
	} else {
		if (request_irq(wl->dev_irq_num, host_wakeup_isr,
				IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND,
				"WILC_IRQ", wl) < 0) {
			PRINT_ER(dev, "Failed to request IRQ [%d]\n",
				 wl->dev_irq_num);
			return -EINVAL;
		}
	}

	PRINT_INFO(dev, GENERIC_DBG, "IRQ request succeeded IRQ-NUM= %d\n",
		   wl->dev_irq_num);
	enable_irq_wake(wl->dev_irq_num);
	return 0;
}

static void deinit_irq(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wilc = vif->wilc;

	/* Deinitialize IRQ */
	if (wilc->dev_irq_num)
		free_irq(wilc->dev_irq_num, wilc);
}

void wilc_mac_indicate(struct wilc *wilc)
{
	s8 status;

	wilc_wlan_cfg_get_val(wilc, WID_STATUS, &status, 1);
	if (wilc->mac_status == WILC_MAC_STATUS_INIT) {
		wilc->mac_status = status;
		complete(&wilc->sync_event);
	} else {
		wilc->mac_status = status;
	}
}

static void free_eap_buff_params(void *vp)
{
	struct wilc_priv *priv;

	priv = (struct wilc_priv *)vp;

	if (priv->buffered_eap) {
		kfree(priv->buffered_eap->buff);
		priv->buffered_eap->buff = NULL;

		kfree(priv->buffered_eap);
		priv->buffered_eap = NULL;
	}
}

#if KERNEL_VERSION(4, 15, 0) <= LINUX_VERSION_CODE
void eap_buff_timeout(struct timer_list *t)
#else
void eap_buff_timeout(unsigned long user)
#endif
{
	u8 null_bssid[ETH_ALEN] = {0};
	u8 *assoc_bss;
	static u8 timeout = 5;
	int status = -1;
#if KERNEL_VERSION(4, 15, 0) <= LINUX_VERSION_CODE
	struct wilc_priv *priv = from_timer(priv, t, eap_buff_timer);
#else
	struct wilc_priv *priv = (struct wilc_priv *)user;
#endif
	struct wilc_vif *vif = netdev_priv(priv->dev);

	assoc_bss = priv->associated_bss;
	if (!(memcmp(assoc_bss, null_bssid, ETH_ALEN)) && (timeout-- > 0)) {
		mod_timer(&priv->eap_buff_timer,
			  (jiffies + msecs_to_jiffies(10)));
		return;
	}
	del_timer(&priv->eap_buff_timer);
	timeout = 5;

	status = wilc_send_buffered_eap(vif, wilc_frmw_to_host,
					free_eap_buff_params,
					priv->buffered_eap->buff,
					priv->buffered_eap->size,
					priv->buffered_eap->pkt_offset,
					(void *)priv);
	if (status)
		PRINT_ER(vif->ndev, "Failed so send buffered eap\n");
}

void wilc_wlan_set_bssid(struct net_device *wilc_netdev, u8 *bssid, u8 mode)
{
	struct wilc_vif *vif = netdev_priv(wilc_netdev);
	struct wilc *wilc = vif->wilc;
	int srcu_idx;

	srcu_idx = srcu_read_lock(&wilc->srcu);
	list_for_each_entry_rcu(vif, &wilc->vif_list, list) {
		if (wilc_netdev == vif->ndev) {
			if (bssid)
				ether_addr_copy(vif->bssid, bssid);
			else
				eth_zero_addr(vif->bssid);
			PRINT_INFO(vif->ndev, GENERIC_DBG,
				   "set bssid [%pM]\n", vif->bssid);
			vif->iftype = mode;
		}
	}
	srcu_read_unlock(&wilc->srcu, srcu_idx);
}

#define TX_BACKOFF_WEIGHT_INCR_STEP (1)
#define TX_BACKOFF_WEIGHT_DECR_STEP (1)
#define TX_BACKOFF_WEIGHT_MAX (0)
#define TX_BACKOFF_WEIGHT_MIN (0)
#define TX_BCKOFF_WGHT_MS (1)

static int wilc_txq_task(void *vp)
{
	int ret;
	u32 txq_count;
	int backoff_weight = TX_BACKOFF_WEIGHT_MIN;
	signed long timeout;
	struct wilc *wl = vp;

	complete(&wl->txq_thread_started);
	while (1) {
		struct wilc_vif *vif = wilc_get_wl_to_vif(wl);
		struct net_device *ndev = vif->ndev;

		PRINT_INFO(ndev, TX_DBG, "txq_task Taking a nap\n");
		if (wait_for_completion_interruptible(&wl->txq_event))
			continue;
		PRINT_INFO(ndev, TX_DBG, "txq_task Who waked me up\n");
		if (wl->close) {
			complete(&wl->txq_thread_started);

			while (!kthread_should_stop())
				schedule();
			PRINT_INFO(ndev, TX_DBG, "TX thread stopped\n");
			break;
		}
		PRINT_INFO(ndev, TX_DBG, "handle the tx packet\n");
		do {
			ret = wilc_wlan_handle_txq(wl, &txq_count);
			if (txq_count < FLOW_CONTROL_LOWER_THRESHOLD) {
				int srcu_idx;
				struct wilc_vif *ifc;

				srcu_idx = srcu_read_lock(&wl->srcu);
				PRINT_INFO(ndev, TX_DBG, "Waking up queue\n");
				list_for_each_entry_rcu(ifc, &wl->vif_list,
							list) {
					if (ifc->mac_opened &&
					    netif_queue_stopped(ifc->ndev))
						netif_wake_queue(ifc->ndev);
				}
				srcu_read_unlock(&wl->srcu, srcu_idx);
			}

			if (ret == WILC_VMM_ENTRY_FULL_RETRY) {
				timeout = msecs_to_jiffies(TX_BCKOFF_WGHT_MS <<
							   backoff_weight);
				do {
			/* Back off from sending packets for some time.
			 * schedule_timeout will allow RX task to run and free
			 * buffers. Setting state to TASK_INTERRUPTIBLE will
			 * put the thread back to CPU running queue when it's
			 * signaled even if 'timeout' isn't elapsed. This gives
			 * faster chance for reserved SK buffers to be freed
			 */
					set_current_state(TASK_INTERRUPTIBLE);
					timeout = schedule_timeout(timeout);
					} while (/*timeout*/0);
				backoff_weight += TX_BACKOFF_WEIGHT_INCR_STEP;
				if (backoff_weight > TX_BACKOFF_WEIGHT_MAX)
					backoff_weight = TX_BACKOFF_WEIGHT_MAX;
			} else if (backoff_weight > TX_BACKOFF_WEIGHT_MIN) {
				backoff_weight -= TX_BACKOFF_WEIGHT_DECR_STEP;
				if (backoff_weight < TX_BACKOFF_WEIGHT_MIN)
					backoff_weight = TX_BACKOFF_WEIGHT_MIN;
			}
		} while (ret == WILC_VMM_ENTRY_FULL_RETRY && !wl->close);
	}
	return 0;
}

static int wilc_wlan_get_firmware(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wilc = vif->wilc;
	const struct firmware *wilc_fw;
	char *firmware;
	int ret;

	if (wilc->chip == WILC_3000) {
		PRINT_INFO(dev, INIT_DBG, "Detect chip WILC3000\n");
		firmware = WILC3000_FW();
	} else if (wilc->chip == WILC_1000) {
		PRINT_INFO(dev, INIT_DBG, "Detect chip WILC1000\n");
		firmware = WILC1000_FW();
	} else {
		return -EINVAL;
	}

	PRINT_INFO(dev, INIT_DBG, "loading firmware %s\n", firmware);

	if (!(&vif->ndev->dev)) {
		PRINT_ER(dev, "Dev  is NULL\n");
		return -EINVAL;
	}

	PRINT_INFO(vif->ndev, INIT_DBG, "WLAN firmware: %s\n", firmware);
	ret = request_firmware(&wilc_fw, firmware, wilc->dev);
	if (ret != 0) {
		PRINT_ER(dev, "%s - firmware not available\n", firmware);
		return -EINVAL;
	}
	wilc->firmware = wilc_fw;

	return 0;
}

static int wilc_start_firmware(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wilc = vif->wilc;
	int ret = 0;

	PRINT_INFO(vif->ndev, INIT_DBG, "Starting Firmware ...\n");

	ret = wilc_wlan_start(wilc);
	if (ret < 0) {
		PRINT_ER(dev, "Failed to start Firmware\n");
		return ret;
	}
	PRINT_INFO(vif->ndev, INIT_DBG, "Waiting for FW to get ready ...\n");

	if (!wait_for_completion_timeout(&wilc->sync_event,
					 msecs_to_jiffies(500))) {
		PRINT_INFO(vif->ndev, INIT_DBG, "Firmware start timed out\n");
		return -ETIME;
	}
	PRINT_INFO(vif->ndev, INIT_DBG, "Firmware successfully started\n");

	return 0;
}

static int wilc_firmware_download(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wilc = vif->wilc;
	int ret = 0;

	if (!wilc->firmware) {
		PRINT_ER(dev, "Firmware buffer is NULL\n");
		ret = -ENOBUFS;
	}
	PRINT_INFO(vif->ndev, INIT_DBG, "Downloading Firmware ...\n");
	ret = wilc_wlan_firmware_download(wilc, wilc->firmware->data,
					  wilc->firmware->size);
	if (ret < 0)
		goto fail;

	PRINT_INFO(vif->ndev, INIT_DBG, "Download Succeeded\n");

fail:
	release_firmware(wilc->firmware);
	wilc->firmware = NULL;

	return ret;
}

static int wilc_init_fw_config(struct net_device *dev, struct wilc_vif *vif)
{
	struct wilc_priv *priv = &vif->priv;
	struct host_if_drv *hif_drv;
	u8 b;
	u16 hw;
	u32 w;

	PRINT_INFO(vif->ndev, INIT_DBG, "Start configuring Firmware\n");
	hif_drv = (struct host_if_drv *)priv->hif_drv;
	PRINT_D(vif->ndev, INIT_DBG, "Host = %p\n", hif_drv);

	w = vif->iftype;
	cpu_to_le32s(&w);
	if (!wilc_wlan_cfg_set(vif, 1, WID_SET_OPERATION_MODE, (u8 *)&w, 4, 0,
			       0))
		goto fail;

	b = WILC_FW_BSS_TYPE_INFRA;
	if (!wilc_wlan_cfg_set(vif, 0, WID_BSS_TYPE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_TX_RATE_AUTO;
	if (!wilc_wlan_cfg_set(vif, 0, WID_CURRENT_TX_RATE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_OPER_MODE_G_MIXED_11B_2;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11G_OPERATING_MODE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_PREAMBLE_AUTO;
	if (!wilc_wlan_cfg_set(vif, 0, WID_PREAMBLE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_11N_PROT_AUTO;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_PROT_MECH, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_ACTIVE_SCAN;
	if (!wilc_wlan_cfg_set(vif, 0, WID_SCAN_TYPE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_SITE_SURVEY_OFF;
	if (!wilc_wlan_cfg_set(vif, 0, WID_SITE_SURVEY, &b, 1, 0, 0))
		goto fail;

	hw = 0xffff;
	cpu_to_le16s(&hw);
	if (!wilc_wlan_cfg_set(vif, 0, WID_RTS_THRESHOLD, (u8 *)&hw, 2, 0, 0))
		goto fail;

	hw = 2346;
	cpu_to_le16s(&hw);
	if (!wilc_wlan_cfg_set(vif, 0, WID_FRAG_THRESHOLD, (u8 *)&hw, 2, 0, 0))
		goto fail;

	b = 0;
	if (!wilc_wlan_cfg_set(vif, 0, WID_BCAST_SSID, &b, 1, 0, 0))
		goto fail;

	b = 1;
	if (!wilc_wlan_cfg_set(vif, 0, WID_QOS_ENABLE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_NO_POWERSAVE;
	if (!wilc_wlan_cfg_set(vif, 0, WID_POWER_MANAGEMENT, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_SEC_NO;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11I_MODE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_AUTH_OPEN_SYSTEM;
	if (!wilc_wlan_cfg_set(vif, 0, WID_AUTH_TYPE, &b, 1, 0, 0))
		goto fail;

	b = 3;
	if (!wilc_wlan_cfg_set(vif, 0, WID_LISTEN_INTERVAL, &b, 1, 0, 0))
		goto fail;

	b = 3;
	if (!wilc_wlan_cfg_set(vif, 0, WID_DTIM_PERIOD, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_ACK_POLICY_NORMAL;
	if (!wilc_wlan_cfg_set(vif, 0, WID_ACK_POLICY, &b, 1, 0, 0))
		goto fail;

	b = 0;
	if (!wilc_wlan_cfg_set(vif, 0, WID_USER_CONTROL_ON_TX_POWER, &b, 1,
			       0, 0))
		goto fail;

	b = 48;
	if (!wilc_wlan_cfg_set(vif, 0, WID_TX_POWER_LEVEL_11A, &b, 1, 0, 0))
		goto fail;

	b = 28;
	if (!wilc_wlan_cfg_set(vif, 0, WID_TX_POWER_LEVEL_11B, &b, 1, 0, 0))
		goto fail;

	hw = 100;
	cpu_to_le16s(&hw);
	if (!wilc_wlan_cfg_set(vif, 0, WID_BEACON_INTERVAL, (u8 *)&hw, 2, 0, 0))
		goto fail;

	b = WILC_FW_REKEY_POLICY_DISABLE;
	if (!wilc_wlan_cfg_set(vif, 0, WID_REKEY_POLICY, &b, 1, 0, 0))
		goto fail;

	w = 84600;
	cpu_to_le32s(&w);
	if (!wilc_wlan_cfg_set(vif, 0, WID_REKEY_PERIOD, (u8 *)&w, 4, 0, 0))
		goto fail;

	w = 500;
	cpu_to_le32s(&w);
	if (!wilc_wlan_cfg_set(vif, 0, WID_REKEY_PACKET_COUNT, (u8 *)&w, 4, 0,
			       0))
		goto fail;

	b = 1;
	if (!wilc_wlan_cfg_set(vif, 0, WID_SHORT_SLOT_ALLOWED, &b, 1, 0,
			       0))
		goto fail;

	b = WILC_FW_ERP_PROT_SELF_CTS;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_ERP_PROT_TYPE, &b, 1, 0, 0))
		goto fail;

	b = 1;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_ENABLE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_11N_OP_MODE_HT_MIXED;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_OPERATING_MODE, &b, 1, 0, 0))
		goto fail;

	b = 1;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_TXOP_PROT_DISABLE, &b, 1, 0, 0))
		goto fail;

	b = WILC_FW_OBBS_NONHT_DETECT_PROTECT_REPORT;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_OBSS_NONHT_DETECTION, &b, 1,
			       0, 0))
		goto fail;

	b = WILC_FW_HT_PROT_RTS_CTS_NONHT;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_HT_PROT_TYPE, &b, 1, 0, 0))
		goto fail;

	b = 0;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_RIFS_PROT_ENABLE, &b, 1, 0,
			       0))
		goto fail;

	b = 7;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_CURRENT_TX_MCS, &b, 1, 0, 0))
		goto fail;

	b = 1;
	if (!wilc_wlan_cfg_set(vif, 0, WID_11N_IMMEDIATE_BA_ENABLED, &b, 1,
			       1, 0))
		goto fail;

	return 0;

fail:
	return -EINVAL;
}

static void wlan_deinitialize_threads(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wl = vif->wilc;

	PRINT_INFO(vif->ndev, INIT_DBG, "Deinitializing Threads\n");
	if (!recovery_on) {
		PRINT_INFO(vif->ndev, INIT_DBG, "Deinit debug Thread\n");
		debug_running = false;
		if (&wl->debug_thread_started)
			complete(&wl->debug_thread_started);
		if (wl->debug_thread) {
			kthread_stop(wl->debug_thread);
			wl->debug_thread = NULL;
		}
	}

	wl->close = 1;
	PRINT_INFO(vif->ndev, INIT_DBG, "Deinitializing Threads\n");

	complete(&wl->txq_event);

	if (wl->txq_thread) {
		kthread_stop(wl->txq_thread);
		wl->txq_thread = NULL;
	}
}

static void wilc_wlan_deinitialize(struct net_device *dev)
{
	int ret;
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wl = vif->wilc;

	if (!wl) {
		PRINT_ER(dev, "wl is NULL\n");
		return;
	}

	if (wl->initialized) {
		PRINT_INFO(vif->ndev, INIT_DBG, "Deinitializing wilc  ...\n");

		PRINT_D(vif->ndev, INIT_DBG, "destroy aging timer\n");

		PRINT_INFO(vif->ndev, INIT_DBG, "Disabling IRQ\n");
		if (wl->io_type == WILC_HIF_SPI ||
		    wl->io_type == WILC_HIF_SDIO_GPIO_IRQ) {
			wilc_disable_irq(wl, 1);
		} else {
			if (wl->hif_func->disable_interrupt) {
				mutex_lock(&wl->hif_cs);
				wl->hif_func->disable_interrupt(wl);
				mutex_unlock(&wl->hif_cs);
			}
		}
		complete(&wl->txq_event);

		PRINT_INFO(vif->ndev, INIT_DBG, "Deinitializing Threads\n");
		wlan_deinitialize_threads(dev);
		PRINT_INFO(vif->ndev, INIT_DBG, "Deinitializing IRQ\n");
		deinit_irq(dev);

		ret = wilc_wlan_stop(wl, vif);
		if (ret != 0)
			PRINT_ER(dev, "failed in wlan_stop\n");

		PRINT_INFO(vif->ndev, INIT_DBG, "Deinitializing WILC Wlan\n");
		wilc_wlan_cleanup(dev);

		wl->initialized = false;

		PRINT_INFO(dev, INIT_DBG, "wilc deinitialization Done\n");
	} else {
		PRINT_INFO(dev, INIT_DBG, "wilc is not initialized\n");
	}
}

static int wlan_initialize_threads(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wilc = vif->wilc;

	PRINT_INFO(vif->ndev, INIT_DBG, "Initializing Threads ...\n");
	PRINT_INFO(vif->ndev, INIT_DBG, "Creating kthread for transmission\n");
	wilc->txq_thread = kthread_run(wilc_txq_task, (void *)wilc,
				       "K_TXQ_TASK");
	if (IS_ERR(wilc->txq_thread)) {
		netdev_err(dev, "couldn't create TXQ thread\n");
		wilc->close = 1;
		return PTR_ERR(wilc->txq_thread);
	}
	wait_for_completion(&wilc->txq_thread_started);

	if (!debug_running) {
		PRINT_INFO(vif->ndev, INIT_DBG,
			   "Creating kthread for Debugging\n");
		wilc->debug_thread = kthread_run(debug_thread, (void *)wilc,
						 "WILC_DEBUG");
		if (IS_ERR(wilc->debug_thread)) {
			PRINT_ER(dev, "couldn't create debug thread\n");
			wilc->close = 1;
			kthread_stop(wilc->txq_thread);
			return PTR_ERR(wilc->debug_thread);
		}
		debug_running = true;
		wait_for_completion(&wilc->debug_thread_started);
	}

	return 0;
}

static int wilc_wlan_initialize(struct net_device *dev, struct wilc_vif *vif)
{
	int ret = 0;
	struct wilc *wl = vif->wilc;

	if (!wl->initialized) {
		wl->mac_status = WILC_MAC_STATUS_INIT;
		wl->close = 0;

		ret = wilc_wlan_init(dev);
		if (ret) {
			PRINT_ER(dev, "Initializing WILC_Wlan FAILED\n");
			return ret;
		}
		PRINT_INFO(vif->ndev, GENERIC_DBG,
			   "WILC Initialization done\n");

		ret = wlan_initialize_threads(dev);
		if (ret) {
			PRINT_ER(dev, "Initializing Threads FAILED\n");
			goto fail_wilc_wlan;
		}

		ret = init_irq(dev);
		if (ret)
			goto fail_threads;

		if (wl->io_type == WILC_HIF_SDIO &&
		    wl->hif_func->enable_interrupt(wl)) {
			PRINT_ER(dev, "couldn't initialize IRQ\n");
			ret = -EIO;
			goto fail_irq_init;
		}

		ret = wilc_wlan_get_firmware(dev);
		if (ret) {
			PRINT_ER(dev, "Can't get firmware\n");
			goto fail_irq_enable;
		}

		ret = wilc_firmware_download(dev);
		if (ret) {
			PRINT_ER(dev, "Failed to download firmware\n");
			goto fail_irq_enable;
		}

		ret = wilc_start_firmware(dev);
		if (ret) {
			PRINT_ER(dev, "Failed to start firmware\n");
			goto fail_irq_enable;
		}

		if (wilc_wlan_cfg_get(vif, 1, WID_FIRMWARE_VERSION, 1, 0)) {
			int size;
			char firmware_ver[50];

			size = wilc_wlan_cfg_get_val(wl, WID_FIRMWARE_VERSION,
						     firmware_ver,
						     sizeof(firmware_ver));
			firmware_ver[size] = '\0';
			PRINT_INFO(dev, INIT_DBG, "WILC Firmware Ver = %s\n",
				   firmware_ver);
		}

		ret = wilc_init_fw_config(dev, vif);
		if (ret < 0) {
			netdev_err(dev, "Failed to configure firmware\n");
			ret = -EIO;
			goto fail_fw_start;
		}

		wl->initialized = true;
		return 0;

fail_fw_start:
		wilc_wlan_stop(wl, vif);

fail_irq_enable:
		if (wl->io_type == WILC_HIF_SDIO)
			wl->hif_func->disable_interrupt(wl);
fail_irq_init:
		deinit_irq(dev);

fail_threads:
		wlan_deinitialize_threads(dev);
fail_wilc_wlan:
		wilc_wlan_cleanup(dev);
		netdev_err(dev, "WLAN initialization FAILED\n");
	} else {
		PRINT_WRN(vif->ndev, INIT_DBG, "wilc already initialized\n");
	}
	return ret;
}

static int mac_init_fn(struct net_device *ndev)
{
	netif_start_queue(ndev);
	netif_stop_queue(ndev);

	return 0;
}

static int wilc_mac_open(struct net_device *ndev)
{
	struct wilc_vif *vif = netdev_priv(ndev);
	struct wilc *wl = vif->wilc;
	int ret = 0;
#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE
	struct mgmt_frame_regs mgmt_regs = {};
#endif

	if (!wl || !wl->dev) {
		netdev_err(ndev, "device not ready\n");
		return -ENODEV;
	}

	PRINT_INFO(ndev, INIT_DBG, "MAC OPEN[%p] %s\n", ndev, ndev->name);

	if (wl->open_ifcs == 0)
		wilc_bt_power_up(wl, DEV_WIFI);

	if (!recovery_on) {
		ret = wilc_init_host_int(ndev);
		if (ret < 0) {
			PRINT_ER(ndev, "Failed to initialize host interface\n");
			return ret;
		}
	}

	PRINT_INFO(vif->ndev, INIT_DBG, "*** re-init ***\n");
	ret = wilc_wlan_initialize(ndev, vif);
	if (ret) {
		PRINT_ER(ndev, "Failed to initialize wilc\n");
		if (!recovery_on)
			wilc_deinit_host_int(ndev);
		return ret;
	}

	wait_for_recovery = 0;
	wilc_set_operation_mode(vif, wilc_get_vif_idx(vif), vif->iftype,
				vif->idx);

	if (is_valid_ether_addr(ndev->dev_addr))
		wilc_set_mac_address(vif, ndev->dev_addr);
	else
		wilc_get_mac_address(vif, ndev->dev_addr);
	netdev_dbg(ndev, "Mac address: %pM\n", ndev->dev_addr);

	if (!is_valid_ether_addr(ndev->dev_addr)) {
		PRINT_ER(ndev, "Wrong MAC address\n");
		wilc_deinit_host_int(ndev);
		wilc_wlan_deinitialize(ndev);
		return -EINVAL;
	}

#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE
	mgmt_regs.interface_stypes = vif->mgmt_reg_stypes;
	/* so we detect a change */
	vif->mgmt_reg_stypes = 0;

	wilc_update_mgmt_frame_registrations(vif->ndev->ieee80211_ptr->wiphy,
					     vif->ndev->ieee80211_ptr,
					     &mgmt_regs);
#else
	wilc_mgmt_frame_register(vif->ndev->ieee80211_ptr->wiphy,
				 vif->ndev->ieee80211_ptr,
				 vif->frame_reg[0].type,
				 vif->frame_reg[0].reg);
	wilc_mgmt_frame_register(vif->ndev->ieee80211_ptr->wiphy,
				 vif->ndev->ieee80211_ptr,
				 vif->frame_reg[1].type,
				 vif->frame_reg[1].reg);
#endif
	netif_wake_queue(ndev);
	wl->open_ifcs++;
	vif->mac_opened = 1;
	return 0;
}

static struct net_device_stats *mac_stats(struct net_device *dev)
{
	struct wilc_vif *vif = netdev_priv(dev);

	return &vif->netstats;
}

static int wilc_set_mac_addr(struct net_device *dev, void *p)
{
	int result;
	struct wilc_vif *vif = netdev_priv(dev);
	struct wilc *wilc = vif->wilc;
	struct sockaddr *addr = (struct sockaddr *)p;
	unsigned char mac_addr[ETH_ALEN];
	struct wilc_vif *tmp_vif;
	int srcu_idx;

	if (!is_valid_ether_addr(addr->sa_data)) {
		PRINT_INFO(vif->ndev, INIT_DBG, "Invalid MAC address\n");
		return -EADDRNOTAVAIL;
	}

	if (!vif->mac_opened) {
		eth_commit_mac_addr_change(dev, p);
		return 0;
	}

	/* Verify MAC Address is not already in use: */
	srcu_idx = srcu_read_lock(&wilc->srcu);
	list_for_each_entry_rcu(tmp_vif, &wilc->vif_list, list) {
		wilc_get_mac_address(tmp_vif, mac_addr);
		if (ether_addr_equal(addr->sa_data, mac_addr)) {
			if (vif != tmp_vif) {
				PRINT_INFO(vif->ndev, INIT_DBG,
					   "MAC address is already in use\n");
				srcu_read_unlock(&wilc->srcu, srcu_idx);
				return -EADDRNOTAVAIL;
			}
			srcu_read_unlock(&wilc->srcu, srcu_idx);
			return 0;
		}
	}
	srcu_read_unlock(&wilc->srcu, srcu_idx);

	/* configure new MAC address */
	result = wilc_set_mac_address(vif, (u8 *)addr->sa_data);
	if (result)
		return result;

	eth_commit_mac_addr_change(dev, p);
	return result;
}

static void wilc_set_multicast_list(struct net_device *dev)
{
	struct netdev_hw_addr *ha;
	struct wilc_vif *vif = netdev_priv(dev);
	int i;
	u8 *mc_list;
	u8 *cur_mc;

	PRINT_INFO(vif->ndev, INIT_DBG,
		   "Setting mcast List with count = %d.\n", dev->mc.count);
	if (dev->flags & IFF_PROMISC) {
		PRINT_INFO(vif->ndev, INIT_DBG,
			   "Set promiscuous mode ON retrieve all pkts\n");
		return;
	}

	if (dev->flags & IFF_ALLMULTI ||
	    dev->mc.count > WILC_MULTICAST_TABLE_SIZE) {
		PRINT_INFO(vif->ndev, INIT_DBG,
			   "Disable mcast filter retrieve multicast pkts\n");
		wilc_setup_multicast_filter(vif, 0, 0, NULL);
		return;
	}

	if (dev->mc.count == 0) {
		PRINT_INFO(vif->ndev, INIT_DBG,
			   "Enable mcast filter retrieve directed pkts only\n");
		wilc_setup_multicast_filter(vif, 1, 0, NULL);
		return;
	}

	mc_list = kmalloc_array(dev->mc.count, ETH_ALEN, GFP_ATOMIC);
	if (!mc_list)
		return;

	cur_mc = mc_list;
	i = 0;
	netdev_for_each_mc_addr(ha, dev) {
		memcpy(cur_mc, ha->addr, ETH_ALEN);
		netdev_dbg(dev, "Entry[%d]: %pM\n", i, cur_mc);
		i++;
		cur_mc += ETH_ALEN;
	}

	if (wilc_setup_multicast_filter(vif, 1, dev->mc.count, mc_list))
		kfree(mc_list);
}

static void wilc_tx_complete(void *priv, int status)
{
	struct tx_complete_data *pv_data = priv;

	if (status == 1)
		PRINT_INFO(pv_data->vif->ndev, TX_DBG,
			   "Packet sentSize= %d Add= %p SKB= %p\n",
			   pv_data->size, pv_data->buff, pv_data->skb);
	else
		PRINT_INFO(pv_data->vif->ndev, TX_DBG,
			   "Couldn't send pkt Size= %d Add= %p SKB= %p\n",
			   pv_data->size, pv_data->buff, pv_data->skb);
	dev_kfree_skb(pv_data->skb);
	kfree(pv_data);
}

netdev_tx_t wilc_mac_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	struct wilc_vif *vif = netdev_priv(ndev);
	struct wilc *wilc = vif->wilc;
	struct tx_complete_data *tx_data = NULL;
	int queue_count;

	PRINT_INFO(vif->ndev, TX_DBG,
		   "Sending packet just received from TCP/IP\n");
	if (skb->dev != ndev) {
		netdev_err(ndev, "Packet not destined to this device\n");
		return NETDEV_TX_OK;
	}

	tx_data = kmalloc(sizeof(*tx_data), GFP_ATOMIC);
	if (!tx_data) {
		PRINT_ER(ndev, "Failed to alloc memory for tx_data struct\n");
		dev_kfree_skb(skb);
		netif_wake_queue(ndev);
		return NETDEV_TX_OK;
	}

	tx_data->buff = skb->data;
	tx_data->size = skb->len;
	tx_data->skb  = skb;

	PRINT_D(vif->ndev, TX_DBG, "Sending pkt Size= %d Add= %p SKB= %p\n",
		tx_data->size, tx_data->buff, tx_data->skb);
	PRINT_D(vif->ndev, TX_DBG, "Adding tx pkt to TX Queue\n");
	vif->netstats.tx_packets++;
	vif->netstats.tx_bytes += tx_data->size;
	tx_data->vif = vif;
	queue_count = wilc_wlan_txq_add_net_pkt(ndev, tx_data,
						tx_data->buff, tx_data->size,
						wilc_tx_complete);

	if (queue_count > FLOW_CONTROL_UPPER_THRESHOLD) {
		int srcu_idx;
		struct wilc_vif *vif;

		srcu_idx = srcu_read_lock(&wilc->srcu);
		list_for_each_entry_rcu(vif, &wilc->vif_list, list) {
			if (vif->mac_opened)
				netif_stop_queue(vif->ndev);
		}
		srcu_read_unlock(&wilc->srcu, srcu_idx);
	}

	return NETDEV_TX_OK;
}

static int wilc_mac_close(struct net_device *ndev)
{
	struct wilc_vif *vif = netdev_priv(ndev);
	struct wilc *wl = vif->wilc;

	PRINT_INFO(vif->ndev, GENERIC_DBG, "Mac close\n");

	if (wl->open_ifcs > 0) {
		wl->open_ifcs--;
	} else {
		PRINT_ER(ndev, "MAC close called with no opened interfaces\n");
		return 0;
	}

	if (vif->ndev) {
		netif_stop_queue(vif->ndev);

	handle_connect_cancel(vif);

	if (!recovery_on)
		wilc_deinit_host_int(vif->ndev);
	}

	if (wl->open_ifcs == 0) {
		netdev_dbg(ndev, "Deinitializing wilc\n");
		wl->close = 1;
		wilc_wlan_deinitialize(ndev);
	}

	vif->mac_opened = 0;

	return 0;
}

void wilc_frmw_to_host(struct wilc_vif *vif, u8 *buff, u32 size,
		       u32 pkt_offset, u8 status)
{
	unsigned int frame_len = 0;
	int stats;
	unsigned char *buff_to_send = NULL;
	struct sk_buff *skb;
	struct wilc_priv *priv;
	u8 null_bssid[ETH_ALEN] = {0};

	buff += pkt_offset;
	priv = &vif->priv;

	if (size == 0) {
		PRINT_ER(vif->ndev,
			 "Discard sending packet with len = %d\n", size);
		return;
	}

	frame_len = size;
	buff_to_send = buff;

	if (status == PKT_STATUS_NEW && buff_to_send[12] == 0x88 &&
	    buff_to_send[13] == 0x8e &&
	    (vif->iftype == WILC_STATION_MODE ||
	     vif->iftype == WILC_CLIENT_MODE) &&
	    ether_addr_equal_unaligned(priv->associated_bss, null_bssid)) {
		if (!priv->buffered_eap) {
			priv->buffered_eap = kmalloc(sizeof(struct
							    wilc_buffered_eap),
						     GFP_ATOMIC);
			if (priv->buffered_eap) {
				priv->buffered_eap->buff = NULL;
				priv->buffered_eap->size = 0;
				priv->buffered_eap->pkt_offset = 0;
			} else {
				PRINT_ER(vif->ndev,
					 "failed to alloc buffered_eap\n");
				return;
			}
		} else {
			kfree(priv->buffered_eap->buff);
		}
		priv->buffered_eap->buff = kmalloc(size + pkt_offset,
						   GFP_ATOMIC);
		priv->buffered_eap->size = size;
		priv->buffered_eap->pkt_offset = pkt_offset;
		memcpy(priv->buffered_eap->buff, buff -
		       pkt_offset, size + pkt_offset);
	#if KERNEL_VERSION(4, 15, 0) > LINUX_VERSION_CODE
		priv->eap_buff_timer.data = (unsigned long)priv;
	#endif
		mod_timer(&priv->eap_buff_timer, (jiffies +
			  msecs_to_jiffies(10)));
		return;
	}
	skb = dev_alloc_skb(frame_len);
	if (!skb) {
		PRINT_ER(vif->ndev, "Low memory - packet dropped\n");
		return;
	}

	skb->dev = vif->ndev;
#if KERNEL_VERSION(4, 13, 0) <= LINUX_VERSION_CODE
	skb_put_data(skb, buff_to_send, frame_len);
#else
	memcpy(skb_put(skb, frame_len), buff_to_send, frame_len);
#endif

	skb->protocol = eth_type_trans(skb, vif->ndev);
	vif->netstats.rx_packets++;
	vif->netstats.rx_bytes += frame_len;
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	stats = netif_rx(skb);
	PRINT_D(vif->ndev, RX_DBG, "netif_rx ret value: %d\n", stats);
}

void wilc_wfi_mgmt_rx(struct wilc *wilc, u8 *buff, u32 size)
{
	int srcu_idx;
	struct wilc_vif *vif;

	srcu_idx = srcu_read_lock(&wilc->srcu);
	list_for_each_entry_rcu(vif, &wilc->vif_list, list) {
		u16 type = le16_to_cpup((__le16 *)buff);
		struct wilc_priv *priv;
#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE
		u32 type_bit = BIT(type >> 4);
#endif

		priv = &vif->priv;
#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE
		if (vif->mgmt_reg_stypes & type_bit &&
		    vif->p2p_listen_state)
			wilc_wfi_p2p_rx(vif, buff, size);
#else
		if (((type == vif->frame_reg[0].type && vif->frame_reg[0].reg) ||
		    (type == vif->frame_reg[1].type && vif->frame_reg[1].reg)) &&
			    vif->p2p_listen_state)
			wilc_wfi_p2p_rx(vif, buff, size);
#endif

		if (vif->monitor_flag)
			wilc_wfi_monitor_rx(wilc->monitor_dev, buff, size);
	}
	srcu_read_unlock(&wilc->srcu, srcu_idx);
}

static const struct net_device_ops wilc_netdev_ops = {
	.ndo_init = mac_init_fn,
	.ndo_open = wilc_mac_open,
	.ndo_stop = wilc_mac_close,
	.ndo_set_mac_address = wilc_set_mac_addr,
	.ndo_start_xmit = wilc_mac_xmit,
	.ndo_get_stats = mac_stats,
	.ndo_set_rx_mode  = wilc_set_multicast_list,
};

void wilc_netdev_cleanup(struct wilc *wilc)
{
	struct wilc_vif *vif;
	int srcu_idx, ifc_cnt = 0;

	if (!wilc)
		return;

	if (wilc->firmware) {
		release_firmware(wilc->firmware);
		wilc->firmware = NULL;
	}

	srcu_idx = srcu_read_lock(&wilc->srcu);
	list_for_each_entry_rcu(vif, &wilc->vif_list, list) {
		/* clear the mode */
		wilc_set_operation_mode(vif, 0, 0, 0);
		if (vif->ndev) {
			PRINT_INFO(vif->ndev, INIT_DBG,
				   "Unregistering netdev %p\n",
				   vif->ndev);
			unregister_netdev(vif->ndev);
		}
	}
	srcu_read_unlock(&wilc->srcu, srcu_idx);

	wilc_wfi_deinit_mon_interface(wilc, false);

	flush_workqueue(wilc->hif_workqueue);
	destroy_workqueue(wilc->hif_workqueue);
	wilc->hif_workqueue = NULL;
	/* update the list */
	while (ifc_cnt < WILC_NUM_CONCURRENT_IFC) {
		mutex_lock(&wilc->vif_mutex);
		if (wilc->vif_num <= 0) {
			mutex_unlock(&wilc->vif_mutex);
			break;
		}
		vif = wilc_get_wl_to_vif(wilc);
		if (!IS_ERR(vif))
			list_del_rcu(&vif->list);
		wilc->vif_num--;
		mutex_unlock(&wilc->vif_mutex);
		synchronize_srcu(&wilc->srcu);
		ifc_cnt++;
	}

	wilc_wlan_cfg_deinit(wilc);
#ifdef WILC_DEBUGFS
	wilc_debugfs_remove();
#endif
	wilc_sysfs_exit();
	wlan_deinit_locks(wilc);
	kfree(wilc->bus_data);
	wiphy_unregister(wilc->wiphy);
	pr_info("Freeing wiphy\n");
	wiphy_free(wilc->wiphy);
	pr_info("Module_exit Done.\n");
}

static u8 wilc_get_available_idx(struct wilc *wl)
{
	int idx = 0;
	struct wilc_vif *vif;
	int srcu_idx;

	srcu_idx = srcu_read_lock(&wl->srcu);
	list_for_each_entry_rcu(vif, &wl->vif_list, list) {
		if (vif->idx == 0)
			idx = 1;
		else
			idx = 0;
	}
	srcu_read_unlock(&wl->srcu, srcu_idx);
	return idx;
}

struct wilc_vif *wilc_netdev_ifc_init(struct wilc *wl, const char *name,
				      int vif_type, enum nl80211_iftype type,
				      bool rtnl_locked)
{
	struct net_device *ndev;
	struct wilc_vif *vif;
	int ret;

	ndev = alloc_etherdev(sizeof(*vif));
	if (!ndev)
		return ERR_PTR(-ENOMEM);

	vif = netdev_priv(ndev);
	ndev->ieee80211_ptr = &vif->priv.wdev;
	strcpy(ndev->name, name);
	vif->wilc = wl;
	vif->ndev = ndev;
	ndev->ml_priv = vif;

	ndev->netdev_ops = &wilc_netdev_ops;

	SET_NETDEV_DEV(ndev, wiphy_dev(wl->wiphy));

	vif->ndev->ml_priv = vif;
	vif->priv.wdev.wiphy = wl->wiphy;
	vif->priv.wdev.netdev = ndev;
	vif->priv.wdev.iftype = type;
	vif->priv.dev = ndev;

	vif->priv.dev = ndev;
	if (rtnl_locked)
		ret = register_netdevice(ndev);
	else
		ret = register_netdev(ndev);

	if (ret) {
		pr_err("Device couldn't be registered - %s\n", ndev->name);
		free_netdev(ndev);
		return ERR_PTR(-EFAULT);
	}
#if KERNEL_VERSION(4, 11, 9) <= LINUX_VERSION_CODE
	ndev->needs_free_netdev = true;
#else
	ndev->destructor = free_netdev;
#endif
	vif->iftype = vif_type;
	vif->idx = wilc_get_available_idx(wl);
	vif->mac_opened = 0;
	mutex_lock(&wl->vif_mutex);
	list_add_tail_rcu(&vif->list, &wl->vif_list);
	wl->vif_num += 1;
	mutex_unlock(&wl->vif_mutex);
	synchronize_srcu(&wl->srcu);

	return vif;
}

MODULE_LICENSE("GPL");
MODULE_FIRMWARE(WILC1000_FW(WILC1000_API_VER));
MODULE_FIRMWARE(WILC3000_FW(WILC3000_API_VER));
