/**
 * RNWFxx System service header file
 * 
 * @file rnwf_system_service.h
 * 
 * @defgroup system_service System Service
 * 
 * @brief This header file contains data types for System service
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
#ifndef RNWF_SYSTEM_SERVICE_H
#define	RNWF_SYSTEM_SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define RNWF_SET_ECHO_OFF       "ATE0\r\n"

#define RNWF_RESET_TARGET       "AT+RST\r\n"
#define RNWF_GET_MAN_ID         "AT+GMI\r\n"
#define RNWF_GET_MOD_ID         "AT+GMI\r\n"
#define RNWF_GET_REV_ID         "AT+GMR\r\n"
#define RNWF_GET_DEV_INFO       "AT+DI\r\n"

/* feature addition from FW v2.0.0*/
#define RNWF_SET_SNTP_DISABLE   "AT+SNTPC=1,0\r\n"
 
#define RNWF_SET_SNTP_ENBL      "AT+SNTPC=1,1\r\n"
#define RNWF_SET_SNTP_STATIC    "AT+SNTPC=2,1\r\n"
#define RNWF_SET_SNTP_DHCP      "AT+SNTPC=2,0\r\n"
/* feature addition from FW v2.0.0*/
#define RNWF_SET_SNTP_SVR_ADDR  "AT+SNTPC=3,\"%s\"\r\n"
 
 

/* feature addition from FW v2.0.0*/ 
#define RNWF_SET_DNS_AUTO       "AT+DNSC=2,1\r\n"
#define RNWF_SET_DNS_MANUAL     "AT+DNSC=2,0\r\n"
#define RNWF_SET_DNS_SERVER_ADD "AT+DNSC=1,\"%s\"\r\n"
#define RNWF_SET_DNS_TIMEOUT    "AT+DNSC=3,%u\r\n"
 

#define RNWF_GET_SYS_TIME       "AT+TIME,3\r\n"

#define RNWF_SET_SYS_TIME_UNIX  "AT+TIME=1,%lu\r\n"
#define RNWF_SET_SYS_TIME_NTP   "AT+TIME=2,%lu\r\n"
/* Date/time in format YYYY-MM-DDTHH:MM:SS.00Z */ 
#define RNWF_SET_SYS_TIME_STRING    "AT+TIME=3,\"%s\"\r\n"

#define RNWF_GET_CERT_LIST      "AT+FS=2,1\r\n"
#define RNWF_GET_KEY_LIST       "AT+FS=2,2\r\n"

#define RNWF_GET_WIFI_IFNO     "AT+NETIFC=0\r\n"

#define RNWF_GET_MQTT_IFNO     "AT+MQTTC\r\n"


/**
 * @ingroup     system_service
 * @brief       System Service List
 * @enum        RNWF_SYSTEM_SERVICE_t
 */
typedef enum{
    RNWF_SYSTEM_RESET,             /**<Request/Trigger reset the system */
    RNWF_SYSTEM_SW_REV,            /**<Request Software Revision */
    RNWF_SYSTEM_DEV_INFO,          /**<Request Software Revision */            
    RNWF_SYSTEM_ECHO_OFF,          /**<Request/Trigger reset the system */            
    RNWF_SYSTEM_GET_MAN_ID,        /**<Get the manufacturing ID */
    RNWF_SYSTEM_SET_SNTP,          /**<Enable SNTP */
    RNWF_SYSTEM_RMV_SNTP,          /**<Disable SNTP */
    RNWF_SYSTEM_SET_DNS,           /**<Configure DNS */
     
    RNWF_SYSTEM_SET_TIME_UNIX,     /**<Set the sytem time in UNIX format */            
    RNWF_SYSTEM_SET_TIME_NTP,      /**<Set the system time in NTP format */            
    RNWF_SYSTEM_SET_TIME_STRING,   /**<Set the system time in string(YYYY-MM-DDTHH:MM:SS.00Z) format */                        
    RNWF_SYSTEM_GET_TIME,          /**<Get the system time */            
    RNWF_SYSTEM_GET_CERT_LIST,     /**<Get the available certificate list */
    RNWF_SYSTEM_GET_KEY_LIST,      /**<Get the available private key list */
    RWWF_SYSTEM_GET_WIFI_INFO,
    RNWF_SYSTEM_GET_MQTT_INFO,                
}RNWF_SYSTEM_SERVICE_t;


/**
 * @ingroup     system_service
 * @brief       System Service Layer API to handle system operations
 * @param[in]   request   Service request of type RNWF_SYSTEM_SERVICE_t
 * @param[in]   input     Parameters for the requested service
 * @retval      RNWF_PASS Requested service is handled successfully
 * @retval      RNWF_FAIL Requested service has failed
 */
RNWF_RESULT_t RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SERVICE_t request, void *input);

#endif	/* RNWF_SYSTEM_SERVICE_H */

/** @}*/
