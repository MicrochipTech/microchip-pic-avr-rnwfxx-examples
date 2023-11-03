/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_mqtt_service.c
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
#include <string.h>

#include "../timer/delay.h"

#include "rnwf_interface.h"
#include "rnwf_mqtt_service.h"
#include "rnwf_net_service.h"
#include "rnwf_system_service.h"


#define MQTT_DPS_TOP_SET_REG        "$dps/registrations/PUT/iotdps-register/?$rid=1"
#define MQTT_DPS_MSG_SET_REQ        "{\\\"payload\\\": {\\\"modelId\\\": \\\"dtmi:com:Microchip:AVR128DB48_CNANO;1\\\"}}"

#define MQTT_DPS_TOP_DPS_GET_STAT   "$dps/registrations/GET/iotdps-get-operationstatus/?$rid=2&operationId=%s"
#define MQTT_DPS_MSG_DPS_GET_STAT   ""

#define MQTT_DPS_SUBSCRIBE_TOPIC    "$dps/registrations/res/#"

#define MQTT_DPS_HUB_ID_STR         "\\\"assignedHub\\\":\\"""
#define MQTT_DPS_DEV_ID_STR         "\\\"deviceId\\\":\\"""        
#define MQTT_DPS_OP_ID_STR          "\\\"operationId\\\":\\"""
#define MQTT_DPS_END_ID_STR         "\\\" \\\""


#define MQTT_IOT_HUB_USERNAME       "%s/%s/?api-version=2021-04-12"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
RNWF_MQTT_CALLBACK_t gMqtt_CallBack_Handler[RNWF_MQTT_SERVICE_CB_MAX] = {NULL, NULL};


  
RNWF_RESULT_t RNWF_MQTT_SRVC_Callback(RNWF_MQTT_EVENT_t event, uint8_t *p_str)
{    
    RNWF_RESULT_t result = RNWF_COTN;
    static uint8_t dps_result = 0;
    uint8_t tmpBuf[256];    
    switch(event)
    {
        case RNWF_MQTT_CONNECTED:
        {
            const char sub_topic[] = MQTT_DPS_SUBSCRIBE_TOPIC;
            DBG_MSG_MQTT("Azure Central Connection Successful!\r\n");
            DBG_MSG_MQTT("Performing Device Provisioning Service(DPS)...\r\n");
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, (void *)sub_topic);             
        }
        break;
        case RNWF_MQTT_SUBCRIBE_ACK:
        {
            RNWF_MQTT_FRAME_t mqtt_pub;              
            mqtt_pub.isNew = NEW_MSG;
            mqtt_pub.qos = MQTT_QOS0;
            mqtt_pub.isRetain = NO_RETAIN;
            mqtt_pub.topic = MQTT_DPS_TOP_SET_REG;
            mqtt_pub.message = MQTT_DPS_MSG_SET_REQ;        
            result = RNWF_MQTT_SrvCtrl(RNWF_MQTT_PUBLISH, (void *)&mqtt_pub);
        }
        break;
        case RNWF_MQTT_SUBCRIBE_MSG:
        {
            
            uint8_t *pDevId = NULL, *pIotId = NULL, *pOpId = NULL, *pTmp;
            if((pIotId = (uint8_t *)strstr((const char *)p_str, MQTT_DPS_HUB_ID_STR)) != NULL)
            {               
                if((pDevId = (uint8_t *)strstr((const char *)p_str, MQTT_DPS_DEV_ID_STR)) != NULL)
                {
                    if((pTmp = (uint8_t *)strstr((const char *)pDevId, MQTT_DPS_END_ID_STR)) != NULL )
                    {
                        *pTmp = '\0';                                                
                        if((pTmp = (uint8_t *)strstr((const char *)pIotId, MQTT_DPS_END_ID_STR)) != NULL)
                        {
                            *pTmp = '\0';
                            sprintf((char *)tmpBuf, MQTT_IOT_HUB_USERNAME, pIotId+strlen(MQTT_DPS_HUB_ID_STR)+1, pDevId+strlen(MQTT_DPS_DEV_ID_STR)+1);                                                                            
                            
                            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_USERNAME, tmpBuf);
                            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_MQTT_SET_BROKER_URL, pIotId+strlen(MQTT_DPS_HUB_ID_STR)+1);                             
                            RNWF_MQTT_SrvCtrl(RNWF_MQTT_DISCONNECT, (void *)NULL);
                            dps_result = 1;
                        }                                                
                    }
                }                                
            }
            else if((pOpId = (uint8_t *)strstr((const char *)p_str, MQTT_DPS_OP_ID_STR)) != NULL)
            {
                RNWF_MQTT_FRAME_t mqtt_pub;                                               
                if((pTmp = (uint8_t *)strstr((const char *)pOpId, MQTT_DPS_END_ID_STR)) != NULL )
                {
                    *pTmp = '\0';
                    sprintf(tmpBuf, MQTT_DPS_TOP_DPS_GET_STAT, pOpId+strlen(MQTT_DPS_OP_ID_STR)+1);
                    DELAY_milliseconds(1500);
//                    DBG_MSG_MQTT("Register Topic = %s\r\n", tmpBuf);
                    mqtt_pub.isNew = NEW_MSG;
                    mqtt_pub.qos = MQTT_QOS0;
                    mqtt_pub.isRetain = NO_RETAIN;
                    mqtt_pub.topic = tmpBuf;                
                    mqtt_pub.message = "";        
                    result = RNWF_MQTT_SrvCtrl(RNWF_MQTT_PUBLISH, (void *)&mqtt_pub);
                }
            }            
        }
        break;
        case RNWF_MQTT_DISCONNECTED:
        {   
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_SET_SRVC_CALLBACK, (void *)NULL);            
            if(gMqtt_CallBack_Handler[1] != NULL)
            {                            
                gMqtt_CallBack_Handler[1](RNWF_MQTT_DPS_STATUS, &dps_result);
            }
//
//            DBG_MSG_MQTT("Disconnected!\r\nReconnecting...\r\n");            
//            RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);            
        }
        break;        
        default:
        break;
    }
    
    return RNWF_COTN; // No need to invoke APP callback
}

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
            if(mqtt_cfg->azure_dps)
            {
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_SET_SRVC_CALLBACK, (void *)RNWF_MQTT_SRVC_Callback);
            }
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
        case RNWF_MQTT_SET_SRVC_CALLBACK:        
        {
            gMqtt_CallBack_Handler[0] = (RNWF_MQTT_CALLBACK_t)(input);            
            result = RNWF_PASS;
        }
        break;
        default:
            break;
        
    }
    return result;
}
