//------------------------------------------------------------------------------
// <copyright file="wlan_node.c" company="Atheros">
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
//    The software is licensed “as-is.” 
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

#include <athdefs.h>
#include <athtypes.h>
#include <osapi.h>
#include <ieee80211.h>
#include <wlan_api.h>
#include <ieee80211_node.h>
#include <htc.h>
#include <wmi.h>
#include <wmi_api.h>

static void wlan_node_timeout(void *arg);
static bss_t * _ieee80211_find_node(struct ieee80211_node_table *nt,
                                    const A_UINT8 *macaddr);

bss_t *
wlan_node_alloc(struct ieee80211_node_table *nt, A_INT32 wh_size)
{
    bss_t *ni;

    ni = A_MALLOC(sizeof(bss_t));

    if (ni != NULL) {
        ni->ni_buf = A_MALLOC(wh_size);
        if (ni->ni_buf == NULL) {
            A_FREE(ni);
            ni = NULL;
        }
    }
	/* Make sure our lists are clean */

    if (ni) {
        ni->ni_list_next = NULL;
        ni->ni_list_prev = NULL;
        ni->ni_hash_next = NULL;
        ni->ni_hash_prev = NULL;
    }

    return ni;
}

void
wlan_node_free(bss_t *ni)
{
    if (ni->ni_buf != NULL) {
        A_FREE(ni->ni_buf);
    }

    A_FREE(ni);
}

void
wlan_setup_node(struct ieee80211_node_table *nt, bss_t *ni,
                const A_UINT8 *macaddr)
{
    A_INT32 hash;

    A_MEMCPY(ni->ni_macaddr, macaddr, IEEE80211_ADDR_LEN);
    hash = IEEE80211_NODE_HASH(macaddr);
    ieee80211_node_initref(ni);     /* mark referenced */

    ni->ni_tstamp = A_MS_TICKGET() + WLAN_NODE_INACT_TIMEOUT_MSEC;
    IEEE80211_NODE_LOCK(nt);

    /* Insert at the end of the node list */
    ni->ni_list_next = NULL;
    ni->ni_list_prev = nt->nt_node_last;
    if(nt->nt_node_last != NULL)
    {
        nt->nt_node_last->ni_list_next = ni;
    }
    nt->nt_node_last = ni;
    if(nt->nt_node_first == NULL)
    {
        nt->nt_node_first = ni;
    }
 
    /* Insert into the hash list i.e. the bucket */
    if((ni->ni_hash_next = nt->nt_hash[hash]) != NULL)
    {
        nt->nt_hash[hash]->ni_hash_prev = ni;
    }
    ni->ni_hash_prev = NULL;
    nt->nt_hash[hash] = ni;

    IEEE80211_NODE_UNLOCK(nt);
}

static bss_t *
_ieee80211_find_node(struct ieee80211_node_table *nt,
    const A_UINT8 *macaddr)
{
    bss_t *ni;
    A_INT32 hash;

    IEEE80211_NODE_LOCK_ASSERT(nt);

    hash = IEEE80211_NODE_HASH(macaddr);
    for(ni = nt->nt_hash[hash]; ni; ni = ni->ni_hash_next) {
        if (IEEE80211_ADDR_EQ(ni->ni_macaddr, macaddr)) {
            ieee80211_node_incref(ni);  /* mark referenced */
            return ni;
        }
    }
    return NULL;
}

bss_t *
wlan_find_node(struct ieee80211_node_table *nt, const A_UINT8 *macaddr)
{
    bss_t *ni;

    IEEE80211_NODE_LOCK(nt);
    ni = _ieee80211_find_node(nt, macaddr);
    IEEE80211_NODE_UNLOCK(nt);
    return ni;
}

static void
wlan_node_dec_free(bss_t *ni)
{
    ieee80211_node_decref(ni);
    if (ieee80211_node_refcnt(ni) == 0) {
        wlan_node_free(ni);
    }
}

/*
 * Reclaim a node.  If this is the last reference count then
 * do the normal free work.  Otherwise remove it from the node
 * table and mark it gone by clearing the back-reference.
 */
