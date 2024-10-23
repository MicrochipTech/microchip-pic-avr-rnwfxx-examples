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



/* feature additions from FW v2.0.0*/
    static uint8_t isSockOpen = 0;      //guard condition to open a socket

RNWF_NET_SOCKET_t tcp_client_sock_443 = {
        .bind_type = RNWF_BIND_REMOTE,
        .sock_port = 443,
        .sock_type = RNWF_SOCK_TCP,
    /* feature additions from FW v2.0.0*/
        .sock_addr = "s3-us-west-2.amazonaws.com",
    /* feature addition from FW v2.0.0*/
        .sockIP = RNWF_NET_SOCK_IPv4,
        .tls_conf = 1,
        };
      
/* feature addition from FW v2.0.0*/
const char *tls_cfg_1[] = {"AmazonRootCA1", 0, 0, 0, "file-download-files.s3-us-west-2.amazonaws.com", "s3-us-west-2.amazonaws.com", true, true};
uint8_t aws_file_request[] = "GET /ref_doc.pdf HTTP/1.1\r\nHost: file-download-files.s3-us-west-2.amazonaws.com\r\nConnection: close\r\n\r\n";

#define TCP_BUF_LEN_MAX     1024
#define HTTP_CONTENT_LEN    "Content-Length:"
uint8_t tcp_data[TCP_BUF_LEN_MAX];
uint32_t gFile_Len = 0;



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
    
    uint8_t *tmpPtr;
    static uint32_t rcvd_bytes;
    
    switch(event)
    {
        case RNWF_NET_SOCK_EVENT_CONNECTED: 
        {           
            printf("Connected to server!\n");
            break;
        }
        case RNWF_NET_SOCK_EVENT_TLS_DONE:
        {
        /* An example to write data on socket with TLS connection */
          RNWF_NET_TCP_SOCK_Write(socket, strlen((char *)aws_file_request), aws_file_request);                            
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
            /* An example to read data continuously from a socket */
            int ret_val;
            uint16_t rx_len = *(uint16_t *)p_str;              
            while(rx_len)
            {
                uint16_t read_len = (rx_len > TCP_BUF_LEN_MAX)?TCP_BUF_LEN_MAX:rx_len;                
                if(((ret_val = RNWF_NET_TCP_SOCK_Read(socket, read_len, tcp_data)) > 0))
                {                              
                    if(!gFile_Len)
                    {
                        printf("%.*s\r\n", ret_val, tcp_data);
                        if((tmpPtr = (uint8_t *)strstr(tcp_data, HTTP_CONTENT_LEN)) != NULL)
                        {
                            volatile char *token = strtok(tmpPtr, "\r\n");
                            gFile_Len = strtol((token+sizeof(HTTP_CONTENT_LEN)), NULL, 10);                                                        
                            printf("File Size = %lu\r\n", gFile_Len);
                        }                                                
                        break;
                    }
                    rcvd_bytes += ret_val;
                    rx_len -= ret_val;
                    printf("Received %lu bytes\r\n", rcvd_bytes);
                    if(rcvd_bytes >= gFile_Len)
                    {
                        printf("Receive Complete!\r\n");  
                        RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, (void *)&socket);
                    }                    
               }            
                else
                {
                    printf("Read Timeout!\r\n");
                    break;
                }
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
            if(isSockOpen == 1)
            {
                RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_TCP_OPEN, &tcp_client_sock_443);
                isSockOpen++;
            }
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
                RNWF_NET_SOCK_SrvCtrl(RNWF_NET_TLS_CONFIG_1, tls_cfg_1);
                tcp_client_sock_443.tls_conf = RNWF_NET_TLS_CONFIG_1;
                isSockOpen = 1;
            }
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


void RNWF_APP_Initialize(void)
{
/* feature additions from FW v2.0.0*/
    // Configure and Enable NTP Client                                        
        const char sntp_url[] =  "216.239.35.4";    
        RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SET_SNTP, sntp_url);           
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