/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_wifi_service.h
 * 
 * @defgroup 
 *
 * @ingroup
 * 
 * @brief 
 *
 * @version Driver Version 1.0.0
*/

/*
? [2023] Microchip Technology Inc. and its subsidiaries.

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


/*! \page wifi_service Wi-Fi Service
This page introduces the user to the topic.
*/


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_WIFI_SERVICE_H
#define	RNWF_WIFI_SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define RNWF_WIFI_GET_STA_STATE     "AT+WSTA\r\n"
#define RNWF_WIFI_CONNECT           "AT+WSTA=1\r\n"
#define RNWF_WIFI_DISCONNECT        "AT+WSTA=0\r\n"
#define RNWF_WIFI_SET_STA_SSID      "AT+WSTAC=1,\"%s\"\r\n"
#define RNWF_WIFI_SET_STA_SEC       "AT+WSTAC=2,%d\r\n"
#define RNWF_WIFI_SET_STA_PWD       "AT+WSTAC=3,\"%s\"\r\n"

#define RNWF_WIFI_SET_STA_CHANNEL   "AT+WSTAC=4,%d\r\n"
#define RNWF_WIFI_SET_STA_BSSID     "AT+WSTAC=5,%s\r\n"
#define RNWF_WIFI_SET_STA_TIMEOUT   "AT+WSTAC=7,%d\r\n"

#define RNWF_WIFI_GET_AP_STATE   "AT+WAP\r\n"
#define RNWF_WIFI_SOFTAP_ENABLE  "AT+WAP=1\r\n"
#define RNWF_WIFI_SOFTAP_DISABLE "AT+WAP=0\r\n"
#define RNWF_WIFI_SET_AP_SSID    "AT+WAPC=1,\"%s\"\r\n"
#define RNWF_WIFI_SET_AP_SEC     "AT+WAPC=2,%d\r\n"
#define RNWF_WIFI_SET_AP_PWD     "AT+WAPC=3,\"%s\"\r\n"
#define RNWF_WIFI_SET_AP_CHANNEL "AT+WAPC=4,%d\r\n"
#define RNWF_WIFI_SET_AP_HIDDEN  "AT+WAPC=5,%d\r\n"


#define RNWF_WIFI_PSV_SCAN      "AT+WSCN=0\r\n"
#define RNWF_WIFI_ACT_SCAN      "AT+WSCN=1\r\n"

#define RNWF_SSID_LEN_MAX       33
#define RNWF_BSSID_LEN_MAX      32
#define RNWF_PWD_LEN_MAX        128

#define RNWF_WIFI_SERVICE_CB_MAX     2
/**
 @defgroup WIFI_GRP Wi-Fi Service API
 @{
 */


/**
 * @brief Wi-Fi Service List
 * 
 */
typedef enum {
    RNWF_STA_CONNECT,           /**<Request/Trigger Wi-Fi connect */
    RNWF_STA_DISCONNECT,        /**<Request/Trigger Wi-Fi disconnect */  
    RNWF_AP_DISABLE,             /**<Request/Trigger SoftAP disable */ 
    RNWF_SET_WIFI_PARAMS,       /**<Configure the Wi-Fi parameters */     
    RNWF_SET_WIFI_AP_CHANNEL,   /**<Configure the Wi-Fi channel */
    RNWF_SET_WIFI_BSSID,        /**<Configure the Access point's BSSID */
    RNWF_SET_WIFI_TIMEOUT,      /**<Configure Wi-Fi connection timeout */
    RNWF_SET_WIFI_HIDDEN,       /**<Configure Hidden mode SSID in SoftAP mode*/
    RNWF_WIFI_PASSIVE_SCAN,     /**<Request/Trigger Wi-Fi passive scan */
    RNWF_WIFI_ACTIVE_SCAN,      /**<Request/Trigger Wi-Fi active scan */                      
    RNWF_WIFI_SET_CALLBACK,     /**<Regester the call back for async events */
    RNWF_WIFI_SET_SRVC_CALLBACK,     /**<Regester the call back for async events */
}RNWF_WIFI_SERVICE_t;

/**
 * @brief Wi-Fi Event Code List
 * 
 */
typedef enum {
    RNWF_CONNECTED,             /**<Wi-Fi connected event code*/
    RNWF_DISCONNECTED,          /**<Wi-Fi disconnected event code*/
    RNWF_CONNECT_FAILED,        /**<Wi-Fi connection failure event code*/
    RNWF_DHCP_DONE,             /**<Wi-Fi DHCP complete event code*/
    RNWF_SCAN_INDICATION,       /**<Scan indication event to report each scan list */
    RNWF_SCAN_DONE,             /**<Scan complete event code*/
    RNWF_SNTP_UP,
}RNWF_WIFI_EVENT_t;

/**
 * @brief Wi-Fi security modes
 * 
 */
typedef enum {
    RNWF_OPEN,                  /**<OPEN mode, no security*/
    RNWF_RSVD,                  
    RNWF_WPA2_MIXED,            /**<WPA2 Mixed mode TKIP/CCMP*/
    RNWF_WPA2,                  /**<WPA2 CCMP*/
    RNWF_WPA3_TRANS,            /**<WPA3 Transition Mode*/
    RNWF_WPA3,                  /**<WPA3 mode*/
}RNWF_WIFI_SECURITY_t;

/**
 * @brief Wi-Fi operation modes
 * 
 */
typedef enum {
    RNWF_WIFI_MODE_STA, /**<Station (STA) mode of WiFi operation*/
    RNWF_WIFI_MODE_AP,  /**<Software Access Point (SoftAP) mode of WiFi operation*/
}RNWF_WIFI_MODE_t;

/**
 * @brief       Wi-Fi operation modes
 * 
 */
typedef struct {
    RNWF_WIFI_MODE_t mode;          /**<Wi-Fi operation mode ::RNWF_WIFI_MODE_t either STA or SoftAP */
    const char  *ssid;              /**<Wi-Fi SSID of Home AP or SoftAP */
    const char  *passphrase;        /**<Wi-Fi Passphrase of Home AP or SoftAP*/
    RNWF_WIFI_SECURITY_t security;  /**<Wi-Fi Secrity mode ::RNWF_WIFI_SECURITY_t */
    uint8_t autoconnect;            /**<Wi-Fi autoconnect, SoftAP */
}RNWF_WIFI_PARAM_t;


/**
 * @brief       Wi-Fi callback function type
 * 
 * @param[out]  event One of the ::RNWF_WIFI_EVENT_t event   
 * @param[out]  msg Received data related to the passed event   
 * 
 */
typedef void (*RNWF_WIFI_CALLBACK_t)(RNWF_WIFI_EVENT_t event, uint8_t *msg);


/**
 * @brief       Wi-Fi callback handler
 * 
 */
extern RNWF_WIFI_CALLBACK_t gWifi_CallBack_Handler[RNWF_WIFI_SERVICE_CB_MAX];

/**
 * @brief Wi-Fi Service Layer API to handle STA and SoftAP mode operations.
 * 
 * @note The asynchronous events are reported through callback, make sure that
 *  the application registers the callback using the ::RNWF_WIFI_SET_CALLBACK
 *  service request
 *
 * @param[in] request   Service request ::RNWF_WIFI_SERVICE_t
 * @param[in] input     Parameters for the requested service
 * 
 * @return ::RNWF_PASS Requested service is handled successfully
 * @return ::RNWF_FAIL Requested service has failed
 */
RNWF_RESULT_t RNWF_WIFI_SrvCtrl( RNWF_WIFI_SERVICE_t request, void *input);

#endif	/* RNWF_WIFI_SERVICE_H */

/** @}*/