/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2012 - 2018 Microchip Technology Inc., and its subsidiaries.
 * All rights reserved.
 */

#ifndef WILC_NETDEV_H
#define WILC_NETDEV_H

#include <linux/tcp.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>
#include <net/ieee80211_radiotap.h>
#include <linux/if_arp.h>
#include <linux/gpio/consumer.h>

#include "hif.h"
#include "wlan.h"
#include "wlan_cfg.h"

extern int wait_for_recovery;

#if KERNEL_VERSION(3, 14, 0) > LINUX_VERSION_CODE
static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	*(u32 *)dst = *(const u32 *)src;
	*(u16 *)(dst + 4) = *(const u16 *)(src + 4);
#else
	u16 *a = (u16 *)dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
#endif
}

static inline bool ether_addr_equal_unaligned(const u8 *addr1, const u8 *addr2)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	return ether_addr_equal(addr1, addr2);
#else
	return memcmp(addr1, addr2, ETH_ALEN) == 0;
#endif
}
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0) */

#if KERNEL_VERSION(3, 12, 0) > LINUX_VERSION_CODE
#define PTR_ERR_OR_ZERO(ptr) PTR_RET(ptr)
#endif

#if KERNEL_VERSION(3, 13, 0) > LINUX_VERSION_CODE
/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
	(((~UL(0)) - (UL(1) << (l)) + 1) & \
	 (~UL(0) >> (BITS_PER_LONG - 1 - (h))))
#endif

#if KERNEL_VERSION(4, 9, 0) > LINUX_VERSION_CODE
#ifdef __CHECKER__
#define __BUILD_BUG_ON_NOT_POWER_OF_2(n) (0)
#else
/* Force a compilation error if a constant expression is not a power of 2 */
#define __BUILD_BUG_ON_NOT_POWER_OF_2(n)	\
	BUILD_BUG_ON(((n) & ((n) - 1)) != 0)
#endif

/*
 * Bitfield access macros
 *
 * FIELD_{GET,PREP} macros take as first parameter shifted mask
 * from which they extract the base mask and shift amount.
 * Mask must be a compilation time constant.
 *
 * Example:
 *
 *  #define REG_FIELD_A  GENMASK(6, 0)
 *  #define REG_FIELD_B  BIT(7)
 *  #define REG_FIELD_C  GENMASK(15, 8)
 *  #define REG_FIELD_D  GENMASK(31, 16)
 *
 * Get:
 *  a = FIELD_GET(REG_FIELD_A, reg);
 *  b = FIELD_GET(REG_FIELD_B, reg);
 *
 * Set:
 *  reg = FIELD_PREP(REG_FIELD_A, 1) |
 *	  FIELD_PREP(REG_FIELD_B, 0) |
 *	  FIELD_PREP(REG_FIELD_C, c) |
 *	  FIELD_PREP(REG_FIELD_D, 0x40);
 *
 * Modify:
 *  reg &= ~REG_FIELD_C;
 *  reg |= FIELD_PREP(REG_FIELD_C, c);
 */

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

#define __BF_FIELD_CHECK(_mask, _reg, _val, _pfx)			\
	({								\
		BUILD_BUG_ON_MSG(!__builtin_constant_p(_mask),		\
				 _pfx "mask is not constant");		\
		BUILD_BUG_ON_MSG(!(_mask), _pfx "mask is zero");	\
		BUILD_BUG_ON_MSG(__builtin_constant_p(_val) ?		\
				 ~((_mask) >> __bf_shf(_mask)) & (_val) : 0, \
				 _pfx "value too large for the field"); \
		BUILD_BUG_ON_MSG((_mask) > (typeof(_reg))~0ull,		\
				 _pfx "type of reg too small for mask"); \
		__BUILD_BUG_ON_NOT_POWER_OF_2((_mask) +			\
					      (1ULL << __bf_shf(_mask))); \
	})
