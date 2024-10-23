/**
 * MQTT Service header file
 * 
 * @file rnwf_mqtt_service.h
 * 
 * @defgroup mqtt_service MQTT Service 
 *
 * @brief This header file contains data types for MQTT Service
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


/*! \page mqtt_service MQTT Service
This page is for advanced users.
*/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_MQTT_SERVICE_H
#define	XC_MQTT_SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#define RNWF_MQTT_DEBUG     1

#ifdef RNWF_MQTT_DEBUG
    #define DBG_MSG_MQTT(args, ...)      printf("[MQTT]:"args, ##__VA_ARGS__)
#endif
 

/* MQTT Configuration Commands */
#define RNWF_MQTT_SET_BROKER_URL    "AT+MQTTC=1,\"%s\"\r\n"
#define RNWF_MQTT_SET_BROKER_PORT   "AT+MQTTC=2,%d\r\n"
#define RNWF_MQTT_SET_CLIENT_ID     "AT+MQTTC=3,\"%s\"\r\n"
#define RNWF_MQTT_SET_USERNAME      "AT+MQTTC=4,\"%s\"\r\n"
#define RNWF_MQTT_SET_PASSWORD      "AT+MQTTC=5,\"%s\"\r\n"
#define RNWF_MQTT_SET_KEEPALIVE     "AT+MQTTC=6,%d\r\n"
#define RNWF_MQTT_SET_TLS_CONF      "AT+MQTTC=7,%d\r\n"
#define RNWF_MQTT_SET_PROTO_VER     "AT+MQTTC=8,%d\r\n"

/* MQTT Connection Commands */
#define RNWF_MQTT_CMD_CONNECT      "AT+MQTTCONN=1\r\n"
#define RNWF_MQTT_CMD_RECONNECT    "AT+MQTTCONN=0\r\n"

/* MQTT Disconnection Commands */
#define RNWF_MQTT_CMD_DISCONNECT        "AT+MQTTDISCONN=0\r\n"

/* MQTT Subscribe Commands */
#define RNWF_MQTT_CMD_SUBSCRIBE_QOS0    "AT+MQTTSUB=\"%s\",0\r\n"
#define RNWF_MQTT_CMD_SUBSCRIBE_QOS1    "AT+MQTTSUB=\"%s\",1\r\n"
#define RNWF_MQTT_CMD_SUBSCRIBE_QOS2    "AT+MQTTSUB=\"%s\",2\r\n"
#define RNWF_MQTT_CMD_UNSUBSCRIBE       "AT+MQTTUNSUB=%s\r\n"

/* MQTT Publish Commands */
#define RNWF_MQTT_CMD_PUBLISH           "AT+MQTTPUB=%d,%d,%d,\"%s\",\"%s\"\r\n"

/* MQTT Transmit Properties  Commands */
#define RNWF_MQTT_SET_TX_SESSION_EXPIRY     "AT+MQTTPROPTX=17,%d\r\n"
#define RNWF_MQTT_SET_TX_USER_PROP          "AT+MQTTPROPTX=38,%s\r\n"

/* MQTT Receive Properties  Commands */
#define RNWF_MQTT_SET_RX_SESSION_EXPIRY     "AT+MQTTPROPRX=17,%d\r\n"
#define RNWF_MQTT_SET_RX_TOP_ALIAS_MAX      "AT+MQTTPROPRX=34,%s\r\n"
#define RNWF_MQTT_SET_RX_TOP_USER_PROP      "AT+MQTTPROPRX=38,%s\r\n"


#define RNWF_MQTT_CLR_TX_PROP_ID            "AT+MQTTPROPTXS=%d,0"
#define RNWF_MQTT_SET_TX_PROP_ID            "AT+MQTTPROPTXS=%d,1"


#define MQTT_PORT_NO_ECN_NO_AUTH        1883
#define MQTT_PORT_NO_ECN_AUTH           1884
#define MQTT_PORT_ECN_NO_AUTH           8883


#define RNWF_MQTT_SERVICE_CB_MAX        2

/**
 * @ingroup mqtt_service
 * @brief   MQTT service status enum
 * @enum    RNWF_MQTT_SERVICE_t
*/
typedef enum 
{
    RNWF_MQTT_CONFIG,               /**<Configure the MQTT Broker parameters*/
    RNWF_MQTT_CONNECT,              /**<Connect to the MQTT Broker */
    RNWF_MQTT_RECONNECT,            /**<Request reconnect to the MQTT Cloud*/
    RNWF_MQTT_DISCONNECT,           /**<Trigger Disconnect from MQTT Broker*/
    RNWF_MQTT_SUBSCRIBE_QOS0,       /**<Subscribe to QoS0 Topics */
    RNWF_MQTT_SUBSCRIBE_QOS1,       /**<Subscribe to QoS1 Topics */
    RNWF_MQTT_SUBSCRIBE_QOS2,       /**<Subscribe to QoS2 Topics */
    RNWF_MQTT_PUBLISH,              /**<Publis to MQTT Broker*/
    RNWF_MQTT_SET_CALLBACK,         /**<Configure the MQTT Application Callback*/              
    RNWF_MQTT_SET_SRVC_CALLBACK,         /**<Configure the MQTT Application Callback*/                          
}RNWF_MQTT_SERVICE_t;


