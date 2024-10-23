/**
 * Wi-Fi service implementation source file
 * 
 * @file rnwf_wifi_service.c
 *
 * @ingroup wifi_service
 * 
 * @brief This file contains APIs and data types for Wifi service
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

/*! \mainpage RNWF02 Service API User Guide

The RNWF02 AT Command service layer wraps the ASCII
command implementation in a service architecture. The
service layer API's are documented here can enable 
easy and quick applciation development.

*/

/* This section lists the other files that are included in this file.
 */
#include <stdio.h>
#include <string.h>

#include "rnwf_interface.h"
#include "rnwf_wifi_service.h"
 

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
RNWF_WIFI_CALLBACK_t gWifi_CallBack_Handler[RNWF_WIFI_SERVICE_CB_MAX];

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

RNWF_RESULT_t RNWF_WIFI_SrvCtrl( RNWF_WIFI_SERVICE_t request, void *input)  {
    
    RNWF_RESULT_t result = RNWF_PASS;
    
    switch (request)
    {
        case RNWF_STA_CONNECT:
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_CONNECT);
            break;
        case RNWF_STA_DISCONNECT:
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_DISCONNECT);
            break;
        case RNWF_AP_DISABLE:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SOFTAP_DISABLE); 
            break;
        }
        case RNWF_SET_WIFI_BSSID:
        {            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_STA_BSSID, (uint8_t *)input);            
            break;
        } 
        case RNWF_SET_WIFI_AP_CHANNEL:
        {          
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_AP_CHANNEL, *(uint8_t *)input); 
            break;
        }
        case RNWF_SET_WIFI_TIMEOUT:
        {            
            uint32_t toVal = *(uint32_t *)input;
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_STA_TIMEOUT, toVal);            
            break;
        } 
        case RNWF_SET_WIFI_PARAMS:  
        {
            RNWF_WIFI_PARAM_t *wifi_config = (RNWF_WIFI_PARAM_t *)input;
            
            if(wifi_config->mode == RNWF_WIFI_MODE_STA)
            {
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SOFTAP_DISABLE);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_DISCONNECT);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_STA_SSID, wifi_config->ssid);            
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_STA_PWD, wifi_config->passphrase);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_STA_SEC, wifi_config->security);    
                if(wifi_config->autoconnect)
                {
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_CONNECT);
                }
            }
            else if(wifi_config->mode == RNWF_WIFI_MODE_AP)                
            {                   
                uint8_t default_channel = 6;
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_DISCONNECT);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SOFTAP_DISABLE);                
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_AP_SSID, wifi_config->ssid);            
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_AP_PWD, wifi_config->passphrase);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_AP_SEC, wifi_config->security);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_AP_CHANNEL, default_channel);    
                if(wifi_config->autoconnect)
                {
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SOFTAP_ENABLE);
                }
            }
            break;            
        }
        case RNWF_GET_WIFI_REGDOM:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+WIFIC:", input, RNWF_WIFI_GET_REGULATORY_DOMAIN);
            break;
        }
        case RNWF_SET_WIFI_REGDOM:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_REGULATORY_DOMAIN, (const char*)input);
            break;
        }
    #ifdef WIFI_BT_COEX_ENABLE
        case RNWF_WIFI_SET_COEX:
        {
            RNWF_WIFI_BT_COEX_t *wifibtCoex = (RNWF_WIFI_BT_COEX_t *)input;
            if(wifibtCoex != NULL)
            {
                if(wifibtCoex->isCoExEnabled)
                {
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_BT_INTF_TYPE, wifibtCoex->interfaceType);
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_BT_WLAN_RX_PRI, wifibtCoex->isWlanRxPrio);
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_BT_WLAN_TX_PRI, wifibtCoex->isWlanTxPrio);
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_BT_ANTENNA_MODE, wifibtCoex->antennaMode);
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_WIFI_BT_COEX, wifibtCoex->isCoExEnabled);
                }
            }
            break;
        }
    #endif
    #ifdef POWERSAVE_ENABLE
        case RNWF_SET_WIFI_PSM:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_WIFI_PWRSV);
            break;
        }
    #endif
    #ifdef PING_ENABLE
        case RNWF_WIFI_PING_ENBL:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_WIFI_PING_ON, (const char*)input);
        }
        break;
    #endif
 
        case RNWF_SET_WIFI_HIDDEN:
        {            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_SET_AP_HIDDEN, *(uint8_t *)input);            
            break;
        }
        case RNWF_WIFI_PASSIVE_SCAN:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_PSV_SCAN);
            break;
        }    
        case RNWF_WIFI_ACTIVE_SCAN:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_WIFI_ACT_SCAN);
            break;
        }            
        case RNWF_WIFI_SET_CALLBACK:  
        {
            gWifi_CallBack_Handler[1] = (RNWF_WIFI_CALLBACK_t)input;                        
        }
        break;
        case RNWF_WIFI_SET_SRVC_CALLBACK:                        
        {
            gWifi_CallBack_Handler[0] = (RNWF_WIFI_CALLBACK_t)input;                
        }
        break;
        
        default:
            result = RNWF_FAIL;
            break;
    };    
    
    return result;
}


/* *****************************************************************************
 End of File
 */