/**
 * FIELD_GET() - extract a bitfield element
 * @_mask: shifted mask defining the field's length and position
 * @_reg:  32bit value of entire bitfield
 *
 * FIELD_GET() extracts the field specified by @_mask from the
 * bitfield passed in as @_reg by masking and shifting it down.
 */
#define FIELD_GET(_mask, _reg)						\
	({								\
		__BF_FIELD_CHECK(_mask, _reg, 0U, "FIELD_GET: ");	\
		(typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask));	\
	})

/**
 * FIELD_PREP() - prepare a bitfield element
 * @_mask: shifted mask defining the field's length and position
 * @_val:  value to put in the field
 *
 * FIELD_PREP() masks and shifts up the value.  The result should
 * be combined with other fields of the bitfield using logical OR.
 */
#define FIELD_PREP(_mask, _val)						\
	({								\
		__BF_FIELD_CHECK(_mask, 0ULL, _val, "FIELD_PREP: ");	\
		((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask);	\
	})
#endif

#define FLOW_CONTROL_LOWER_THRESHOLD		128
#define FLOW_CONTROL_UPPER_THRESHOLD		256

#define PMKID_FOUND				1
#define NUM_STA_ASSOCIATED			8

#define TCP_ACK_FILTER_LINK_SPEED_THRESH	54
#define DEFAULT_LINK_SPEED			72

#define ANT_SWTCH_INVALID_GPIO_CTRL		0
#define ANT_SWTCH_SNGL_GPIO_CTRL		1
#define ANT_SWTCH_DUAL_GPIO_CTRL		2

struct wilc_wfi_stats {
	unsigned long rx_packets;
	unsigned long tx_packets;
	unsigned long rx_bytes;
	unsigned long tx_bytes;
	u64 rx_time;
	u64 tx_time;

};

struct wilc_wfi_key {
	u8 *key;
	u8 *seq;
	int key_len;
	int seq_len;
	u32 cipher;
};

struct wilc_wfi_wep_key {
	u8 *key;
	u8 key_len;
	u8 key_idx;
};

struct sta_info {
	u8 sta_associated_bss[WILC_MAX_NUM_STA][ETH_ALEN];
};

/* Parameters needed for host interface for remaining on channel */
struct wilc_wfi_p2p_listen_params {
	struct ieee80211_channel *listen_ch;
	u32 listen_duration;
	u64 listen_cookie;
};

/* Struct to buffer eapol 1/4 frame */
struct wilc_buffered_eap {
	unsigned int size;
	unsigned int pkt_offset;
	u8 *buff;
};

struct wilc_p2p_var {
	u8 local_random;
	u8 recv_random;
	bool is_wilc_ie;
};

static const u32 wilc_cipher_suites[] = {
	WLAN_CIPHER_SUITE_WEP40,
	WLAN_CIPHER_SUITE_WEP104,
	WLAN_CIPHER_SUITE_TKIP,
	WLAN_CIPHER_SUITE_CCMP,
	WLAN_CIPHER_SUITE_AES_CMAC
};

#if KERNEL_VERSION(4, 7, 0) > LINUX_VERSION_CODE
#define CHAN2G(_channel, _freq, _flags) {       \
	.band             = IEEE80211_BAND_2GHZ, \
	.center_freq      = (_freq),             \
	.hw_value         = (_channel),          \
	.flags            = (_flags),            \
	.max_antenna_gain = 0,                   \
	.max_power        = 30,                  \
}
#else
#define CHAN2G(_channel, _freq, _flags) {       \
	.band             = NL80211_BAND_2GHZ, \
	.center_freq      = (_freq),             \
	.hw_value         = (_channel),          \
	.flags            = (_flags),            \
	.max_antenna_gain = 0,                   \
	.max_power        = 30,                  \
}
#endif

