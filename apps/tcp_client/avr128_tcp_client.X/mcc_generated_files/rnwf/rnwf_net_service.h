/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_net_service.h
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


/*! \page net_sock_service Network Socket Service
This page introduces the user to the topic.
*/


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_NET_SERVICE_H
#define	RNWF_NET_SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>


#define RNWF_NET_SOCK_SERVICE_CB_MAX    2

#define RNWF_SOCK_ID_LEN_MAX            8
#define RNWF_SOCK_ADDR_LEN_MAX          32
#define RNWF_SOCK_TLS_CFG_LEN_MAX       64


#define RNWF_DHCPS_ENABLE               "AT+DHCPSC=0,1,1\r\n"
#define RNWF_DHCPS_DISABLE              "AT+DHCPSC=0,1,0\r\n"
#define RNWF_DHCPS_POOL_START           "AT+DHCPSC=0,2,\"%s\"\r\n"

#define RNWF_DHCPS_SET_GW               "AT+DHCPSC=0,10,\"%s\"\r\n"

#define RNWF_NETIF_SET_IP               "AT+NETIFC=0,40,\"%s\"\r\n"

#define RNWF_SOCK_GET_LIST              "AT+SOCKLST\r\n"

#define RNWF_SOCK_OPEN_UDP              "AT+SOCKO=1,4\r\n"
#define RNWF_SOCK_OPEN_TCP              "AT+SOCKO=2,4\r\n"
#define RNWF_SOCK_OPEN_RESP             "+SOCKO:"

#define RNWF_SOCK_BIND_LOCAL            "AT+SOCKBL=%lu,%d\r\n"
#define RNWF_SOCK_BIND_REMOTE           "AT+SOCKBR=%lu,\"%s\",%d\r\n"
#define RNWF_SOCK_BIND_MCAST            "AT+SOCKBM=%lu,%s,%d\r\n"


#define RNWF_SOCK_BINARY_WRITE_TCP      "AT+SOCKWR=%lu,%u\r\n"
#define RNWF_SOCK_BINARY_WRITE_UDP      "AT+SOCKWRTO=%lu,%s,%d,%d\r\n"

#define RNWF_SOCK_ASCII_WRITE_TCP       "AT+SOCKWR=%d,%d,\"%.*s\"\r\n"
#define RNWF_SOCK_ASCII_WRITE_UDP       "AT+SOCKWRTO=%d,%s,%d,%d,\"%.*s\"\r\n"

#define RNWF_SOCK_READ_BUF              "AT+SOCKRDBUF=%lu,%d,%d\r\n"
#define RNWF_SOCK_READ                  "AT+SOCKRD=%lu,%d,%d\r\n"
#define RNWF_SOCK_READ_RESP             "+SOCKRD:"

#define RNWF_SOCK_CLOSE                 "AT+SOCKCL=%lu\r\n"

#define RNWF_SOCK_CONFIG_TLS            "AT+SOCKTLS=%lu,%d\r\n"
#define RNWF_SOCK_CONFIG_KEEPALIVE      "AT+SOCKC=%lu,1,%d\r\n"
#define RNWF_SOCK_CONFIG_NODELAY        "AT+SOCKC=%lu,2,%d\r\n"


#define RNWF_SOCK_TLS_GET_CONFIG        "AT+TLSC\r\n"
#define RNWF_SOCK_TLS_SET_CA_NAME       "AT+TLSC=%d,1,\"%s\"\r\n"
#define RNWF_SOCK_TLS_SET_CERT_NAME     "AT+TLSC=%d,2,\"%s\"\r\n"
#define RNWF_SOCK_TLS_SET_KEY_NAME      "AT+TLSC=%d,3,\"%s\"\r\n"
#define RNWF_SOCK_TLS_SET_KEY_PWD       "AT+TLSC=%d,4,\"%s\"\r\n"
#define RNWF_SOCK_TLS_SERVER_NAME       "AT+TLSC=%d,5,\"%s\"\r\n"
#define RNWF_SOCK_TLS_DOMAIN_NAME       "AT+TLSC=%d,6,\"%s\"\r\n"

