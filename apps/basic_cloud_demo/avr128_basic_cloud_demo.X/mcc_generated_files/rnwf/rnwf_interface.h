/**
 * INF Service header file
 * 
 * @file rnwf_interface.h
 * 
 * @defgroup rnwf_interface Wi-Fi Interface
 *
 * @brief This header file contains data types for interface
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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_INTERFACE_H
#define	RNWF_INTERFACE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include "../uart/uart_drv_interface.h"


#define DBG_MSG_IF(args, ...)    printf("[IF]:"args, ##__VA_ARGS__)
/**
 * @ingroup rnwf_interface
 * @brief RNWF Return Values
 * @enum RNWF_RESULT_t
*/
typedef enum
{      
    RNWF_PASS =  0x0000,     /**<Success*/
    RNWF_FAIL =  -1,     /**<Failure*/            
    RNWF_RAW = -2,       /**<RAW mode*/
    RNWF_COTN =  -3,     /**<Retry*/                 
    RNWF_BUSY = -4,     /**<Busy*/       
    RNWF_TIMEOUT = -5,  /**<Timeout*/                 
}RNWF_RESULT_t;

/**
 * @ingroup rnwf_interface
 * @brief   RNWF interface states
 * @enum    RNWF_INTERFACE_STATE_t
*/
typedef enum
{           
    RNWF_INTERFACE_FREE,
    RNWF_INTERFACE_BUSY
}RNWF_INTERFACE_STATE_t;

extern const uart_drv_interface_t UART2;


#define MQTT_APP

 

#if defined(MQTT_APP) || defined(MOBILE_APP_PROV)
#define RNWF_INTERFACE_LEN_MAX    1024

#define RNWF_IF_ASYCN_MSG_MAX  (512+256)
#define RNWF_IF_ASYCN_MSG_CNT  2
#define RNWF_IF_ASYCN_BUF_MAX  RNWF_IF_ASYCN_MSG_MAX*RNWF_IF_ASYCN_MSG_CNT
#else
#define RNWF_INTERFACE_LEN_MAX    512
#define RNWF_IF_ASYCN_BUF_MAX  1024
#define RNWF_IF_ASYCN_MSG_MAX  64
#endif

#define RNWF_IF_BUF_MAX     (RNWF_IF_ASYCN_BUF_MAX/RNWF_IF_ASYCN_MSG_MAX)

extern RNWF_INTERFACE_STATE_t   g_interface_state;

extern uint32_t   g_interface_timeout;



#define RNWF_INTERFACE_TIMEOUT      0x7FFFF

 

 

#define RNWF_IS_INTERFACE_BUSY()      if(g_interface_state == RNWF_INTERFACE_BUSY){printf("IF Busy\n");return RNWF_INTERFACE_BUSY;}
#define RNWF_SET_INTERFACE_BUSY()     (g_interface_state = RNWF_INTERFACE_BUSY)             
#define RNWF_SET_INTERFACE_FREE()     (g_interface_state = RNWF_INTERFACE_FREE)         

#define RNWF_AT_CMD     "AT"
#define RNWF_AT_EOL     "\r\n>"
#define RNWF_AT_DONE    "OK"
#define RNWF_AT_ERROR     "ERROR"

#define RNWF_ARG_DELIMETER          ":"

/*  Wi-Fi Event Code*/
#define RNWF_EVENT_STA_AUTO_IP        "WSTAAIP:"
#define RNWF_EVENT_AP_AUTO_IP         "WAPAIP:"

#define RNWF_EVENT_LINK_UP        "WSTALU:"
#define RNWF_EVENT_LINK_LOSS      "WSTALD:"
#define RNWF_EVENT_ERROR          "WSTAERR:"

/*  SCAN Event Code*/
#define RNWF_EVENT_SCAN_IND       "WSCNIND:"
#define RNWF_EVENT_SCAN_DONE      "WSCNDONE:"

/*  DHCP Event Code*/
#define RNWF_EVENT_DHCP_CFG       "DHCPSC:"

/*  DNS Event Code */
#define RNWF_EVENT_DNS_RESOLVE    "DNSRESOLV:"
#define RNWF_EVENT_DNS_ERROR      "DNSERR:"

/* feature addition from FW v2.0.0*/
/* Regulatory Domain Event Code */
#define RNWF_EVENT_GET_REGDOM     "WIFIC:"

/* PING Event Codes */
#define RNWF_EVENT_PING_FAILURE     "PINGERR:"
#define RNWF_EVENT_PING_SUCCESS     "PING:"
 

/*  TIME Event Code */
#define RNWF_EVENT_TIME             "TIME:"

/*  INFO Event Code */
#define RNWF_EVENT_INFO           "INFO:"

/* SOCKET Event Code */
#define RNWF_EVENT_SOCK_CONNECTED   "SOCKIND:"
#define RNWF_EVENT_SOCK_TLS_DONE    "SOCKTLS:"
#define RNWF_EVENT_SOCK_TCP_RECV    "SOCKRXT:"
#define RNWF_EVENT_SOCK_UDP_RECV    "SOCKRXU:"
#define RNWF_EVENT_SOCK_CLOSE       "SOCKCL:"
#define RNWF_EVENT_SOCK_TLS_SUCCESS "SOCKTLS:"
#define RNWF_EVENT_SOCK_ERROR       "SOCKERR:"