static const struct ieee80211_channel wilc_2ghz_channels[] = {
	CHAN2G(1,  2412, 0),
	CHAN2G(2,  2417, 0),
	CHAN2G(3,  2422, 0),
	CHAN2G(4,  2427, 0),
	CHAN2G(5,  2432, 0),
	CHAN2G(6,  2437, 0),
	CHAN2G(7,  2442, 0),
	CHAN2G(8,  2447, 0),
	CHAN2G(9,  2452, 0),
	CHAN2G(10, 2457, 0),
	CHAN2G(11, 2462, 0),
	CHAN2G(12, 2467, 0),
	CHAN2G(13, 2472, 0),
	CHAN2G(14, 2484, 0)
};

#define RATETAB_ENT(_rate, _hw_value, _flags) {	\
	.bitrate  = (_rate),			\
	.hw_value = (_hw_value),		\
	.flags    = (_flags),			\
}

static struct ieee80211_rate wilc_bitrates[] = {
	RATETAB_ENT(10,  0,  0),
	RATETAB_ENT(20,  1,  0),
	RATETAB_ENT(55,  2,  0),
	RATETAB_ENT(110, 3,  0),
	RATETAB_ENT(60,  9,  0),
	RATETAB_ENT(90,  6,  0),
	RATETAB_ENT(120, 7,  0),
	RATETAB_ENT(180, 8,  0),
	RATETAB_ENT(240, 9,  0),
	RATETAB_ENT(360, 10, 0),
	RATETAB_ENT(480, 11, 0),
	RATETAB_ENT(540, 12, 0)
};

struct wilc_priv {
	struct wireless_dev wdev;
	struct cfg80211_scan_request *scan_req;

	struct wilc_wfi_p2p_listen_params remain_on_ch_params;
	u64 tx_cookie;

	bool cfg_scanning;

	u8 associated_bss[ETH_ALEN];
	struct sta_info assoc_stainfo;
	struct sk_buff *skb;
	struct net_device *dev;
	struct host_if_drv *hif_drv;
	struct wilc_pmkid_attr pmkid_list;
	u8 wep_key[4][WLAN_KEY_LEN_WEP104];
	u8 wep_key_len[4];

	/* The real interface that the monitor is on */
	struct net_device *real_ndev;
	struct wilc_wfi_key *wilc_gtk[WILC_MAX_NUM_STA];
	struct wilc_wfi_key *wilc_ptk[WILC_MAX_NUM_STA];
	u8 wilc_groupkey;

	/* mutexes */
	struct mutex scan_req_lock;

	struct wilc_buffered_eap *buffered_eap;

	struct timer_list eap_buff_timer;
	int scanned_cnt;
	struct wilc_p2p_var p2p;
	u64 inc_roc_cookie;
};

#if KERNEL_VERSION(5, 8, 0) > LINUX_VERSION_CODE
struct frame_reg {
	u16 type;
	bool reg;
};

#define NUM_REG_FRAME				2
#endif

#define MAX_TCP_SESSION                25
#define MAX_PENDING_ACKS               256

struct ack_session_info {
	u32 seq_num;
	u32 bigger_ack_num;
	u16 src_port;
	u16 dst_port;
	u16 status;
};

struct pending_acks {
	u32 ack_num;
	u32 session_index;
	struct txq_entry_t  *txqe;
};

struct tcp_ack_filter {
	struct ack_session_info ack_session_info[2 * MAX_TCP_SESSION];
	struct pending_acks pending_acks[MAX_PENDING_ACKS];
	u32 pending_base;
	u32 tcp_session;
	u32 pending_acks_idx;
	bool enabled;
};

#define WILC_P2P_ROLE_GO	1
#define WILC_P2P_ROLE_CLIENT	0

struct sysfs_attr_group {
	bool p2p_mode;
	u8 ant_swtch_mode;
	u8 antenna1;
	u8 antenna2;
};

struct wilc_vif {
	u8 idx;
	u8 iftype;
	int monitor_flag;
	int mac_opened;
#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE
	u32 mgmt_reg_stypes;
#else
	struct frame_reg frame_reg[NUM_REG_FRAME];
#endif
	struct net_device_stats netstats;
	struct wilc *wilc;
	u8 bssid[ETH_ALEN];
	struct host_if_drv *hif_drv;
	struct net_device *ndev;

