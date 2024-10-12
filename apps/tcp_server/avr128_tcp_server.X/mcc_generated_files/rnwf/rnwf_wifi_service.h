/**
 * Wi-Fi service header file
 * 
 * @file rnwf_wifi_service.h
 * 
 * @defgroup wifi_service Wi-Fi Configuration Service
 * 
 * @brief This header file contains data types for Wifi service
 *
 * @version Driver Version 2.0.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_WIFI_SERVICE_H
#define	RNWF_WIFI_SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.  

/* RNWFxx Wi-Fi STA mode AT commands */

#define RNWF_WIFI_GET_STA_STATE     "AT+WSTA\r\n"
#define RNWF_WIFI_CONNECT           "AT+WSTA=1\r\n"
#define RNWF_WIFI_DISCONNECT        "AT+WSTA=0\r\n"
#define RNWF_WIFI_SET_STA_SSID      "AT+WSTAC=1,\"%s\"\r\n"
#define RNWF_WIFI_SET_STA_SEC       "AT+WSTAC=2,%d\r\n"
#define RNWF_WIFI_SET_STA_PWD       "AT+WSTAC=3,\"%s\"\r\n"
#define RNWF_WIFI_SET_STA_CHANNEL   "AT+WSTAC=4,%d\r\n"
#define RNWF_WIFI_SET_STA_BSSID     "AT+WSTAC=5,\"%s\"\r\n"
#define RNWF_WIFI_SET_STA_TIMEOUT   "AT+WSTAC=7,%d\r\n"

/* RNWFxx Wi-Fi AP mode AT commands */

#define RNWF_WIFI_GET_AP_STATE   "AT+WAP\r\n"
#define RNWF_WIFI_SOFTAP_ENABLE  "AT+WAP=1\r\n"
#define RNWF_WIFI_SOFTAP_DISABLE "AT+WAP=0\r\n"
#define RNWF_WIFI_SET_AP_SSID    "AT+WAPC=1,\"%s\"\r\n"
#define RNWF_WIFI_SET_AP_SEC     "AT+WAPC=2,%d\r\n"
#define RNWF_WIFI_SET_AP_PWD     "AT+WAPC=3,\"%s\"\r\n"
#define RNWF_WIFI_SET_AP_CHANNEL "AT+WAPC=4,%d\r\n"
#define RNWF_WIFI_SET_AP_HIDDEN  "AT+WAPC=5,%d\r\n"

/* RNWFxx Wi-Fi Scan AT commands*/

#define RNWF_WIFI_PSV_SCAN      "AT+WSCN=0\r\n"
#define RNWF_WIFI_ACT_SCAN      "AT+WSCN=1\r\n"

//--------------------------------------------------

/* Additional feature support for RNWF02 with firmware v2.0.0 */

/* Check available and configure regulatory domain */
#define RNWF_WIFI_GET_REGULATORY_DOMAIN     "AT+WIFIC=11\r\n"
#define RNWF_WIFI_SET_REGULATORY_DOMAIN     "AT+WIFIC=10,\"%s\"\r\n"

/* Wi-Fi Power Save Mode config */
#define RNWF_SET_WIFI_PWRSV       "AT+WIFIC=20,1\r\n"

/* Adds a support for Wi-Fi Ping test */
#define RNWF_SET_WIFI_PING_ON      "AT+PING=\"%s\"\r\n"

/* Wi-Fi BT Co-existence configurations */
#define RNWF_SET_WIFI_BT_COEX       "AT+WIFIC=30,%d\r\n"
#define RNWF_WIFI_BT_INTF_TYPE      "AT+WIFIC=31,%d\r\n"
#define RNWF_WIFI_BT_WLAN_RX_PRI    "AT+WIFIC=32,%d\r\n"
#define RNWF_WIFI_BT_WLAN_TX_PRI    "AT+WIFIC=33,%d\r\n"
#define RNWF_WIFI_BT_ANTENNA_MODE   "AT+WIFIC=34,%d\r\n"
 

#define RNWF_SSID_LEN_MAX           33
#define RNWF_BSSID_LEN_MAX          32
#define RNWF_PWD_LEN_MAX            128

#define RNWF_WIFI_SERVICE_CB_MAX    2

/**
 * @ingroup wifi_service
 * @brief Wi-Fi Service List
 * @enum    RNWF_WIFI_SERVICE_t
 */
