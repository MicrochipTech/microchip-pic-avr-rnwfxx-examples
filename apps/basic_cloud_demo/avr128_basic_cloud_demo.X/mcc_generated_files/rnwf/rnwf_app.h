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

/* Wi-Fi Configuration */

#define HOME_AP_SSID        "wsn"
#define HOME_AP_PASSPHRASE  "brucenegley"
#define HOME_AP_SECURITY     RNWF_WPA2_MIXED




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

/* Cloud Configuration */
#define CLIENT_ID         "RNWF02_Test"
/* MQTT Publish Topic Name */
#define MQTT_PUB_TOPIC     "mchp/rnwf02/from" 


#define SUBSCRIBE_QOS_VALUE         RNWF_MQTT_SUBSCRIBE_QOS0

#define CLOUD_SUBACK_HANDLER()        APP_Cloud_SUBACK_Handler()    /* Provide app specific implementation for APP_Cloud_SUBACK_Handler() */
#define CLOUD_SUBMSG_HANDLER(msg)     APP_Cloud_SUB_Handler(msg)    /* Provide app specific implementation for APP_Cloud_SUB_Handler(msg) */
#define CLOUD_STATE_MACHINE()         APP_Cloud_Task()              /* Provide app specific implementation for APP_Cloud_Task() */

void RNWF_APP_Initialize(void);
RNWF_RESULT_t RNWF_IF_Init(void);
RNWF_RESULT_t APP_MQTT_Publish(const char *top, const char *msg);

#endif	/* RNWF_APP_H */