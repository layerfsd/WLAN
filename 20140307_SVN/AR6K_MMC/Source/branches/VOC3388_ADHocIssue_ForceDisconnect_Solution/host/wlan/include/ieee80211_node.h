//------------------------------------------------------------------------------
// <copyright file="ieee80211_node.h" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifndef _IEEE80211_NODE_H_
#define _IEEE80211_NODE_H_

/*
 * Node locking definitions.
 */

#ifdef REXOS
#define IEEE80211_NODE_LOCK_INIT(_nt)   A_MUTEX_INIT_REX(&(_nt)->nt_nodelock)

#define IEEE80211_NODE_LOCK(_nt)        A_MUTEX_LOCK_REX(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_UNLOCK(_nt)      A_MUTEX_UNLOCK_REX(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_LOCK_BH(_nt)     A_MUTEX_LOCK_REX(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_UNLOCK_BH(_nt)   A_MUTEX_UNLOCK_REX(&(_nt)->nt_nodelock)

#else

#define IEEE80211_NODE_LOCK_INIT(_nt)   A_MUTEX_INIT(&(_nt)->nt_nodelock)

#define IEEE80211_NODE_LOCK(_nt)        A_MUTEX_LOCK(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_UNLOCK(_nt)      A_MUTEX_UNLOCK(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_LOCK_BH(_nt)     A_MUTEX_LOCK(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_UNLOCK_BH(_nt)   A_MUTEX_UNLOCK(&(_nt)->nt_nodelock)

#endif

#define IEEE80211_NODE_LOCK_DESTROY(_nt) if (A_IS_MUTEX_VALID(&(_nt)->nt_nodelock)) { \
                                               A_MUTEX_DELETE(&(_nt)->nt_nodelock); }
#define IEEE80211_NODE_LOCK_ASSERT(_nt)

/*
 * Node reference counting definitions.
 *
 * ieee80211_node_initref   initialize the reference count to 1
 * ieee80211_node_incref    add a reference
 * ieee80211_node_decref    remove a reference
 * ieee80211_node_dectestref    remove a reference and return 1 if this
 *              is the last reference, otherwise 0
 * ieee80211_node_refcnt    reference count for printing (only)
 */
#define ieee80211_node_initref(_ni)     ((_ni)->ni_refcnt = 1)
#define ieee80211_node_incref(_ni)      ((_ni)->ni_refcnt++)
#define ieee80211_node_decref(_ni)      ((_ni)->ni_refcnt--)
#define ieee80211_node_dectestref(_ni)  (((_ni)->ni_refcnt--) == 1)
#define ieee80211_node_refcnt(_ni)      ((_ni)->ni_refcnt)

#define IEEE80211_NODE_HASHSIZE 32
/* simple hash is enough for variation of macaddr */
#define IEEE80211_NODE_HASH(addr)   \
    (((const A_UINT8 *)(addr))[IEEE80211_ADDR_LEN - 1] % \
        IEEE80211_NODE_HASHSIZE)

/*
 * Table of ieee80211_node instances.  Each ieee80211com
 * has at least one for holding the scan candidates.
 * When operating as an access point or in ibss mode there
 * is a second table for associated stations or neighbors.
 */
struct ieee80211_node_table {
    void                   *nt_wmip;       /* back reference */
    A_MUTEX_T               nt_nodelock;    /* on node table */
    struct bss              *nt_node_first; /* information of all nodes */
    struct bss              *nt_node_last;  /* information of all nodes */
    struct bss              *nt_hash[IEEE80211_NODE_HASHSIZE];
    const char              *nt_name;   /* for debugging */
    A_UINT32                nt_scangen; /* gen# for timeout scan */
    A_UINT32                nt_nodeAge; /* node aging time */
#ifdef OS_ROAM_MANAGEMENT
    A_UINT32                nt_si_gen; /* gen# for scan indication*/
#endif
};

#define WLAN_NODE_INACT_TIMEOUT_MSEC            120000
#define WLAN_NODE_INACT_CNT            4

#endif /* _IEEE80211_NODE_H_ */
