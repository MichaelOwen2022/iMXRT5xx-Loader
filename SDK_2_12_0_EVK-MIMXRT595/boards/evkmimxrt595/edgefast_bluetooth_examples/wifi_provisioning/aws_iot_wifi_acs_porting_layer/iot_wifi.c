/*
 * Amazon FreeRTOS Wi-Fi
 *
 *  Copyright 2019-2021 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */

/**
 * @file iot_wifi.c
 * @brief Wi-Fi Interface.
 */

/* Socket and Wi-Fi interface includes. */
#include "FreeRTOS.h"
#include "list.h"
#include "iot_wifi.h"

/* Wi-Fi configuration includes. */
#include "aws_wifi_config.h"

#include "wlan.h"
#include "wmlog.h"
#include "wm_net.h"
#if defined(SD8801)
#include "sd8801_wlan.h"
#elif defined(SD8977)
#include "sduart8977_wlan_bt.h"
#elif defined(SD8978)
#include "sduartIW416_wlan_bt.h"
#elif defined(SD8987)
#include "sduart8987_wlan_bt.h"
#elif defined(SD8997)
#include "sduart8997_wlan_bt.h"
#elif defined(SD9097)
#include "pvt_sd9097_wlan.h"
#elif defined(SD9098)
#include "pvt_sd9098_wlan.h"
#endif

#include "fsl_debug_console.h"

#if (defined(IOT_WIFI_ENABLE_SAVE_NETWORK) && (IOT_WIFI_ENABLE_SAVE_NETWORK > 0))
#include "lfs.h"
#include "littlefs_pl.h"
#endif

#ifndef IOT_WIFI_MAX_SAVED_NETWORKS
#define IOT_WIFI_MAX_SAVED_NETWORKS 8
#endif

