/**
 * RNWFxx System service implementation source file
 * 
 * @file rnwf_system_service.c
 *
 * @ingroup system_service
 * 
 * @brief This file contains APIs and data types for System service
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
#include "rnwf_system_service.h"

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

RNWF_RESULT_t RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SERVICE_t request, void *input) {
    RNWF_RESULT_t result = RNWF_FAIL;
    switch(request)
    {
        case RNWF_SYSTEM_RESET:
        {
            uint8_t init_msg[128];
            result = RNWF_CMD_SEND_OK_WAIT(NULL, init_msg, RNWF_RESET_TARGET);
        }
        break;
        case RNWF_SYSTEM_ECHO_OFF:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_ECHO_OFF);
        }
        break;case RNWF_SYSTEM_GET_MAN_ID:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+GMI:", input, RNWF_GET_MAN_ID);
        }
        break;
        case RNWF_SYSTEM_SW_REV:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+GMR:", input, RNWF_GET_REV_ID);
        }
        break;
        case RNWF_SYSTEM_DEV_INFO:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+DI:", input, RNWF_GET_DEV_INFO);
        }
        break;
        case RNWF_SYSTEM_GET_CERT_LIST:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+FS:2,1,", input, RNWF_GET_CERT_LIST);
        }
        break;
        case RNWF_SYSTEM_GET_KEY_LIST:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+FS:2,2,", input, RNWF_GET_KEY_LIST);
        }
        break;
        case RNWF_SYSTEM_GET_TIME:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_GET_SYS_TIME);
        }
        break;
        case RNWF_SYSTEM_SET_TIME_UNIX:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SYS_TIME_UNIX, *(uint32_t*)input);
        }
        break;
        case RNWF_SYSTEM_SET_TIME_NTP:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SYS_TIME_NTP, *(uint32_t*)input);
        }
        break;
        case RNWF_SYSTEM_SET_TIME_STRING:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SYS_TIME_STRING, (uint8_t*)input);
        }
        break;
        case RWWF_SYSTEM_GET_WIFI_INFO:
        {
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+NETIFC:", input, RNWF_GET_WIFI_IFNO);
        }
        break;
 
        case RNWF_SYSTEM_SET_SNTP:
        {         
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SNTP_STATIC, input);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SNTP_SVR_ADDR, input);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_SNTP_ENBL, input);
        }
        break;
        case RNWF_SYSTEM_SET_DNS:
        {
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_DNS_TIMEOUT, 5000);
            result = RNWF_CMD_SEND_OK_WAIT(NULL, NULL, RNWF_SET_DNS_AUTO, NULL);
        }
        break;
         
        case RNWF_SYSTEM_GET_MQTT_INFO:
        {            
            *(uint8_t*)input = '\0';
            result = RNWF_CMD_SEND_OK_WAIT("+MQTTC:", input, RNWF_GET_MQTT_IFNO);
        }
        break;
        default:
            break;
    }
    return result;
}

/* *****************************************************************************
 End of File
 */
