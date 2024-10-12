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
#include "rnwf_net_service.h"

/*
    Main application
*/


/* feature additions from FW v2.0.0*/
uint8_t appBuffer[APP_BUFFER_SZ];
uint8_t tcp_client_msg[] = "Type here and receive its echo!!\n\r";


/* feature additions from FW v2.0.0*/
    static uint8_t isSockOpen = 0;      //guard condition to open a socket

RNWF_NET_SOCKET_t tcp_client_sock_6666 = {
        .bind_type = RNWF_BIND_REMOTE,
        .sock_port = 6666,
        .sock_type = RNWF_SOCK_TCP,
        .sock_addr = "192.168.0.195",
    /* feature addition from FW v2.0.0*/
        .sockIP = RNWF_NET_SOCK_IPv4,
        };
      




/**
 * @ingroup rnwf_app
 * @brief Net socket events function
 * @param[in] socket        socket address
 * @param[in] event         Net socket event type
 * @param[in] p_str         string pointer
 * @return None
 */
void APP_SOCKET_Callback(uint32_t socket, RNWF_NET_SOCK_EVENT_t event, uint8_t *p_str)
{
    switch(event)
    {
        case RNWF_NET_SOCK_EVENT_CONNECTED: 
        {           
            printf("Connected to server!\n");
        /* An example to write data on a non-TLS socket */
          RNWF_NET_TCP_SOCK_Write(socket, sizeof(tcp_client_msg), tcp_client_msg);
            break;
        }
        case RNWF_NET_SOCK_EVENT_DISCONNECTED:
        {
            printf("Socket Closed!\n"); 
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, &socket); 
            break;
        }
        case RNWF_NET_SOCK_EVENT_ERROR:
        {
            printf("Error: %s!\n", p_str);
            break;
        }  
        case RNWF_NET_SOCK_EVENT_READ:
        {   
        /* An example to read/write data from/to a socket */            
            uint8_t rx_data[64];
            int32_t rcvd_len;
            uint16_t rx_len = *(uint16_t *)p_str;         
            if((rx_len < 64) && (rcvd_len = RNWF_NET_TCP_SOCK_Read(socket, rx_len, rx_data)) > 0)
            {                
                rx_data[rx_len] = '\0';
                printf("Rx->%s\r\n", rx_data);
                RNWF_NET_TCP_SOCK_Write(socket, rx_len, rx_data);                
            }


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
            if(isSockOpen == 0)
            {
                isSockOpen = 1;
                RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_TCP_OPEN, &tcp_client_sock_6666);
            }
            RNWF_WIFI_SrvCtrl(RNWF_WIFI_PING_ENBL, (const char*)PING_TO_ADDRESS);
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
    /* feature additions from FW v2.0.0*/
        case RNWF_PING_ERROR:
        {
            printf("\n\rPing Error - Ping failed with error code: %s\n\n\r", &p_str[0]);
            break;
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_PING_SUCCESS:
        {
            printf("\n\rPing Success!\n\r");
            printf("Received reply from %s\n\n\r", PING_TO_ADDRESS);
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


void RNWF_APP_Initialize(void)
{
/* feature additions from FW v2.0.0*/
    // Disable NTP Client
        RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_RMV_SNTP, NULL);
        RNWF_WIFI_SrvCtrl(RNWF_GET_WIFI_REGDOM, appBuffer);
    /* Wi-Fi Connectivity */
        RNWF_WIFI_PARAM_t wifi_sta_cfg = {RNWF_WIFI_MODE_STA, HOME_AP_SSID, HOME_AP_PASSPHRASE, HOME_AP_SECURITY, STA_AUTOCONNECT};    
        printf("Connecting to %s\r\n", HOME_AP_SSID);
        RNWF_WIFI_SrvCtrl(RNWF_WIFI_SET_CALLBACK, APP_WIFI_Callback);
        RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_PARAMS, &wifi_sta_cfg);
    /* RNWF Application Callback register */
        RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_SET_CALLBACK, APP_SOCKET_Callback);  

    while(1)
    {  
        RNWF_EVENT_Handler();
    }    
}