/**
 @defgroup NETSOCK_GRP Network Socket API
 @{
 */

/**
 @brief Network Interfaces List
 
 */
typedef enum {
    RNWF_IF_WIFI,
}RNWF_NET_IFS_t;

/**
 @brief Network and Socket service List
 
 */
typedef enum {
    RNWF_NET_TLS_CONFIG_1 = 1,          /**<Use the TLS configuration 1*/
    RNWF_NET_TLS_CONFIG_2,              /**<Use the TLS configuration 2*/
    RNWF_NET_IF_CONFIG,                 /**<Network Interface configuration*/
    RNWF_NET_DHCP_SERVER_ENABLE,        /**<Enable the DHCP server */
    RNWF_NET_DHCP_SERVER_DISABLE,       /**<Disable the DHCP server */
    RNWF_NET_SOCK_TCP_OPEN,             /**<Open TCP Socket*/
    RNWF_NET_SOCK_UDP_OPEN,             /**<Open UDP Socket*/
    RNWF_NET_SOCK_CLOSE,                /**<Close the socket*/
    RNWF_NET_SOCK_CONFIG,               /**<Configurs the socket settings*/
    RNWF_NET_SOCK_SET_CALLBACK,         /**<Register application callback for sockets*/
    RNWF_NET_SOCK_SET_SRVC_CALLBACK,
}RNWF_NET_SOCK_SERVICE_t;



/**
 @brief Socket Type
 
 */
typedef enum    {    
    RNWF_SOCK_UDP = 1,                  /**<UDP Socket type*/
    RNWF_SOCK_TCP,                      /**<TCP Socket type*/
}RNWF_SOCK_TYPE_t;

/**
 @brief Socket Bind Type
 
 */
typedef enum    {    
    RNWF_BIND_LOCAL,                    /**<Bind as a server Socket*/
    RNWF_BIND_REMOTE,                   /**<Bind as a client Socket*/
    RNWF_BIND_MCAST,                    /**<Bind as a multicast Socket*/
    RNWF_BIND_NONE,
}RNWF_BIND_TYPE_t;

/**
 @brief Socket Type
 
 */
typedef enum    {       
    RNWF_ASCII_MODE = 1,                /**<ASCII Socket mode*/
    RNWF_BINARY_MODE,                   /**<Binary Socket mode*/
}RNWF_SOCK_RW_MODE_t;



/**
 @brief Network socket events
 
 */
typedef enum {
    RNWF_NET_SOCK_EVENT_CONNECTED,      /**<Socket connected event*/
    RNWF_NET_SOCK_EVENT_TLS_DONE,       /**<TLS handshake done event*/
    RNWF_NET_SOCK_EVENT_DISCONNECTED,   /**<Socket disonnected event*/
    RNWF_NET_SOCK_EVENT_READ,           /**<Socket data read event*/
    RNWF_NET_SOCK_EVENT_ERROR,          /**<Socket error event*/
    RNWF_NET_SOCK_EVENT_UNDEFINED,
}RNWF_NET_SOCK_EVENT_t;



typedef enum {
    RNWF_NET_TLS_CA_CERT = 0,          
    RNWF_NET_TLS_CERT_NAME,            
    RNWF_NET_TLS_KEY_NAME,
    RNWF_NET_TLS_KEY_PWD,
    RNWF_NET_TLS_SERVER_NAME,
    RNWF_NET_TLS_DOMAIN_NAME,
}RNWF_NET_TLS_CONFIG_ID_t;


/**
 @brief Socket data type
 
 */
typedef struct {    
    RNWF_BIND_TYPE_t    bind_type;          /**<Bind type of socket*/
    RNWF_SOCK_TYPE_t    sock_type;          /**<UDP or TCP socket type*/
    uint16_t            sock_port;          /**<Server or Client port number*/
    const char          *sock_addr;         /**<Socket Address (IPv4 Address)*/
    uint32_t            sock_master;        /**<Server Socket ID*/
    uint8_t             tls_conf;           /**<TLS configuration */
}RNWF_NET_SOCKET_t;


