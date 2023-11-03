/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_provision_service.c
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

/*! \mainpage RNWF02 Service API User Guide

The RNWF02 AT Command service layer wraps the ASCII
command implementation in a service architecture. The
service layer API's are documented here can enable 
easy and quick applciation development.

- \ref SERVICE_GRP "System Service"
- \ref WIFI_GRP "Wi-Fi Service"
- \ref WIFI_PROV_GRP "Wi-Fi Provisioning Service"
- \ref MQTT_GRP "MQTT Service"
- \ref NETSOCK_GRP "Network Socket Service"
- \ref OTA_GRP "OTA Service"
*/

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../timer/delay.h"

#include "rnwf_interface.h"
#include "rnwf_net_service.h"
#include "rnwf_wifi_service.h"
#include "rnwf_provision_service.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

RNWF_PROV_CALLBACK_t gProv_CallBack_Handler; 

/* TCP Socket */
RNWF_NET_SOCKET_t provision_socket = {
        .bind_type = RNWF_BIND_LOCAL,
        .sock_port = 80,
        .sock_type = RNWF_SOCK_TCP,
        };

/*Parse or translate security type received from mobile app*/
RNWF_WIFI_SECURITY_t RNWF_PROV_ParseAuth(uint8_t secType)
{
    RNWF_WIFI_SECURITY_t authType;
    
    switch(secType)
    {
        case 1:
            authType = secType - 1;
            break;
        case 2:
            authType = secType;
            break;
        case 4:
            authType = secType + 1;
            break;
            
        //To-Do: 
        //Enterprise mode security type if applicable
            
        default:
            printf("Invalid security type\r\n");
            break;
    }
    return authType;
}
            
/* Parse Wi-Fi configuration file */
/* Format is APP_WIFI_PROV_WIFI_CONFIG_ID,<SSID>,<AUTH>,<PASSPHRASE>*/
RNWF_RESULT_t RNWF_PROV_APP_Parse(uint8_t *wifiCofnig, RNWF_WIFI_PARAM_t *wifi_config)
{
    char* p;    
    RNWF_RESULT_t ret = RNWF_PASS;
    
    p = strtok((char *)wifiCofnig, ",");
    if (p != NULL && !strncmp(p, APP_WIFI_PROV_CONFIG_ID, strlen(APP_WIFI_PROV_CONFIG_ID))) 
    {
        p = strtok(NULL, ",");
        if (p)
            wifi_config->ssid = p;

        p = strtok(NULL, ",");
        if (p) 
        {
            uint8_t security = (RNWF_WIFI_SECURITY_t)atoi(p);
            
            
            
            wifi_config->security = RNWF_PROV_ParseAuth(security);
                    
//            wifi_config->security = (RNWF_WIFI_SECURITY_t)atoi(p);
            
            if (RNWF_OPEN < wifi_config->security &&  wifi_config->security <= RNWF_WPA3)
            {
                p = strtok(NULL, ",");
                if (p) 
                    wifi_config->passphrase =  p;
                else
                    ret = RNWF_FAIL;
            } 
            else if (wifi_config->security == RNWF_OPEN)
                wifi_config->passphrase = NULL;
            else
                ret = RNWF_FAIL;
        }
        else
            ret = RNWF_FAIL;

        DBG_MSG_PROV("SSID:%s - PASSPHRASE:%s - AUTH:%d\r\n", 
                            wifi_config->ssid, 
                            wifi_config->passphrase, 
                            wifi_config->security);
    }
    else if(p != NULL && !strncmp(p, APP_WIFI_PROV_DONE_ID, strlen(APP_WIFI_PROV_DONE_ID)))
    {
        
        
    }
    return ret;
}


