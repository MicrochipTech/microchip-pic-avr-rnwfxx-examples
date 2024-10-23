/**
 * Application source file
 * 
 * @file rnwf_app.c
 *
 * @ingroup rnwf_app
 * 
 * @brief Application file
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

#include "rnwf_app.h"
#include "rnwf_wifi_service.h"
#include "rnwf_system_service.h"
#include "rnwf_provision_service.h"

/*
    Main application
*/


/* feature additions from FW v2.0.0*/
uint8_t appBuffer[APP_BUFFER_SZ];







/**
 * @ingroup rnwf_app
 * @brief Wi-Fi callback function for Wi-Fi events like scan, SNTP, DHCP
 * @param[in] event         Wi-Fi event type
 * @param[in] p_str         string pointer
 * @return None
 */
void APP_WIFI_Callback(RNWF_WIFI_EVENT_t event, uint8_t *p_str)
{
    switch(event)
    {
        case RNWF_SNTP_UP:
        {   
            break;
        }
        case RNWF_CONNECTED:
        {
            printf("Wi-Fi Connected\n");
            break;
        }
        case RNWF_DISCONNECTED:
        {
            printf("Wi-Fi Disconnected\nReconnecting... \n");
            RNWF_WIFI_SrvCtrl(RNWF_STA_CONNECT, NULL);
            break;
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_DHCP_IPV4_DONE:
        {
            printf("\n\rDHCP IPv4: %s\n\r", &p_str[2]);
            break;       
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_DHCP_LINK_LOCAL_IPV6_DONE:
        {
            printf("\n\rDHCP link-local IPv6:%s\n\r", &p_str[2]);
            break;
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_DHCP_GLOBAL_IPV6_DONE:
        {
            printf("\n\rDHCP global IPv6:%s\n\r", &p_str[2]);
            break;
        }
    /* feature additions from FW v2.0.0*/    
        case RNWF_SET_REGDOM:
        {
            RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_REGDOM, (void *)COUNTRY_CODE);
            break;
        }
        case RNWF_SCAN_INDICATION:
        {
            break;
        }
        case RNWF_SCAN_DONE:
        {
            break;
        }
        case RNWF_CONNECT_FAILED:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * @ingroup rnwf_app
 * @brief Wi-fi provisioning callback function for provisioning events
 * @param[in] event         Provisioning event
 * @param[in] p_str         string pointer
 * @return None
 */
void APP_PROV_Callback(RNWF_PROV_EVENT_t event, uint8_t *p_str)
{
    switch(event)
    {
        case RNWF_PROV_COMPLTE:
        {
            RNWF_PROV_SrvCtrl(RNWF_PROV_DISABLE, NULL);
            
            RNWF_WIFI_SrvCtrl(RNWF_WIFI_SET_CALLBACK, APP_WIFI_Callback);
            // Application can save the configuration in NVM
            RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_PARAMS, (void *)p_str);                      
        }
        break;  
        case RNWF_PROV_FAILURE:
            break;
        default:
            break;  
    }
}

void RNWF_APP_Initialize(void)
{
/* feature additions from FW v2.0.0*/
    // Disable NTP Client
        RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_RMV_SNTP, NULL);
        RNWF_WIFI_SrvCtrl(RNWF_GET_WIFI_REGDOM, appBuffer);
    /* RNWF Application Callback register */
        RNWF_PROV_SrvCtrl(RNWF_PROV_ENABLE, NULL);                 
        RNWF_PROV_SrvCtrl(RNWF_PROV_SET_CALLBACK, (void *)APP_PROV_Callback);

    while(1)
    {  
        RNWF_EVENT_Handler();
    }    
}