/**
 @brief Advanced Socket settings
 
 */
typedef struct {  
    uint32_t    sock_id;                /**<Socket ID */
    uint8_t     sock_keepalive;         /**<Keep-Alive option*/
    uint8_t     sock_nodelay;           /**<Socket NAGLE/NoDelay*/    
}RNWF_NET_SOCKET_CONFIG_t;


void RNWF_EXIT_RAW_Mode(void);

/**
 @brief Network socket events callback function type 
 
 */
typedef RNWF_RESULT_t (*RNWF_NET_SOCK_CALLBACK_t)(uint32_t sock, RNWF_NET_SOCK_EVENT_t, uint8_t *);


/**
 @brief Network socket events callback handler 
 
 */
extern RNWF_NET_SOCK_CALLBACK_t gSocket_CallBack_Handler[RNWF_NET_SOCK_SERVICE_CB_MAX];

/**
 * @brief NET Sock Service Layer API to handle system operations.
 * 
 *
 * @param[in] request       Requested service ::RNWF_NET_SOCK_SERVICE_t
 * @param[in] input         Input/Output data for the requested service 
 * 
 * @return ::RNWF_PASS Requested service is handled successfully
 * @return ::RNWF_FAIL Requested service has failed
 */
RNWF_RESULT_t RNWF_NET_SOCK_SrvCtrl( RNWF_NET_SOCK_SERVICE_t request, void *input);


/**
 * @brief NET Socket Write API to send data over TCP socket.
 * 
 *
 * @param[in] socket        Socket ID 
 * @param[in] length        Length of data to be read 
 * @param[in] input         Input buffer to read the data     
 * @param[in] mode          Write mode for the socket ::RNWF_SOCK_RW_MODE_t
 * 
 * @return ::RNWF_PASS Requested service is handled successfully
 * @return ::RNWF_FAIL Requested service has failed
 */
RNWF_RESULT_t RNWF_NET_TCP_SOCK_Write( uint32_t socket, uint16_t length, uint8_t *input);

/**
 * @brief NET Socket Write API to send data UDP socket.
 * 
 *
 * @param[in] socket        Socket ID 
 * @param[in] addr          IP address of the UDP peer
 * @param[in] port          Port address of the UDP peer
 * @param[in] length        Length of data to be read 
 * @param[in] input         Input buffer to read the data     

 * 
 * @return ::RNWF_PASS Requested service is handled successfully
 * @return ::RNWF_FAIL Requested service has failed
 */
RNWF_RESULT_t RNWF_NET_UDP_SOCK_Write( uint32_t socket, uint8_t *addr, uint32_t port, uint16_t length, uint8_t *input);

/**
 * @brief NET Socket Read API to read data from TCP Socket.
 * 
 *
 * @param[in] socket        Socket ID 
 * @param[in] length        Length of data to be read 
 * @param[in] input         Input buffer to read the data     
 * 
 * @return ::RNWF_PASS Requested service is handled successfully
 * @return ::RNWF_FAIL Requested service has failed
 */
int16_t RNWF_NET_TCP_SOCK_Read( uint32_t socket, uint16_t length, uint8_t *input);


/**
 * @brief NET Socket Read API to read data from UDP socket.
 * 
 *
 * @param[in] socket        Socket ID 
 * @param[in] length        Length of data to be read 
 * @param[in] input         Input buffer to read the data     
 * 
 * @return ::RNWF_PASS Requested service is handled successfully
 * @return ::RNWF_FAIL Requested service has failed
 */
int16_t RNWF_NET_UDP_SOCK_Read( uint32_t socket, uint16_t length, uint8_t *input);

#endif	/* RNWF_NET_SERVICE_H */

/** @}*/