void
wlan_node_reclaim(struct ieee80211_node_table *nt, bss_t *ni)
{
    if(ni->ni_list_prev == NULL)
    {
        /* First in list so fix the list head */
        nt->nt_node_first = ni->ni_list_next;
    }
    else
    {
        ni->ni_list_prev->ni_list_next = ni->ni_list_next;
    }

    if(ni->ni_list_next == NULL)
    {
        /* Last in list so fix list tail */
        nt->nt_node_last = ni->ni_list_prev;
    }
    else
    {
        ni->ni_list_next->ni_list_prev = ni->ni_list_prev;
    }
 
    if(ni->ni_hash_prev == NULL)
    {
        /* First in list so fix the list head */
        int hash;
        hash = IEEE80211_NODE_HASH(ni->ni_macaddr);
        nt->nt_hash[hash] = ni->ni_hash_next;
    }
    else
    {
        ni->ni_hash_prev->ni_hash_next = ni->ni_hash_next;
    }

    if(ni->ni_hash_next != NULL)
    {
        ni->ni_hash_next->ni_hash_prev = ni->ni_hash_prev;
    }

    wlan_node_dec_free(ni);
}

void
wlan_node_ret_n_remove(struct ieee80211_node_table *nt, bss_t *ni)
{
    IEEE80211_NODE_LOCK(nt);

    ieee80211_node_decref(ni);
    if (ieee80211_node_refcnt(ni) == 0) {
        wlan_node_free(ni);
    } else {
        wlan_node_reclaim(nt, ni);
    }

    IEEE80211_NODE_UNLOCK(nt);
}

void
wlan_free_allnodes(struct ieee80211_node_table *nt)
{
    bss_t *ni;

    IEEE80211_NODE_LOCK(nt);
    while ((ni = nt->nt_node_first) != NULL) {
        wlan_node_reclaim(nt, ni);
    }
    IEEE80211_NODE_UNLOCK(nt);
}

void
wlan_iterate_nodes(struct ieee80211_node_table *nt, wlan_node_iter_func *f,
                   void *arg)
{
    bss_t *ni;
    A_UINT32 gen;

    gen = nt->nt_scangen++;

    IEEE80211_NODE_LOCK(nt);

    for (ni = nt->nt_node_first; ni; ni = ni->ni_list_next) {
        if (ni->ni_scangen != gen) {
            ni->ni_scangen = gen;
            (*f)(arg, ni);
        }
    }
    IEEE80211_NODE_UNLOCK(nt);
}

/*
 * Node table support.
 */
void
wlan_node_table_init(void *wmip, struct ieee80211_node_table *nt)
{
	int i;
    WLAN_DEBUG_PRINTF(ATH_LOG_INF,"node table = 0x%x\n", (A_UINT32)nt);
    IEEE80211_NODE_LOCK_INIT(nt);
    nt->nt_node_first = nt->nt_node_last = NULL;
    for(i = 0; i < IEEE80211_NODE_HASHSIZE; i++)
    {
        nt->nt_hash[i] = NULL;
    }
    A_INIT_TIMER(&nt->nt_inact_timer, wlan_node_timeout, nt);
    A_TIMEOUT_MS(&nt->nt_inact_timer, WLAN_NODE_INACT_TIMEOUT_MSEC, 0);
    nt->nt_wmip = wmip;
}

static void
wlan_node_timeout(void *arg)
{
    struct ieee80211_node_table *nt = (struct ieee80211_node_table *)arg;
    bss_t *bss, *nextBss;
    A_UINT8 myBssid[IEEE80211_ADDR_LEN];

    wmi_get_current_bssid(nt->nt_wmip, myBssid);

    IEEE80211_NODE_LOCK(nt);
    bss = nt->nt_node_first;
    while (bss != NULL) {
        nextBss = bss->ni_list_next;
        if ((A_MEMCMP(myBssid, bss->ni_macaddr, sizeof(myBssid)) != 0) &&
            (bss->ni_tstamp <= A_MS_TICKGET()))
        {
            /*
             * free up all but the current bss - if set
             */
            wlan_node_reclaim(nt, bss);
        }
        bss = nextBss;
    }

    IEEE80211_NODE_UNLOCK(nt);
    A_TIMEOUT_MS(&nt->nt_inact_timer, WLAN_NODE_INACT_TIMEOUT_MSEC, 0);
}

void
wlan_node_table_cleanup(struct ieee80211_node_table *nt)
{
    A_UNTIMEOUT(&nt->nt_inact_timer);
    wlan_free_allnodes(nt);
    IEEE80211_NODE_LOCK_DESTROY(nt);
}