typedef enum {
    RNWF_STA_CONNECT,               /**<Request/Trigger Wi-Fi connect */
    RNWF_STA_DISCONNECT,            /**<Request/Trigger Wi-Fi disconnect */  
    RNWF_AP_DISABLE,                /**<Request/Trigger SoftAP disable */ 
    RNWF_SET_WIFI_PARAMS,           /**<Configure the Wi-Fi parameters */   
/* Additional feature support for RNWF02 with firmware v2.0.0 */
    RNWF_GET_WIFI_REGDOM,           /**<Query the available Wi-Fi Regulatory Domain */
    RNWF_SET_WIFI_REGDOM,           /**<Configure the Wi-Fi Regulatory Domain */
    RNWF_WIFI_SET_COEX,             /**<Configure Wi-Fi Bluetooth Co-Ex */
    RNWF_SET_WIFI_PSM,              /**<Wi-Fi Power Save Mode ON */
    RNWF_WIFI_PING_ENBL,            /**<Enable Wi-Fi Ping support */
 
    RNWF_SET_WIFI_AP_CHANNEL,       /**<Configure the Wi-Fi channel in DUT's Access Point mode */
    RNWF_SET_WIFI_TIMEOUT,          /**<Configure Wi-Fi connection timeout */
    RNWF_SET_WIFI_BSSID,            /**<Configure the Access point's BSSID */
    RNWF_SET_WIFI_HIDDEN,           /**<Configure Hidden mode SSID in SoftAP mode*/
    RNWF_WIFI_PASSIVE_SCAN,         /**<Request/Trigger Wi-Fi passive scan */
    RNWF_WIFI_ACTIVE_SCAN,          /**<Request/Trigger Wi-Fi active scan */                      
    RNWF_WIFI_SET_CALLBACK,         /**<Regester the call back for async events */
    RNWF_WIFI_SET_SRVC_CALLBACK,    /**<Regester the call back for async events */
}RNWF_WIFI_SERVICE_t;

/**
 * @ingroup wifi_service
 * @brief   Wi-Fi Event Code list
 * @enum    RNWF_WIFI_EVENT_t
 */
typedef enum {
    RNWF_CONNECTED,                 /**<Wi-Fi connected event code*/
    RNWF_DISCONNECTED,              /**<Wi-Fi disconnected event code*/
    RNWF_CONNECT_FAILED,            /**<Wi-Fi connection failure event code*/
 
    RNWF_DHCP_IPV4_DONE,            /**<Wi-Fi DHCP IPv4 address received event code*/
    RNWF_DHCP_LINK_LOCAL_IPV6_DONE, /**<Wi-Fi DHCP IPv6 (Local) address received event code*/
    RNWF_DHCP_GLOBAL_IPV6_DONE,     /**<Wi-Fi DHCP IPv6 (Global) address received event code*/
    RNWF_SET_REGDOM,                /**<Wi-Fi Set regulatory domain event code*/
    RNWF_PING_ERROR,                /**<Wi-Fi Ping Error event code*/
    RNWF_PING_SUCCESS,              /**<Wi-Fi Ping Success event code*/
     
    RNWF_SCAN_INDICATION,           /**<Scan indication event to report each scan list */
    RNWF_SCAN_DONE,                 /**<Scan complete event code*/
    RNWF_SNTP_UP,                   /**<SNTP time synchronization complete event code*/
}RNWF_WIFI_EVENT_t;

/**
 * @ingroup wifi_service
 * @brief   Wi-Fi Security modes
 * @enum    RNWF_WIFI_SECURITY_t
 */
typedef enum {
    RNWF_OPEN,                      /**<OPEN mode, no security*/
    RNWF_RSVD,                  
    RNWF_WPA2_MIXED,                /**<WPA2 Mixed mode TKIP/CCMP*/
    RNWF_WPA2,                      /**<WPA2 CCMP*/
    RNWF_WPA3_TRANS,                /**<WPA3 Transition Mode*/
    RNWF_WPA3,                      /**<WPA3 mode*/
}RNWF_WIFI_SECURITY_t;

/**
 * @ingroup wifi_service
 * @brief   Wi-Fi operation modes
 * @enum    RNWF_WIFI_MODE_t
 */
typedef enum {
    RNWF_WIFI_MODE_STA,             /**<Station (STA) mode of WiFi operation*/
    RNWF_WIFI_MODE_AP,              /**<Software Access Point (SoftAP) mode of WiFi operation*/
}RNWF_WIFI_MODE_t;

/**
 * @ingroup wifi_service
 * @brief   Wi-Fi BT co-existence arbiter antenna modes
 * @enum    RNWF_COEX_ANTENNA_MODE_t
 */
/* feature addition for RNWF02 from FW v2.0.0*/
typedef enum {
    DEDICATED = 0,                  /**<Co-existence arbiter antenna mode - Dedicated Antenna */
    SHARED = 1,                     /**<Co-existence arbiter antenna mode - Shared Antenna */
}RNWF_COEX_ANTENNA_MODE_t;

