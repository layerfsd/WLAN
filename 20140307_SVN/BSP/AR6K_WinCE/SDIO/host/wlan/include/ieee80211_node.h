//------------------------------------------------------------------------------
// <copyright file="ieee80211_node.h" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// 
//
// Author(s): ="Atheros"
//=============================================================================
#ifndef _IEEE80211_NODE_H_
#define _IEEE80211_NODE_H_

/*
 * Node locking definitions.
 */
#define IEEE80211_NODE_LOCK_INIT(_nt)   A_MUTEX_INIT(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_LOCK_DESTROY(_nt) A_MUTEX_DESTROY(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_LOCK(_nt)        A_MUTEX_LOCK(&(_nt)->nt_nodelock)
#define IEEE80211_NODE_UNLOCK(_nt)      A_MUTEX_UNLOCK(&(_nt)->nt_nodelock)
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
#define ieee80211_node_dectestref(_ni)  (((_ni)->ni_refcnt--) == 0)
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
    struct bss             *nt_node_first;  /* information of all nodes */
	struct bss             *nt_node_last;   /* information of all nodes */
    struct bss             *nt_hash[IEEE80211_NODE_HASHSIZE];
    const char              *nt_name;   /* for debugging */
    A_UINT32                nt_scangen; /* gen# for timeout scan */
    A_TIMER                 nt_inact_timer;
};

#define WLAN_NODE_INACT_TIMEOUT_MSEC            10000

#endif /* _IEEE80211_NODE_H_ */
