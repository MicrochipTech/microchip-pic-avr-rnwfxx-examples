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
#define CLIENT_ID               "RNWF02_Test"
/* MQTT Publish Topic Name */
#define MQTT_PUB_TOPIC          "mchp/rnwf02/to" 


#define SUBSCRIBE_QOS_VALUE     RNWF_MQTT_SUBSCRIBE_QOS0

#define CLOUD_SUBACK_HANDLER()       APP_Cloud_SUBACK_Handler() /* Provide reference to a function that handles Subscribe acknowledgement */

#define CLOUD_SUBMSG_HANDLER(msg)    APP_Cloud_SUB_Handler(msg) /* Provide reference to a function that notifies the app about received message on a subscribed topic */
#define CLOUD_STATE_MACHINE()        APP_Cloud_Task()           /* Provide reference to a function that handles cloud specific app state machine */

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

#endif	/* RNWF02_APP_H */