/**
 * @ingroup mqtt_service
 * @brief   MQTT Application callback events
 * @enum    RNWF_MQTT_EVENT_t
*/
typedef enum
{
    RNWF_MQTT_CONNECTED,    /**<Connected to MQTT broker event */
    RNWF_MQTT_DISCONNECTED, /**<Disconnected from MQTT broker event*/   
    RNWF_MQTT_SUBCRIBE_MSG,  /**<Event to report received MQTT message*/   
    RNWF_MQTT_SUBCRIBE_ACK,
    RNWF_MQTT_PUBLIC_ACK,
   
  
}RNWF_MQTT_EVENT_t;

/**
 * @ingroup     mqtt_service
 * @brief       MQTT message configuration
 * @struct      RNWF_MQTT_CFG_t
*/
typedef struct 
{    
    const char *url;            /**<MQTT Broker/Server URL */    
    const char *clientid;
    const char *username;       /**<MQTT User Name Credential */
    const char *password;       /**<MQTT Password Credential */ 
    uint16_t port;              /**<MQTT Broker/Server Port */
    uint8_t     tls_idx;
    uint8_t     azure_dps;
    uint8_t     *tls_conf;
}RNWF_MQTT_CFG_t;

/**
 * @ingroup mqtt_service
 * @brief   MQTT message
 * @enum    RNWF_MQTT_MSG_t
*/
typedef enum
{
    NEW_MSG,        /**New message*/ 
    DUP_MSG         /**Duplicate message*/ 
}RNWF_MQTT_MSG_t;

/**
 * @ingroup mqtt_service
 * @brief MQTT Message QoS Type
 * @enum RNWF_MQTT_QOS_t
*/
typedef enum
{
    MQTT_QOS0,      /**<No-Ack, Best effort delivery(No Guarantee)*/          
    MQTT_QOS1,      /**<Pub-Ack, sent untill PUBACK from broker(possible duplicates) */
    MQTT_QOS2,      /**<Highest service, no duplicate with guarantee */          
}RNWF_MQTT_QOS_t;

/**
 * @ingroup mqtt_service
 * @brief   MQTT Message Retain flag
 * @enum    RNWF_MQTT_RETAIN_t
*/
typedef enum
{
    NO_RETAIN,          /**<Publish message is not saved at broker */
    RETAIN,             /**<Publish message is saved at broker */
}RNWF_MQTT_RETAIN_t;

/**
 * @ingroup mqtt_service
 * @brief MQTT Publish frame format
 * @struct RNWF_MQTT_FRAME_t
*/
typedef struct
{
    RNWF_MQTT_MSG_t isNew;          /**<Indicates message is new or duplicate */
    RNWF_MQTT_QOS_t qos;            /**<QoS type for the message of type RNWF_MQTT_QOS_t */
    RNWF_MQTT_RETAIN_t isRetain;    /**<Retain flag for the publish message */
    const char *topic;              /**<Publish topic for the message */
    const char *message;            /**<Indicates message is new or duplicate */               
}RNWF_MQTT_FRAME_t;


/**
 * @ingroup     mqtt_service
 * @brief       MQTT Callback Function definition
 * @param[in]   RNWF_MQTT_EVENT_t MQTT callback event
 * @param[in]   input parameter for service
 * @retval      RNWF_PASS Requested service handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
typedef RNWF_RESULT_t (*RNWF_MQTT_CALLBACK_t)(RNWF_MQTT_EVENT_t, uint8_t *);


/**
 * @ingroup mqtt_service
 * @brief MQTT Callback Function handler
 * @var gMqtt_CallBack_Handler stores MQTT callbacks
*/
extern RNWF_MQTT_CALLBACK_t gMqtt_CallBack_Handler[RNWF_MQTT_SERVICE_CB_MAX];

/**
 * @ingroup     mqtt_service
 * @brief       MQTT Service Layer API to handle system operations.
 * @param[in]   request       Requested service of type RNWF_MQTT_SERVICE_t
 * @param[in]   input         Input/Output data for the requested service 
 * @retval      RNWF_PASS Requested service is handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
RNWF_RESULT_t RNWF_MQTT_SrvCtrl( RNWF_MQTT_SERVICE_t request, void *input);

#endif	/* XC_HEADER_TEMPLATE_H */

/** @}*/