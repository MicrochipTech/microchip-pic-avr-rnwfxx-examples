/**
 * Application header file
 * 
 * @file rnwf_app.h
 * 
 * @defgroup rnwf_app RNWF Application
 *
 * @brief Application header file
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
#ifndef RNWF02_APP_H
#define	RNWF02_APP_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "../system/system.h"
#include "../timer/delay.h"
#include "rnwf_interface.h"

/* Wi-Fi Configuration */

#define HOME_AP_SSID            "wsn"
#define HOME_AP_PASSPHRASE      "brucenegley"
#define HOME_AP_SECURITY        RNWF_WPA2_MIXED

/* feature additions from FW v2.0.0*/
#define APP_BUFFER_SZ           255
#define COUNTRY_CODE            "GEN"
#define CHANNEL_NUMBER          0
 
#define STA_AUTOCONNECT         1
#define CONNECTION_TIMEOUT      10000
 
/* System Tick timer tick for 1Sec */
#define APP_SYS_TICK_COUNT_1SEC 1000

/* APP Switch long press delay in seconds */
#define APP_SW_LONG_PRESS_DELAY 3 * APP_SYS_TICK_COUNT_1SEC

/* APP Cloud Telemetry Rate in seconds */
#define APP_CLOUD_REPORT_INTERVAL  10 * APP_SYS_TICK_COUNT_1SEC 




/* Application generic purpose buffer size */
#define APP_BUFFER_SIZE_MAX     512
/**
 * @ingroup rnwf_app
 * @brief Cloud App specific State Machine
 * @enum APP_STATE_t
 */
typedef enum {
    APP_SYS_INIT,
    APP_SYS_UP,
    APP_WIFI_INIT,
    APP_WIFI_DOWN,
    APP_WIFI_UP,
    APP_CLOUD_DOWN,
    APP_CLOUD_UP
} APP_STATE_t;

/* Cloud Configuration */
#define CLIENT_ID               "rnwf02-temp-node1"
#define AZURE_PUB_TELEMETRY     "devices/rnwf02-temp-node1/messages/events/"
#define AZURE_PUB_TWIN_GET      "$iothub/twin/GET/?$rid=getTwin"
#define AZURE_PUB_PROPERTY      "$iothub/twin/PATCH/properties/reported/?$rid=1"
#define AZURE_PUB_CMD_RESP      "$iothub/methods/res/200/?$%s"

/* Properties and Telemetry reporting format */
#define AZURE_FMT_COUNTER_TEL   "{\\\"counter\\\": \\\"%d\\\"}"
#define AZURE_FMT_BUTTON_TEL    "{\\\"buttonEvent\\\": {\\\"button_name\\\":\\\"SW0\\\", \\\"press_count\\\":%d}"
#define AZURE_MSG_IPADDRESS     "\\\"ipAddress\\\":\\\"%s\\\""
#define AZURE_FMT_DELAY_RSP     "{\\\"status\\\":\\\"Success\\\",\\\"delay\\\":%d}"
#define AZURE_FMT_ECHO_RSP      "{\\\"echoString\\\":\\\"%s\\\"}"

/* Parsing Tags for the Azure messages */
#define AZURE_DEALY_TAG         "\"delay\\\":\\\"PT"
#define AZURE_ECHO_TAG          "\"echoString\\\":\\\""


#define CLOUD_SUBACK_HANDLER()       APP_AZURE_SUBACK_Handler() /* Provide reference to a function that handles Subscribe acknowledgement */

#define CLOUD_SUBMSG_HANDLER(msg)    APP_AZURE_SUB_Handler(msg) /* Provide reference to a function that notifies the app about received message on a subscribed topic */
#define CLOUD_STATE_MACHINE()        APP_AZURE_Task() /* Provide reference to a function that handles cloud specific app state machine */

/**
 * @ingroup rnwf_app
 * @brief App initialization function to initialize Wi-Fi parameters
 * @param None
 * @return None
 */
void RNWF_APP_Initialize(void);
/**
 * @ingroup rnwf_app
 * @brief MQTT publish function
 * @param[in] top       topic names
 * @param[in] msg       publish message
 * @return RNWF_PASS or RNWF_FAIL
 */
RNWF_RESULT_t APP_MQTT_Publish(const char *top, const char *msg);

void APP_SW_Handler(void);
void APP_SYS_Tick(void);
void APP_AZURE_Task(void);
void APP_AZURE_BUTTON_Telemetry(uint32_t press_count);
void APP_AZURE_COUNTER_Telemetry(uint32_t counter);

#endif	/* RNWF02_APP_H */