 /*
 * MAIN Generated Driver File
 * 
 * @file rnwf_app.h
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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_APP_H
#define	RNWF_APP_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "../system/system.h"
#include "../timer/delay.h"
#include "rnwf_interface.h"
#include "rnwf_wifi_service.h"

/* Wi-Fi Configuration */

#define HOME_AP_SSID        "wsn"
#define HOME_AP_PASSPHRASE  "brucenegley"
#define HOME_AP_SECURITY     RNWF_WPA2_MIXED


/* System Tick timer tick for 1Sec */
#define APP_SYS_TICK_COUNT_1SEC 1000

/* APP Switch long press delay in seconds */
#define APP_SW_LONG_PRESS_DELAY 3 * APP_SYS_TICK_COUNT_1SEC

/* LED blink timer period for 200mSec delay */
#define APP_LED_TCA1_PER_TICK   0x3D08

/* APP Cloud Telemetry Rate in seconds */
#define APP_CLOUD_REPORT_INTERVAL  2 * APP_SYS_TICK_COUNT_1SEC

/* Application generic purpose buffer size */
#define APP_BUFFER_SIZE_MAX     512

/* Cloud App specific State Machine */
typedef enum {
    APP_SYS_INIT,
    APP_SYS_UP,
    APP_WIFI_INIT,
    APP_WIFI_DOWN,
    APP_WIFI_UP,
    APP_CLOUD_DOWN,
    APP_CLOUD_UP
} APP_STATE_t;

typedef enum {
    APP_LED_ON = 1,
    APP_LED_OFF,
    APP_LED_BLINK,
    APP_LED_MAX,
} APP_LED_STATE_t;

/* Cloud Configuration */
#define CLIENT_ID         "rnwf02_device_01"
#define AZURE_SCOPE_ID          "0ne00ABD7D1"
#define AZURE_PUB_TELEMETRY     "devices/rnwf02_device_01/messages/events/"


#define AZURE_PUB_PROPERTY      "$iothub/twin/PATCH/properties/reported/?$rid=1"
#define AZURE_PUB_TWIN_GET      "$iothub/twin/GET/?$rid=getTwin"
#define AZURE_PUB_CMD_RESP      "$iothub/methods/res/200/?$%s"

#define AZURE_MSG_IPADDRESS     "\\\"ipAddress\\\":\\\"%s\\\""

/* Properties and Telemetry reporting format */
#define AZURE_FMT_BUTTON_TEL    "{\\\"buttonEvent\\\": {\\\"button_name\\\":\\\"SW0\\\", \\\"press_count\\\":%d}"
#define AZURE_FMT_COUNTER_TEL   "{\\\"counter\\\": \\\"%d\\\"}"
#define AZURE_FMT_RATE_PROP     "{\\\"reportRate\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"ad\\\":\\\"success\\\",\\\"value\\\":%s}}"
#define AZURE_FMT_LED0_PROP     "{\\\"LED0\\\":{\\\"ac\\\":200,\\\"av\\\":%s,\\\"ad\\\":\\\"success\\\",\\\"value\\\":%s}}"
#define AZURE_FMT_DELAY_RSP     "{\\\"status\\\":\\\"Success\\\",\\\"delay\\\":%d}"
#define AZURE_FMT_ECHO_RSP      "{\\\"echoString\\\":\\\"%s\\\"}"

/* Parsing Tags for the Azure messages */
#define AZURE_VERSION_TAG       "\"$version\\\":"
#define AZURE_LED0_TAG          "\"LED0\\\":"
#define AZURE_RATE_TAG          "\"reportRate\\\":"
#define AZURE_DEALY_TAG         "\"delay\\\":\\\"PT"
#define AZURE_ECHO_TAG          "\"echoString\\\":\\\""

#define CLOUD_STATE_MACHINE()         APP_AZURE_Task() /* Provide app specific implementation for APP_Cloud_Task() */

#define CLOUD_SUBACK_HANDLER()        APP_AZURE_SUBACK_Handler() /* Provide app specific implementation for APP_Cloud_SUBACK_Handler() */

#define CLOUD_SUBMSG_HANDLER(msg)     APP_AZURE_SUB_Handler(msg) /* Provide app specific implementation for APP_Cloud_SUB_Handler(msg) */

void RNWF_APP_Initialize(void);
RNWF_RESULT_t RNWF_IF_Init(void);
void APP_SW_Handler(void);
void APP_SYS_Tick(void);
RNWF_RESULT_t APP_MQTT_Publish(const char *top, const char *msg);
void APP_AZURE_Task(void);
void APP_AZURE_BUTTON_Telemetry(uint32_t press_count);
void APP_AZURE_COUNTER_Telemetry(uint32_t counter);

#endif	/* RNWF_APP_H */