	struct timer_list periodic_rssi;
	struct rf_info periodic_stat;
	struct tcp_ack_filter ack_filter;
	bool connecting;
	struct wilc_priv priv;
	struct list_head list;
	u8 restart;
	bool p2p_listen_state;
	struct cfg80211_bss *bss;
};

struct wilc_power_gpios {
	int reset;
	int chip_en;
};

struct wilc_power {
	struct wilc_power_gpios gpios;
	u8 status[DEV_MAX];
};

struct wilc_tx_queue_status {
	u8 buffer[AC_BUFFER_SIZE];
	u16 end_index;
	u16 cnt[NQUEUES];
	u16 sum;
	bool initialized;
};

struct wilc {
	struct wiphy *wiphy;
	const struct wilc_hif_func *hif_func;
	u32 chipid;
	int io_type;
	s8 mac_status;
	struct clk *rtc_clk;
	bool initialized;
	int dev_irq_num;
	int close;
	u8 vif_num;
	struct list_head vif_list;

	/* protect vif list */
	struct mutex vif_mutex;
	struct srcu_struct srcu;
	u8 open_ifcs;

	/* protect head of transmit queue */
	struct mutex txq_add_to_head_cs;

	/* protect txq_entry_t transmit queue */
	spinlock_t txq_spinlock;

	/* protect rxq_entry_t receiver queue */
	struct mutex rxq_cs;

	/* lock to protect hif access */
	struct mutex hif_cs;

	struct completion cfg_event;
	struct completion sync_event;
	struct completion txq_event;
	struct completion txq_thread_started;
	struct completion debug_thread_started;
	struct task_struct *txq_thread;
	struct task_struct *debug_thread;

	int quit;

	/* lock to protect issue of wid command to firmware */
	struct mutex cfg_cmd_lock;
	struct wilc_cfg_frame cfg_frame;
	u32 cfg_frame_offset;
	u8 cfg_seq_no;

	u8 *rx_buffer;
	u32 rx_buffer_offset;
	u8 *tx_buffer;

	struct txq_handle txq[NQUEUES];
	int txq_entries;

	struct wilc_tx_queue_status tx_q_limit;
	struct rxq_entry_t rxq_head;

	const struct firmware *firmware;

	struct device *dev;
	struct device *dt_dev;

	enum wilc_chip_type chip;
	struct wilc_power power;
	uint8_t keep_awake[DEV_MAX];
	struct mutex cs;
	struct workqueue_struct *hif_workqueue;
	struct wilc_cfg cfg;
	void *bus_data;
	struct net_device *monitor_dev;

	/* deinit lock */
	struct mutex deinit_lock;
	u8 sta_ch;
	u8 op_ch;
	struct sysfs_attr_group attr_sysfs;
	struct ieee80211_channel channels[ARRAY_SIZE(wilc_2ghz_channels)];
	struct ieee80211_rate bitrates[ARRAY_SIZE(wilc_bitrates)];
	struct ieee80211_supported_band band;
	u32 cipher_suites[ARRAY_SIZE(wilc_cipher_suites)];
};

struct wilc_wfi_mon_priv {
	struct net_device *real_ndev;
};

void wilc_frmw_to_host(struct wilc_vif *vif, u8 *buff, u32 size,
		       u32 pkt_offset, u8 status);
void wilc_mac_indicate(struct wilc *wilc);
void wilc_netdev_cleanup(struct wilc *wilc);
void wilc_wfi_mgmt_rx(struct wilc *wilc, u8 *buff, u32 size);
void wilc_wlan_set_bssid(struct net_device *wilc_netdev, u8 *bssid, u8 mode);
struct wilc_vif *wilc_netdev_ifc_init(struct wilc *wl, const char *name,
				      int vif_type, enum nl80211_iftype type,
				      bool rtnl_locked);
int wilc_bt_power_up(struct wilc *wilc, int source);
int wilc_bt_power_down(struct wilc *wilc, int source);

#endif
