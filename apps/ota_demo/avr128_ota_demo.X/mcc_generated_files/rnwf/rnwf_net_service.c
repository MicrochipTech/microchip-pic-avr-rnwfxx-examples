/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_net_service.c
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

/* This section lists the other files that are included in this file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../timer/delay.h"

#include "rnwf_interface.h"
#include "rnwf_net_service.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

RNWF_NET_SOCK_CALLBACK_t gSocket_CallBack_Handler[RNWF_NET_SOCK_SERVICE_CB_MAX];

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

RNWF_RESULT_t RNWF_NET_SOCK_SrvCtrl( RNWF_NET_SOCK_SERVICE_t request, void *input)  {
         
    RNWF_RESULT_t result = RNWF_PASS; 
    switch(request)
    {
        case RNWF_NET_DHCP_SERVER_ENABLE:
        {
            if(input == NULL)
                break;
            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_DHCPS_DISABLE);
            const char **dhcps_cfg_list = input;                        
            if(dhcps_cfg_list[0] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_NETIF_SET_IP, dhcps_cfg_list[0]);            
            if(dhcps_cfg_list[1] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_DHCPS_POOL_START, dhcps_cfg_list[1]);
            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_DHCPS_ENABLE);
            
            break;
        }
        case RNWF_NET_DHCP_SERVER_DISABLE:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_DHCPS_DISABLE);   
            break;
        }
        case RNWF_NET_SOCK_TCP_OPEN: 
        {
            RNWF_NET_SOCKET_t *socket = (RNWF_NET_SOCKET_t*)(input); 
            uint8_t socket_id[32];
            if(RNWF_CMD_SEND_OK_WAIT(RNWF_SOCK_OPEN_RESP, (uint8_t *)socket_id, RNWF_SOCK_OPEN_TCP) == RNWF_PASS)
            {
                sscanf((char *)socket_id, "%lu", &socket->sock_master);
                switch(socket->bind_type)
                {
                    case RNWF_BIND_LOCAL:
                        result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BIND_LOCAL, socket->sock_master, socket->sock_port);                        
                        break;
                    case RNWF_BIND_REMOTE:
                        result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BIND_REMOTE, socket->sock_master, socket->sock_addr, socket->sock_port);
                        break;
                    case RNWF_BIND_MCAST:
                        result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BIND_MCAST, socket->sock_master, socket->sock_addr, socket->sock_port);
                        break;
                    default:
                        break;
                }
                if(socket->tls_conf && result == RNWF_PASS)
                    result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_CONFIG_TLS, socket->sock_master, socket->tls_conf);
            }                                    
            break;
        }
            
        case RNWF_NET_SOCK_UDP_OPEN:   
        {
            RNWF_NET_SOCKET_t *socket = (RNWF_NET_SOCKET_t*)(input);
            int8_t socket_id[32];
            if(RNWF_CMD_SEND_OK_WAIT(RNWF_SOCK_OPEN_RESP, (uint8_t *)socket_id, RNWF_SOCK_OPEN_UDP) == RNWF_PASS)
            {
                sscanf((char *)socket_id, "%lu", &socket->sock_master);
                switch(socket->bind_type)
                {
                    case RNWF_BIND_LOCAL:
                        result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BIND_LOCAL, socket->sock_master, socket->sock_port);                        
                        break;
                    case RNWF_BIND_REMOTE:
                        result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BIND_REMOTE, socket->sock_master, socket->sock_addr, socket->sock_port);
                        break;
                    case RNWF_BIND_MCAST:
                        result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BIND_MCAST, socket->sock_master, socket->sock_addr, socket->sock_port);
                        break;   
                    default:
                        break;
                }               
            }             
            break;
        }
            
        case RNWF_NET_SOCK_CLOSE:
        {
            uint32_t socket = *((uint32_t *)input);
            if(socket)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_CLOSE, socket);            
            break;
        }
        case RNWF_NET_SOCK_CONFIG:
        {
            RNWF_NET_SOCKET_CONFIG_t *sock_cfg = (RNWF_NET_SOCKET_CONFIG_t *)input;
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_CONFIG_NODELAY, sock_cfg->sock_id, sock_cfg->sock_nodelay);            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_CONFIG_KEEPALIVE, sock_cfg->sock_id, sock_cfg->sock_keepalive);              
            break;
        }
        case RNWF_NET_TLS_CONFIG_1:
        case RNWF_NET_TLS_CONFIG_2:
        {
            const char **tls_cfg_list = input;            
            if(tls_cfg_list[RNWF_NET_TLS_CA_CERT] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_TLS_SET_CA_NAME, request, tls_cfg_list[RNWF_NET_TLS_CA_CERT]);     
            if(tls_cfg_list[RNWF_NET_TLS_CERT_NAME] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_TLS_SET_CERT_NAME, request, tls_cfg_list[RNWF_NET_TLS_CERT_NAME]);     
            if(tls_cfg_list[RNWF_NET_TLS_KEY_NAME] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_TLS_SET_KEY_NAME, request, tls_cfg_list[RNWF_NET_TLS_KEY_NAME]);     
            if(tls_cfg_list[RNWF_NET_TLS_KEY_PWD] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_TLS_SET_KEY_PWD, request, tls_cfg_list[RNWF_NET_TLS_KEY_PWD]);     
            if(tls_cfg_list[RNWF_NET_TLS_SERVER_NAME] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_TLS_SERVER_NAME, request, tls_cfg_list[RNWF_NET_TLS_SERVER_NAME]);
            if(tls_cfg_list[RNWF_NET_TLS_DOMAIN_NAME] != NULL)
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_TLS_DOMAIN_NAME, request, tls_cfg_list[RNWF_NET_TLS_DOMAIN_NAME]);
            
            break;
        }
        case RNWF_NET_SOCK_SET_CALLBACK:
            gSocket_CallBack_Handler[1] = (RNWF_NET_SOCK_CALLBACK_t)(input);                        
            break;
        
        case RNWF_NET_SOCK_SET_SRVC_CALLBACK:
            gSocket_CallBack_Handler[0] = (RNWF_NET_SOCK_CALLBACK_t)(input);                        
            break;            
        default:
            result = RNWF_FAIL;
            break;
    }        
    return result;
}

RNWF_RESULT_t RNWF_NET_TCP_SOCK_Write( uint32_t socket, uint16_t length, uint8_t *input)  {
    
    RNWF_RESULT_t result = RNWF_FAIL;
                
    if((result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_BINARY_WRITE_TCP, socket, length)) == RNWF_RAW)     
    {                              
        result = RNWF_RAW_Write(input, length);         
    }    
    return result;
}
    
RNWF_RESULT_t RNWF_NET_UDP_SOCK_Write( uint32_t socket, uint8_t *addr, uint32_t port, uint16_t length, uint8_t *input)  {
    
    RNWF_RESULT_t result = RNWF_FAIL;
                         
    if((result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_ASCII_WRITE_UDP, socket, addr, port, length, input)) == RNWF_RAW)     
    {                                                            
        result = RNWF_RAW_Write(input, length);         
    }
    
    return result;
}

   

int16_t RNWF_NET_SOCK_Read( uint32_t socket, uint16_t length, uint8_t *buffer)  {                
    int16_t result = RNWF_FAIL;
                                        
    if(RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SOCK_READ, socket, RNWF_BINARY_MODE, length) == RNWF_RAW)
    {
        result = RNWF_RAW_Read(buffer, length);
    }    
    return result;
}


int16_t RNWF_NET_TCP_SOCK_Read( uint32_t socket, uint16_t length, uint8_t *buffer)  {       
    return RNWF_NET_SOCK_Read(socket, length, buffer);
}


int16_t RNWF_NET_UDP_SOCK_Read( uint32_t socket, uint16_t length, uint8_t *buffer)  {                   
    return RNWF_NET_SOCK_Read(socket, length, buffer);
}
/* *****************************************************************************
 End of File
 */
