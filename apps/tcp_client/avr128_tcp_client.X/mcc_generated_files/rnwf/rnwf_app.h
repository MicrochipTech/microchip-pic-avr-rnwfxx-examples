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
� [2024] Microchip Technology Inc. and its subsidiaries.

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
#define TOTAL_NET_SOCK_NUM      1
#define COUNTRY_CODE            "GEN"
#define PING_ENABLE             1
#define PING_TO_ADDRESS         "192.168.0.195"
#define CHANNEL_NUMBER          0
 
#define STA_AUTOCONNECT         1
#define CONNECTION_TIMEOUT      10000
#define HOME_AP_BSSID           ""
 



/**
 * @ingroup rnwf_app
 * @brief App initialization function to initialize Wi-Fi parameters
 * @param None
 * @return None
 */
void RNWF_APP_Initialize(void);

#endif	/* RNWF02_APP_H */