RNWF_RESULT_t RNWF_APP_PROV_Process(uint32_t socket, uint16_t rx_len) {
        
    RNWF_WIFI_PARAM_t wifiConfig;
    uint8_t prov_buf[PROV_BUF_LEN_MAX];
    
    if(RNWF_NET_TCP_SOCK_Read(socket, rx_len, (uint8_t *)prov_buf) > 0)
    {
        if(RNWF_PROV_APP_Parse(prov_buf, &wifiConfig) == RNWF_PASS)
        {
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, &socket);
            wifiConfig.mode = RNWF_WIFI_MODE_STA;
            wifiConfig.autoconnect = 1;
            if(gProv_CallBack_Handler)
                gProv_CallBack_Handler(RNWF_PROV_COMPLTE, (uint8_t *)&wifiConfig);
            return RNWF_PASS;
        }
        else
        {
            if(gProv_CallBack_Handler)
                gProv_CallBack_Handler(RNWF_PROV_FAILURE, NULL);
        }
    }
    return RNWF_FAIL;
}

void RNWF_PROV_SOCKET_Callback(uint32_t sock, RNWF_NET_SOCK_EVENT_t event, uint8_t *p_str)
{
    switch(event)
    {
        case RNWF_NET_SOCK_EVENT_CONNECTED:
        {
            break;
        }
        case RNWF_NET_SOCK_EVENT_DISCONNECTED:
        {
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, &sock);
            break;
        }
        case RNWF_NET_SOCK_EVENT_READ:
        {
        RNWF_APP_PROV_Process(sock, *(uint16_t *)p_str);
        }
        default:
            break;
    }
}

void RNWF_PROV_WIFI_Callback(RNWF_WIFI_EVENT_t event, uint8_t *p_str)
{            
    switch(event)
    {
        case RNWF_DHCP_DONE:
            printf("DHCP IP:%s\n", &p_str[2]);
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_TCP_OPEN, &provision_socket);
            
            break;
        case RNWF_SCAN_INDICATION:
        {
            break;            
        }
        case RNWF_SCAN_DONE:            

            
            break;
        default:
            break;
                    
    }    
}

RNWF_RESULT_t RNWF_PROV_SrvCtrl(RNWF_PROV_SERVICE_t request, void *input)  {
    
    switch(request)
    {
        case RNWF_PROV_ENABLE:
        {                   
            /* RNWF Application Callback register */
            RNWF_WIFI_SrvCtrl(RNWF_WIFI_SET_SRVC_CALLBACK, RNWF_PROV_WIFI_Callback);
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_SET_SRVC_CALLBACK, RNWF_PROV_SOCKET_Callback);

            const char *dhcps_cfg[] = {"192.168.1.1/24", "192.168.1.10"};
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_DHCP_SERVER_ENABLE, dhcps_cfg);  

            /* Wi-Fii Connectivity */
            RNWF_WIFI_PARAM_t wifi_ap_cfg = {RNWF_WIFI_MODE_AP, PROV_AP_SSID, PROV_AP_PASSPHRASE, PROV_AP_SECURITY, 1};    
            RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_PARAMS, &wifi_ap_cfg);
            DBG_MSG_PROV("Provision mode enabled\n\r");
            DBG_MSG_PROV("SSID:%s - PASSPHRASE:%s - AUTH:%d\n\r", 
                                wifi_ap_cfg.ssid, 
                                wifi_ap_cfg.passphrase,
                                wifi_ap_cfg.security);
        }
        break;
        
        case RNWF_PROV_DISABLE:
        {
            /* RNWF Application Callback register */
            RNWF_WIFI_SrvCtrl(RNWF_WIFI_SET_SRVC_CALLBACK, NULL);
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_SET_SRVC_CALLBACK, NULL);
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_DHCP_SERVER_DISABLE, NULL);
            DBG_MSG_PROV("Provision mode disabled\n\r");
        }
        break;
        
        case RNWF_PROV_SET_CALLBACK:
        {
            if(input != NULL)
                gProv_CallBack_Handler = (RNWF_PROV_CALLBACK_t)input;
        }    
        break;
            
        default:
            break;
    }
    
    return RNWF_PASS;
    
}


/* *****************************************************************************
 End of File
 */