#define RNWF_EVENT_MQTT_CONNECTED   "MQTTCONN:"
#define RNWF_EVENT_MQTT_CONN_ACKED  "MQTTCONNACK:"
#define RNWF_EVENT_MQTT_PUB_ACKED   "MQTTPUBACK:"
#define RNWF_EVENT_MQTT_PUB_ERR     "MQTTPUBERR:"
#define RNWF_EVENT_MQTT_PUB_COMPLT  "MQTTPUBCOMP:"
#define RNWF_EVENT_MQTT_SUB_RESP    "MQTTSUB:"
#define RNWF_EVENT_MQTT_UNSUB_RESP  "MQTTUNSUB:"
#define RNWF_EVENT_MQTT_PROP_RX     "MQTTPROPRX:"
#define RNWF_EVENT_MQTT_SUB_MSG     "MQTTSUBRX:"

/**
 * @ingroup     rnwf_interface
 * @brief       Buffer queue
 * @struct      IF_QUEUE_t
*/
typedef struct {
        int8_t    head; 
        int8_t    tail;             
        uint32_t  queue[RNWF_IF_BUF_MAX];
        uint8_t    size;
}IF_QUEUE_t;

/**
 * @ingroup rnwf_interface
 * @brief   DHCP IP Address type
 * @enum    RNWF_DHCP_IP_ADD_TYPE_t
*/
typedef enum {
    RNWF_IP_UNKNOWN = 1,            /**<RNWF DHCP IP address error code*/
    RNWF_IPv4,                      /**<RNWF DHCP IP type IPv4*/
    RNWF_LINK_LOCAL_IPv6,           /**<RNWF DHCP IP type Link-local IPv6*/
    RNWF_GLOBAL_IPv6,               /**<RNWF DHCP IP type Global IPv6*/    
}RNWF_DHCP_IP_ADD_TYPE_t;
 

#define IF_BUF_Q_ENQUEUE(frameIdx)   if_q_enqueue(&g_if_free_q, frameIdx) 
#define IF_BUF_Q_DEQUEUE(frameIdx)   if_q_dequeue(&g_if_free_q, frameIdx) 

#define IF_RX_Q_ENQUEUE(frameIdx)   if_q_enqueue(&g_if_rx_q, frameIdx) 
#define IF_RX_Q_DEQUEUE(frameIdx)   if_q_dequeue(&g_if_rx_q, frameIdx) 

/**
 * @ingroup     rnwf_interface
 * @brief       Initializing interface buffer queue
 * @param[in]   None
 * @retval      RNWF_PASS Requested service handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
RNWF_RESULT_t RNWF_IF_Init(void);

/**
 * @ingroup     rnwf_interface
 * @brief       Write the bytes to buffer
 * @param[in]   buffer    Pointer to the buffer
 * @param[in]   len       Length of the data to write
 * @retval      RNWF_PASS Requested service handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
RNWF_RESULT_t RNWF_RAW_Write(uint8_t *buffer, uint16_t len);

/**
 * @ingroup     rnwf_interface
 * @brief       Read from buffer
 * @param[in]   buffer    Pointer to the buffer
 * @param[in]   len       Length of the data read
 * @return      RNWF data length that was read successfully
 * @return      RNWF_TIMEOUT when fails to read successfully
*/
int16_t RNWF_RAW_Read(uint8_t *buffer, uint16_t len);

/**
 * @ingroup     rnwf_interface
 * @brief       Send the command to RNWF device
 * @param[in]   cmd_complete  AT command's response to wait for
 * @param[in]   delimeter     delimeter
 * @param[in]   response      AT command in form of string
 * @param[in]   format        (optional)parameters or arguments of the AT command to send
 * @return      RNWF response (timeout, pass or response length)
*/
int16_t RNWF_CMD_RSP_Send(const char *cmd_complete, const char *delimeter, uint8_t *response, const char *format, ...);

/**
 * @ingroup     rnwf_interface
 * @brief       Event handler running in loop to capture asynchronous message response
 * @param[in]   None
 * @retval      RNWF_PASS Requested service handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
RNWF_RESULT_t RNWF_EVENT_Handler(void);

/**
 * @ingroup     rnwf_interface
 * @brief       Software reset API
 * @param[in]   None
 * @return      RNWF return values
*/
RNWF_RESULT_t RNWF_IF_SW_Reset(void);

/**
 * @ingroup     rnwf_interface
 * @brief       Trimming tab or null character from asynchronous response
 * @param[in]   buffer Pointer to input buffer conatining data
 * @retval      RNWF_PASS Requested service handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
RNWF_RESULT_t RNWF_RESPONSE_Trim(uint8_t *buffer);

/**
 * @ingroup     rnwf_interface
 * @brief       Write bytes to buffer
 * @param[in]   buffer    Pointer to the buffer
 * @param[in]   len       Length of the data to write
 * @retval      RNWF_PASS Requested service handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
uint16_t RNWF_IF_Write(uint8_t *buffer, uint16_t len);

/**
 * @ingroup     rnwf_interface
 * @brief       Read from buffer
 * @param[in]   buffer    Pointer to the buffer
 * @param[in]   len       Length of the data read
 * @retval RNWF data length that was read successfully
 * @retval RNWF_TIMEOUT when fails to read successfully
*/
uint16_t RNWF_IF_Read(uint8_t *buffer, uint16_t len);
                  
#define RNWF_CMD_SEND_OK_WAIT(delimeter, response, format, ...) RNWF_CMD_RSP_Send(RNWF_AT_DONE, delimeter, response, format, ##__VA_ARGS__)
#define RNWF_CMD_SEND_RESP_WAIT(cmd_complete, delimeter, response, format, ...) RNWF_CMD_RSP_Send(cmd_complete, delimeter, response, format, __VA_ARGS__)

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* RNWF_INTERFACE_H */