/**
 * @ingroup wifi_service
 * @brief   Wi-Fi BT co-existence interface type
 * @enum    RNWF_COEX_INTF_TYPE_t
 */
/* feature addition for RNWF02 from FW v2.0.0*/
typedef enum {
THREE_WIRE = 0,                     /**<3-wire interface (BT_Act, BT_Prio, WLAN_Act) */
TWO_WIRE = 1,                       /**<2-wire interface (BT_Prio, WLAN_Act) */
}RNWF_COEX_INTF_TYPE_t;
 

/**
 * @ingroup     wifi_service
 * @brief       Wi-Fi Config struct
 * @struct      RNWF_WIFI_PARAM_t
 */
typedef struct {
    RNWF_WIFI_MODE_t mode;          /**<Wi-Fi operation mode of type RNWF_WIFI_MODE_t - either STA or SoftAP */
    const char  *ssid;              /**<Wi-Fi SSID of Home AP or SoftAP */
    const char  *passphrase;        /**<Wi-Fi Passphrase of Home AP or SoftAP*/
    RNWF_WIFI_SECURITY_t security;  /**<Wi-Fi Security of type RNWF_WIFI_SECURITY_t */
    uint8_t autoconnect;            /**<Wi-Fi autoconnect, SoftAP */
/* feature additions from FW v2.0.0*/
    uint8_t channelNo;              /**<Wi-Fi channel number for Home AP or SoftAP */
    uint32_t connTimeout;           /**<Wi-Fi STA mode - Connection timeout in milliseconds */
    const char* bssid;              /**<Wi-Fi STA mode - The BSSID of the network to connect to */
 
}RNWF_WIFI_PARAM_t;

/**
 * @ingroup     wifi_service
 * @brief       Wi-Fi-Bluetooth CoEx parameter configurations
 * @struct      RNWF_WIFI_BT_COEX_t
 */
/* feature additions from FW v2.0.0*/
typedef struct {
    /**<Wi-Fi/BT co-existence arbiter: 0 - Disabled, 1 - Enabled */
    bool isCoExEnabled;         
    
    /**<Wi-Fi/BT co-existence arbiter interface type */
    /**<0: 3-wire interface (BT_Act, BT_Prio, WLAN_Act) */
    /**<1: 2-wire interface (BT_Prio, WLAN_Act) */
    RNWF_COEX_INTF_TYPE_t interfaceType;         
    
    /**<Wi-Fi/BT co-existence arbiter WLAN RX priority over BT */
    /**<0: WLAN Rx priority lower than BT Low Priority */
    /**<1: WLAN Rx priority higher than BT Low Priority */
    bool isWlanRxPrio;          
    
    /**<Wi-Fi/BT co-existence arbiter WLAN TX priority over BT */
    /**<0: WLAN Tx priority lower than BT Low Priority */
    /**<1: WLAN Tx priority higher than BT Low Priority */
    bool isWlanTxPrio;          
    
    /**<Wi-Fi/BT co-existence arbiter antenna mode: 0 - Dedicated antenna, 1 - Shared antenna */
    RNWF_COEX_ANTENNA_MODE_t antennaMode;      

}RNWF_WIFI_BT_COEX_t;
 

/**
 * @ingroup     wifi_service
 * @brief       Wi-Fi callback function type
 * @param[in]   event   Event of type RNWF_WIFI_EVENT_t   
 * @param[in]   msg     Received data related to the passed event   
 * @return None
 */
typedef void (*RNWF_WIFI_CALLBACK_t)(RNWF_WIFI_EVENT_t event, uint8_t *msg);

/**
 * @ingroup     wifi_service
 * @brief       Wi-Fi callback handler
 * @var         gWifi_CallBack_Handler stores Wi-Fi service related callbacks
 */
extern RNWF_WIFI_CALLBACK_t gWifi_CallBack_Handler[RNWF_WIFI_SERVICE_CB_MAX];

/**
 * @ingroup wifi_service
 * @brief Wi-Fi Service Layer API to handle STA and SoftAP mode operations.
 * @note The asynchronous events are reported through callback, make sure that
 *  the application registers the callback using the RNWF_WIFI_SET_CALLBACK
 *  service request
 * @param[in]   request     Service request of type RNWF_WIFI_SERVICE_t
 * @param[in]   input       Parameters for the requested service
 * @retval      RNWF_PASS   Requested service is handled successfully
 * @retval      RNWF_FAIL   Requested service has failed
 */
RNWF_RESULT_t RNWF_WIFI_SrvCtrl( RNWF_WIFI_SERVICE_t request, void *input);

#endif	/* RNWF_WIFI_SERVICE_H */

/** @}*/