#define wm_wlan_e(...)                             \
    wmlog_e("wm_wlan", ##__VA_ARGS__)
#define wm_wlan_w(...)                             \
    wmlog_w("wm_wlan", ##__VA_ARGS__)
#define wm_wlan_d(...)                             \
    wmlog("wm_wlan", ##__VA_ARGS__)

//#define wmprintf(...) wmlog("wm_wlan", ##__VA_ARGS__)
#define wmprintf vLoggingPrintf

static os_semaphore_t scan_protection_sem;

static os_semaphore_t wlan_init_sem;
static os_mutex_t wlan_mtx;
static WIFIEventHandler_t xWifiEventHandlers[ eWiFiEventMax ];

#if (defined(IOT_WIFI_ENABLE_SAVE_NETWORK) && (IOT_WIFI_ENABLE_SAVE_NETWORK > 0))
static WIFINetworkProfile_t NvmSavedWifiNetworkProfile[IOT_WIFI_MAX_SAVED_NETWORKS];
static lfs_t * lfs;
static lfs_file_t lfs_write_file;
static lfs_file_t lfs_read_file;
static char lfs_file_name[16];
static struct lfs_info lfs_file_info;
#define LFS_FILE_NAME "wifint"
#endif /* IOT_WIFI_ENABLE_SAVE_NETWORK */

static int wlan_event_callback(enum wlan_event_reason event, void *data)
{
    int ret;
    struct wlan_ip_config addr;
    char ip[16];

    /* After successful wlan subsytem initialization */
    if (event == WLAN_REASON_INITIALIZED) {
        wmprintf("WLAN_REASON_INITIALIZED\r\n");

        ret = os_mutex_create(&wlan_mtx, "wlanmtx",
                OS_MUTEX_INHERIT);
        if (ret != WM_SUCCESS) {
            return eWiFiFailure;
        }
        os_semaphore_put(&wlan_init_sem);
    }

    if (event == WLAN_REASON_SUCCESS) {
        ret = wlan_get_address(&addr);
        if (ret != WM_SUCCESS) {
            wmprintf("failed to get IP address\r\n");
            return 0;
        }

        net_inet_ntoa(addr.ipv4.address, ip);

        wmprintf("Connected to with IP = [%s]\r\n", ip);
    }

    if (event == WLAN_REASON_USER_DISCONNECT) {
	    wmprintf("Dis-connected\r\n");
    }

    return 0;
}

/*-----------------------------------------------------------*/
WIFIReturnCode_t WIFI_On( void )
{
    int rv;
    static bool wifi_init_done = false;

    if(wifi_init_done)
	return eWiFiSuccess;

#if (defined(IOT_WIFI_ENABLE_SAVE_NETWORK) && (IOT_WIFI_ENABLE_SAVE_NETWORK > 0))
    if (NULL == lfs)
    {
        lfs = lfs_pl_init();
        if (NULL == lfs)
        {
            return eWiFiFailure;
        }
    }

    rv = lfs_stat(lfs, LFS_FILE_NAME, &lfs_file_info);
    if (rv < 0)
    {
        rv = lfs_mkdir(lfs, LFS_FILE_NAME);
        if (rv < 0)
        {
            return eWiFiFailure;
        }
    }


    for (uint8_t index = 0; index < IOT_WIFI_MAX_SAVED_NETWORKS; index ++ )
    {
        snprintf(lfs_file_name, sizeof(lfs_file_name) - 1, "%s/%d", LFS_FILE_NAME, index);
        rv = lfs_file_open (lfs, &lfs_read_file, (char *)lfs_file_name, LFS_O_RDONLY);
        if (rv >= 0)
        {
            rv = lfs_file_read (lfs, &lfs_read_file, &NvmSavedWifiNetworkProfile[index], sizeof(NvmSavedWifiNetworkProfile[index]));

            (void)lfs_file_close(lfs, &lfs_read_file);

            if (rv < 0)
            {
                memset(&NvmSavedWifiNetworkProfile[index], 0, sizeof(NvmSavedWifiNetworkProfile[index]));
            }
        }
    }
#endif

    rv = os_semaphore_create(&wlan_init_sem, "wlansem");
    if (rv != WM_SUCCESS) {
            return eWiFiFailure;
    }

    /* Consume so that 'get' blocks when used later */
    os_semaphore_get(&wlan_init_sem, OS_WAIT_FOREVER);

    rv = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
    if (rv != WM_SUCCESS) {
        os_semaphore_delete(&wlan_init_sem);
        return eWiFiFailure;
    }

    rv = wlan_start(wlan_event_callback);
    if (rv != WM_SUCCESS) {
        os_semaphore_delete(&wlan_init_sem);
        return eWiFiFailure;
    }

    /* Wait till app framework is initialized */
    rv = os_semaphore_get(&wlan_init_sem, OS_WAIT_FOREVER);
    if (rv != WM_SUCCESS) {
        os_semaphore_delete(&wlan_init_sem);
        return eWiFiFailure;
    }

    os_semaphore_delete(&wlan_init_sem);

    wifi_init_done = true;

    /* For Test Only */
#if 0
    WIFINetworkProfile_t xNetworkProfile = {0};
    WIFIReturnCode_t xWiFiStatus;
    uint16_t usIndex;
    strncpy( (char *)xNetworkProfile.ucSSID, "test", strlen("test"));
    xNetworkProfile.ucSSIDLength = strlen("test");
    strncpy( (char *)xNetworkProfile.cPassword, "12348765", strlen("12348765"));
    xNetworkProfile.ucPasswordLength = strlen("12348765");
    xNetworkProfile.xSecurity = eWiFiSecurityWPA2;
    WIFI_NetworkAdd( &xNetworkProfile, &usIndex );
#endif

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Off( void )
{
    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

#define IS_SSID_LEN_VALID(x) \
        ((x) <=  wificonfigMAX_SSID_LEN && (x) > 0)
#define IS_PASSPHRASE_LEN_VALID(x) \
        ((x) <= wificonfigMAX_PASSPHRASE_LEN && (x) > 0)

WIFIReturnCode_t WIFI_ConnectAP( const WIFINetworkParams_t * const pxNetworkParams )
{
    int ret = eWiFiFailure;
    struct wlan_network home_nw;
    enum wlan_connection_state state;
    WIFIEvent_t eventInfo;

    int retry_cnt = 3;
    int connect_retry = 10;

    if (os_mutex_get(&wlan_mtx, OS_WAIT_FOREVER) != WM_SUCCESS)
           return eWiFiFailure;

    if(pxNetworkParams == NULL)
        goto fail;

    if (pxNetworkParams->ucSSID == NULL || pxNetworkParams->xSecurity ==
            eWiFiSecurityNotSupported) {
        goto fail;
    }

    if (!IS_SSID_LEN_VALID(pxNetworkParams->ucSSIDLength) ||
            (pxNetworkParams->xSecurity != eWiFiSecurityOpen &&
            !IS_PASSPHRASE_LEN_VALID(pxNetworkParams->xPassword.xWPA.ucLength))) {
	goto fail;
    }

    if (wlan_get_connection_state(&state)) {
		wmprintf("Error: unable to get "
				"connection state\r\n");
	goto fail;
    }

    if (state == WLAN_CONNECTED) {
        int connect_retry = 200;
        ret = wlan_disconnect();
        if (ret != WM_SUCCESS)
            goto fail;

        do {
            /* wait for interface up */
            os_thread_sleep(os_msec_to_ticks(50));

            if (wlan_get_connection_state(&state)) {
            wmprintf("Error: unable to get "
                    "connection state\r\n");
                continue;
            }

            if (state == WLAN_DISCONNECTED || connect_retry == 0)
                break;
        } while (connect_retry--);

        if (state != WLAN_DISCONNECTED) {
            wmprintf("Error: Not connected.\r\n");
            goto fail;
        }
    }

    memset(&home_nw, 0, sizeof(home_nw));
    strncpy(home_nw.name, "aws_network", WLAN_NETWORK_NAME_MAX_LENGTH);

    /* Set SSID as passed by the user */
    memcpy(home_nw.ssid, pxNetworkParams->ucSSID, pxNetworkParams->ucSSIDLength);

    if(pxNetworkParams->xPassword.xWPA.ucLength > 0) {
        /* Set the passphrase */
        memcpy(home_nw.security.psk, pxNetworkParams->xPassword.xWPA.cPassphrase, pxNetworkParams->xPassword.xWPA.ucLength);

        /* Set the passphrase length */
        home_nw.security.psk_len = pxNetworkParams->xPassword.xWPA.ucLength;
    }

    //home_nw.channel = pxNetworkParams->ucChannel;

    if (pxNetworkParams->xSecurity == eWiFiSecurityOpen)
        home_nw.security.type = WLAN_SECURITY_NONE;
    else if (pxNetworkParams->xSecurity == eWiFiSecurityWEP)
        home_nw.security.type = WLAN_SECURITY_WEP_OPEN;
    else if (pxNetworkParams->xSecurity == eWiFiSecurityWPA)
        home_nw.security.type = WLAN_SECURITY_WPA;
    else if (pxNetworkParams->xSecurity == eWiFiSecurityWPA2)
        home_nw.security.type = WLAN_SECURITY_WPA2;

    home_nw.type = WLAN_BSS_TYPE_STA;
    home_nw.role = WLAN_BSS_ROLE_STA;
    home_nw.ip.ipv4.addr_type = ADDR_TYPE_DHCP;

    ret = wlan_remove_network("aws_network");
    if (ret != WM_SUCCESS && ret != -WM_E_INVAL) {
        wmprintf(" Failed to add network %d\r\n", ret);
        goto fail;
    }

    ret = wlan_add_network(&home_nw);
    if (ret != WM_SUCCESS) {
        wmprintf(" Failed to add network %d\r\n", ret);
        goto fail;
    }


    do {
        wmprintf("Connecting to %s .....\r\n", home_nw.ssid);

        ret = wlan_connect(home_nw.name);

        if (ret != 0) {
            wmprintf("Failed to connect %d\r\n", ret);
            goto retry;
        }

        connect_retry = 2000;

        do {
            /* wait for interface up */
            os_thread_sleep(os_msec_to_ticks(50));

            if (wlan_get_connection_state(&state)) {
                wmprintf("Error: unable to get "
                        "connection state\r\n");
                continue;
            }

            if (state == WLAN_CONNECTED || connect_retry == 0)
                break;
        } while (connect_retry--);

        if (state != WLAN_CONNECTED) {
            wmprintf("Error: Not connected.\r\n");
            goto retry;
        }

        if (os_mutex_put(&wlan_mtx) != WM_SUCCESS)
            return eWiFiFailure;

        if (NULL != xWifiEventHandlers[eWiFiEventIPReady])
        {
            struct wlan_network network;
            if (wlan_get_current_network(&network) == 0)
            {
                eventInfo.xInfo.xIPReady.xIPAddress.ulAddress[0] = network.ip.ipv4.address;
            }
            else
            {
                eventInfo.xInfo.xIPReady.xIPAddress.ulAddress[0] = home_nw.ip.ipv4.address;
            }
            eventInfo.xEventType = eWiFiEventIPReady;
            xWifiEventHandlers[eWiFiEventIPReady](&eventInfo);
        }
        return eWiFiSuccess;
retry:
        retry_cnt--;
        wmprintf("Connect to AP FAIL ! Retrying ..... \r\n");

    } while (retry_cnt != 0);

    if (retry_cnt == 0) {
        os_thread_sleep(os_msec_to_ticks(500));

	wmprintf("Connection Failed !\r\n");

	ret = wlan_disconnect();
	if (ret != 0) {
	    wmprintf("Failed to disconnect \r\n");
	    if (os_mutex_put(&wlan_mtx) != WM_SUCCESS)
                return eWiFiFailure;
        return eWiFiFailure;
        }
        if (os_mutex_put(&wlan_mtx) != WM_SUCCESS)
            return eWiFiFailure;
        return eWiFiFailure;
    }

fail:

    if (os_mutex_put(&wlan_mtx) != WM_SUCCESS)
        return eWiFiFailure;

    if (ret != 0)
    {
        return eWiFiFailure;
    }
    else
    {
        return eWiFiSuccess;
    }
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Disconnect( void )
{
    int ret = eWiFiFailure;
    enum wlan_connection_state state;
    int connect_retry = 200;
    WIFIEvent_t eventInfo;

    if (os_mutex_get(&wlan_mtx, OS_WAIT_FOREVER) != WM_SUCCESS)
           return eWiFiFailure;

    ret = wlan_disconnect();
    if (ret != WM_SUCCESS)
	    goto fail;

    do {
        /* wait for interface up */
        os_thread_sleep(os_msec_to_ticks(50));

        if (wlan_get_connection_state(&state)) {
            wmprintf("Error: unable to get "
                "connection state\r\n");
            continue;
        }

        if (state == WLAN_DISCONNECTED || connect_retry == 0)
            break;
    } while (connect_retry--);

    if (state != WLAN_DISCONNECTED) {
        wmprintf("Error: Not connected.\r\n");
        goto fail;
    }

    if (NULL != xWifiEventHandlers[eWiFiEventDisconnected])
    {
        eventInfo.xEventType = eWiFiEventDisconnected;
        eventInfo.xInfo.xDisconnected.xReason = eWiFiReasonUnspecified;
        xWifiEventHandlers[eWiFiEventDisconnected](&eventInfo);
    }
fail:

    if (os_mutex_put(&wlan_mtx) != WM_SUCCESS)
           return eWiFiFailure;

    if (ret != 0)
    {
        return eWiFiFailure;
    }
    else
    {
        return eWiFiSuccess;
    }
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Reset( void )
{
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

//#define CONFIG_MAX_AP_ENTRIES 20
#define MAX_SITES CONFIG_MAX_AP_ENTRIES

struct site_survey {
    struct wlan_scan_result sites[MAX_SITES];
    unsigned int num_sites;
    os_mutex_t lock;
} survey;

static int prov_handle_scan_results(unsigned int count)
{
    int i, j = 0;

    /* WLAN firmware does not provide Noise Floor (NF) value with
     * every scan result. We call the function below to update a
     * global NF value so that /sys/scan can provide the latest NF value */
    wlan_get_current_nf();

    /* lock the scan results */
    if (os_mutex_get(&survey.lock, OS_WAIT_FOREVER) != WM_SUCCESS)
        return 0;

    /* clear out and refresh the site survey table */
    memset(survey.sites, 0, sizeof(survey.sites));

    /*
     * Loop through till we have results to process or
     * run out of space in survey.sites
     */
    for (i = 0; i < count && j < MAX_SITES; i++) {
        if (wlan_get_scan_result(i, &survey.sites[j]))
            break;

        if (survey.sites[j].ssid[0] != 0)
            j++;
    }
    survey.num_sites = j;

    /* unlock the site survey table */
    os_mutex_put(&survey.lock);
    /* Give up the semaphore to process next scan/prov_stop request */
    os_semaphore_put(&scan_protection_sem);
    return 0;
}

#define DEFAULT_SCAN_PROTECTION_TIMEOUT 1000

WIFIReturnCode_t WIFI_Scan( WIFIScanResult_t * pxBuffer,
                            uint8_t ucNumNetworks )
{
    int ret = 0;
    int i = 0;

    if (os_mutex_get(&wlan_mtx, OS_WAIT_FOREVER) != WM_SUCCESS)
           return eWiFiFailure;

    if (pxBuffer == NULL) {
           os_mutex_put(&wlan_mtx);
           return eWiFiFailure;
    }

    memset(pxBuffer, 0, sizeof(WIFIScanResult_t) * ucNumNetworks);

    ret = os_semaphore_create(&scan_protection_sem, "scanprotsem");
    if (ret != WM_SUCCESS) {
        os_mutex_put(&wlan_mtx);
        return eWiFiFailure;
    }

    ret = os_mutex_create(&survey.lock, "survey-mtx",
			OS_MUTEX_INHERIT);
    if (ret != WM_SUCCESS) {
        os_semaphore_delete(&scan_protection_sem);
        os_mutex_put(&wlan_mtx);
        return eWiFiFailure;
    }

    ret = os_semaphore_get(&scan_protection_sem, os_msec_to_ticks(
            DEFAULT_SCAN_PROTECTION_TIMEOUT * 1000));

    if (ret != WM_SUCCESS) {
        wm_wlan_e("Scan protection timeout occurred."
                   " Failed to get the scan protection for scan start"
                   " semaphore", ret);
        os_mutex_delete(&survey.lock);
        os_semaphore_delete(&scan_protection_sem);
        os_mutex_put(&wlan_mtx);
        return eWiFiFailure;
    }

    ret = wlan_scan(prov_handle_scan_results);
    if (ret != WM_SUCCESS)
    {
        os_mutex_delete(&survey.lock);
        os_semaphore_delete(&scan_protection_sem);
        os_mutex_put(&wlan_mtx);
        return eWiFiFailure;
    }

    ret = os_semaphore_get(&scan_protection_sem, os_msec_to_ticks(
            DEFAULT_SCAN_PROTECTION_TIMEOUT * 1000));
    if (ret != WM_SUCCESS) {
        wm_wlan_e("Scan protection timeout occurred."
                   " Failed to get the scan protection for scan result"
                   " semaphore", ret);
        os_mutex_delete(&survey.lock);
        os_semaphore_delete(&scan_protection_sem);
        os_mutex_put(&wlan_mtx);
        return eWiFiFailure;
    }

    if(survey.num_sites < ucNumNetworks ) {
        ucNumNetworks = survey.num_sites;
    }

    for(i = 0; i < ucNumNetworks; i++) {
        pxBuffer[i].ucChannel = survey.sites[i].channel;
        pxBuffer[i].cRSSI = survey.sites[i].rssi;

        if (survey.sites[i].rssi < 128)
        {
            pxBuffer[i].cRSSI = pxBuffer[i].cRSSI * (-1);
        }

        if (survey.sites[i].ssid_len) {
            strncpy((char *)pxBuffer[i].ucSSID, survey.sites[i].ssid, survey.sites[i].ssid_len);
        }
        pxBuffer[i].ucSSIDLength = survey.sites[i].ssid_len;

        memcpy((char *)pxBuffer[i].ucBSSID, survey.sites[i].bssid, wificonfigMAX_BSSID_LEN);

        if(survey.sites[i].wep) {
             pxBuffer[i].xSecurity = eWiFiSecurityWEP;
        }
        else if (survey.sites[i].wpa) {
             pxBuffer[i].xSecurity = eWiFiSecurityWPA;
        }
        else if (survey.sites[i].wpa2) {
             pxBuffer[i].xSecurity = eWiFiSecurityWPA2;
        }
        else if (survey.sites[i].wpa2_entp || survey.sites[i].wpa3_sae) {
             pxBuffer[i].xSecurity = eWiFiSecurityNotSupported;
        }
        else {
             pxBuffer[i].xSecurity = eWiFiSecurityOpen;
        }
    }

    os_mutex_delete(&survey.lock);
    os_semaphore_delete(&scan_protection_sem);

    if (os_mutex_put(&wlan_mtx) != WM_SUCCESS)
        return eWiFiFailure;

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetMode( WIFIDeviceMode_t xDeviceMode )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMode( WIFIDeviceMode_t * pxDeviceMode )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkAdd( const WIFINetworkProfile_t * const pxNetworkProfile,
                                  uint16_t * pusIndex )
{
    /* FIX ME. */
#if (defined(IOT_WIFI_ENABLE_SAVE_NETWORK) && (IOT_WIFI_ENABLE_SAVE_NETWORK > 0))
    int rv;
    WIFIReturnCode_t ret = eWiFiSuccess;
    uint8_t savedIndex = IOT_WIFI_MAX_SAVED_NETWORKS;

    if ((NULL == pusIndex) || (NULL == pxNetworkProfile))
    {
        ret = eWiFiFailure;
    }

    if (eWiFiSuccess == ret)
    {
        for (uint8_t index = 0; index < IOT_WIFI_MAX_SAVED_NETWORKS; index ++ )
        {
            if (0 == strlen((const char *)NvmSavedWifiNetworkProfile[index].ucSSID))
            {
                savedIndex = index;
                break;
            }
        }
        if (IOT_WIFI_MAX_SAVED_NETWORKS == savedIndex)
        {
            savedIndex = 0;
        }

        memcpy(&NvmSavedWifiNetworkProfile[savedIndex], pxNetworkProfile, sizeof(NvmSavedWifiNetworkProfile[savedIndex]));
        *pusIndex = savedIndex;

        snprintf(lfs_file_name, sizeof(lfs_file_name) - 1, "%s/%d", LFS_FILE_NAME, savedIndex);
        rv = lfs_file_open (lfs, &lfs_write_file, (char *)lfs_file_name, LFS_O_WRONLY | LFS_O_CREAT);
        if (rv >= 0)
        {
            rv = lfs_file_write (lfs, &lfs_write_file, &NvmSavedWifiNetworkProfile[savedIndex], sizeof(NvmSavedWifiNetworkProfile[savedIndex]));

            (void)lfs_file_close(lfs, &lfs_write_file);
            (void)rv;
        }
    }
    return ret;
#else
    return eWiFiNotSupported;
#endif
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkGet( WIFINetworkProfile_t * pxNetworkProfile,
                                  uint16_t usIndex )
{
    /* FIX ME. */
#if (defined(IOT_WIFI_ENABLE_SAVE_NETWORK) && (IOT_WIFI_ENABLE_SAVE_NETWORK > 0))
    WIFIReturnCode_t ret = eWiFiSuccess;

    if ((NULL == pxNetworkProfile))
    {
        ret = eWiFiFailure;
    }

    if ((eWiFiSuccess == ret) && (usIndex >= IOT_WIFI_MAX_SAVED_NETWORKS))
    {
        ret = eWiFiNotSupported;
    }

    if (eWiFiSuccess == ret)
    {
        if (0 == strlen((const char *)NvmSavedWifiNetworkProfile[usIndex].ucSSID))
        {
            ret = eWiFiNotSupported;
        }
        else
        {
            memcpy(pxNetworkProfile, &NvmSavedWifiNetworkProfile[usIndex], sizeof(*pxNetworkProfile));
        }
    }
    return ret;
#else
    return eWiFiNotSupported;
#endif
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkDelete( uint16_t usIndex )
{
    /* FIX ME. */
#if (defined(IOT_WIFI_ENABLE_SAVE_NETWORK) && (IOT_WIFI_ENABLE_SAVE_NETWORK > 0))
    int rv;
    WIFIReturnCode_t ret = eWiFiSuccess;

    if ((eWiFiSuccess == ret) && (usIndex >= IOT_WIFI_MAX_SAVED_NETWORKS))
    {
        ret = eWiFiNotSupported;
    }

    if (eWiFiSuccess == ret)
    {
        memset(&NvmSavedWifiNetworkProfile[usIndex], 0, sizeof(NvmSavedWifiNetworkProfile[usIndex]));
        snprintf(lfs_file_name, sizeof(lfs_file_name) - 1, "%s/%d", LFS_FILE_NAME, usIndex);
        rv = lfs_file_open (lfs, &lfs_write_file, (char *)lfs_file_name, LFS_O_WRONLY | LFS_O_CREAT);
        if (rv >= 0)
        {
            rv = lfs_file_write (lfs, &lfs_write_file, &NvmSavedWifiNetworkProfile[usIndex], sizeof(NvmSavedWifiNetworkProfile[usIndex]));

            (void)lfs_file_close(lfs, &lfs_write_file);
            (void)rv;
        }
    }
    return ret;
#else
    return eWiFiNotSupported;
#endif
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_Ping( uint8_t * pucIPAddr,
                            uint16_t usCount,
                            uint32_t ulIntervalMS )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetIP( uint8_t * pucIPAddr )
{
    struct wlan_ip_config ip_addr;
    int ret;

    if (pucIPAddr == NULL) {
        return eWiFiFailure;
    }

    ret = wlan_get_address(&ip_addr);

    if (ret != WM_SUCCESS) {
        wm_wlan_e("app_network_config: failed to get IP address");
        memset(pucIPAddr, 0, 4);
        return eWiFiFailure;
     }

    memcpy(pucIPAddr, &(ip_addr.ipv4.address), 4);

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetMAC( uint8_t * pucMac )
{
    if (pucMac == NULL) {
        return eWiFiFailure;
    }

    if (WM_SUCCESS != wlan_get_mac_address(pucMac)) {
        return eWiFiFailure;
    }

    return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetHostIP( char * pcHost,
                                 uint8_t * pucIPAddr )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StartAP( void )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StopAP( void )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_ConfigureAP( const WIFINetworkParams_t * const pxNetworkParams )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetPMMode( WIFIPMMode_t xPMModeType,
                                 const void * pvOptionValue )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetPMMode( WIFIPMMode_t * pxPMModeType,
                                 void * pvOptionValue )
{
    /* FIX ME. */
    return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

BaseType_t WIFI_IsConnected( const WIFINetworkParams_t * pxNetworkParams )
{
    if(is_sta_connected())
        return pdTRUE;

    return pdFALSE;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_RegisterEvent( WIFIEventType_t xEventType,
                                     WIFIEventHandler_t xHandler )
{
    WIFIReturnCode_t xWiFiRet;

    switch( xEventType )
    {
        case eWiFiEventIPReady:
        case eWiFiEventDisconnected:
            xWifiEventHandlers[ xEventType ] = xHandler;
            xWiFiRet = eWiFiSuccess;
            break;
        default:
            xWiFiRet = eWiFiNotSupported;
            break;
    }


    return xWiFiRet;
}
