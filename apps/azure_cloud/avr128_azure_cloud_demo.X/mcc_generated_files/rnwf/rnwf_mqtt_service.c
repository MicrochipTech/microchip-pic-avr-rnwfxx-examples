/**
 * MQTT Service source file
 * 
 * @file rnwf_mqtt_service.c
 * 
 * @ingroup mqtt_service
 * 
 * @brief This file contains APIs and data types for MQTT service
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

#include "../timer/delay.h"

#include "rnwf_interface.h"
#include "rnwf_mqtt_service.h"
#include "rnwf_net_service.h"
#include "rnwf_system_service.h"



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
RNWF_MQTT_CALLBACK_t gMqtt_CallBack_Handler[RNWF_MQTT_SERVICE_CB_MAX] = {NULL, NULL};


  

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

RNWF_RESULT_t RNWF_MQTT_SrvCtrl( RNWF_MQTT_SERVICE_t request, void *input)  
{
RNWF_RESULT_t result = RNWF_FAIL;

    switch(request)
    {
        case RNWF_MQTT_CONFIG:
        {
            RNWF_MQTT_CFG_t *mqtt_cfg = (RNWF_MQTT_CFG_t *)input;  
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_PROTO_VER, 3);
            if(mqtt_cfg->tls_idx != 0)
            {
                    result = RNWF_NET_SOCK_SrvCtrl(mqtt_cfg->tls_idx, mqtt_cfg->tls_conf);
                result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_TLS_CONF, mqtt_cfg->tls_idx);                             
            }
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_BROKER_URL, mqtt_cfg->url);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_BROKER_PORT, mqtt_cfg->port);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_CLIENT_ID, mqtt_cfg->clientid);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_USERNAME, mqtt_cfg->username);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_PASSWORD, mqtt_cfg->password);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_KEEPALIVE, 60);
            break;
        }           
        case RNWF_MQTT_CONNECT:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_DISCONNECT);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_CONNECT);     
        }
            break;
        case RNWF_MQTT_RECONNECT:
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_RECONNECT);     
            break;            
        case RNWF_MQTT_DISCONNECT:
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_DISCONNECT);     
            break;
        
        case RNWF_MQTT_PUBLISH:
        {
            RNWF_MQTT_FRAME_t *mqtt_frame = (RNWF_MQTT_FRAME_t *)input;
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_PUBLISH, mqtt_frame->isNew, mqtt_frame->qos, mqtt_frame->isRetain, mqtt_frame->topic, mqtt_frame->message);     
        }
        break;            
        
        case RNWF_MQTT_SUBSCRIBE_QOS0:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_SUBSCRIBE_QOS0, (const char *)input);
        }
        break;
        case RNWF_MQTT_SUBSCRIBE_QOS1:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_SUBSCRIBE_QOS1, (const char *)input);
        }
        break;
        case RNWF_MQTT_SUBSCRIBE_QOS2:
        {            
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_CMD_SUBSCRIBE_QOS2, (const char *)input);     
        }        
        break;            
        
        case RNWF_MQTT_SET_CALLBACK:
        {
            gMqtt_CallBack_Handler[1] = (RNWF_MQTT_CALLBACK_t)(input);            
            result = RNWF_PASS;
        }
        break;
        default:
            break;
        
    }
    return result;
}
