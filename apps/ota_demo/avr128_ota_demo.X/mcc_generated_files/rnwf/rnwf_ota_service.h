/**
 * OTA Service header file
 * 
 * @file rnwf_ota_service.h
 * 
 * @defgroup ota_service OTA Configuration Service 
 * 
 * @brief This header file contains data types for OTA service
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


/*! \page ota_service OTA Service
This page is for advanced users.
*/


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_OTA_SERVICE_H
#define	RNWF_OTA_SERVICE_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "../system/pins.h"

#define VAL(str)    #str
#define TOSTR(str)  VAL(str)

#define DBG_MSG_OTA(args, ...)      printf("[OTA]:"args, ##__VA_ARGS__)

#define OTA_BUF_LEN_MAX             4096

#define OTA_CONF_PORT               6666

#define OTA_HTTP_REQ                "GET /%s HTTP/1.1\r\n Connection: Keep-Alive\r\n\r\n"

#define OTA_CONF_FW_HDR             "firmware:"
#define OTA_CONF_FS_HDR             "filesystem:"
/**Server URL Port Number File Name*/

#define OTA_CONF_FORMAT             "%s %d %s"

#define OTA_IMAGE_MAX               2

/* Maximum supported write size by DFU PE (Programming Executive). */
#define MAX_PE_WRITE_SIZE           4096U

/* PE commands */
#define PE_CMD_PAGE_ERASE         0x05
#define PE_CMD_EXEC_VERSION       0x07
#define PE_CMD_PGM_CLUSTER_VERIFY 0x11
#define PE_CMD_GET_DEVICE_ID      0xA
#define CFGMethod                 0x1

/* PE sizes */
#define PE_ERASE_PAGE_SIZE        4096
#define PE_MAX_RESPONSE_SIZE         8


/* Time */
/* Values may need to be adjusted based on host platform. */
#define TP_DELAY_USEC             100
#define MSEC_TO_SEC               1000
#define WRITE_DELAY_USEC          250

/* DFU */
#define RIO0_PE_VERSION 1
//#define RIO0_CHIP_ID    0x29c71053
#define RIO0_CHIP_ID    0x29c70053
#define RIO0_FLASH_BASE 0x60000000
/* DFU_PE_WRITE_SIZE should be integer factor of 4096 and larger than 2
 * e.g. (4,8,16,32,64,128,256,512,4096).
 * Must not exceed MAX_PE_WRITE_SIZE (4096) or DFU_PE_Write() will fail.
 */
#define DFU_PE_WRITE_SIZE   4096


/* Time */
#define UART_DELAY_MSEC     500

#define OTA_SOCK_ID     gOta_CfgData.socket

/**
 * @ingroup ota_service
 * @brief   OTA Configurations Parameters List
 * @enum    OTA_CFG_PARAM_t
*/
typedef enum
{
    OTA_CFG_PARAM_PORT,
    OTA_CFG_PARAM_SERVER,
    OTA_CFG_PARAM_FILE,
    OTA_CFG_PARAM_TYPE,
    OTA_CFG_PARAM_MAX,
}OTA_CFG_PARAM_t;

/**
 * @ingroup ota_service
 * @brief   OTA modes
 * @enum    RNWF_OTA_MODES_t
*/
typedef enum
{
    RNWF_OTA_MODE_HTTP,     /**<FW file is from HTTP server */
    RNWF_OTA_MODE_USB,      /**<FW file is from Host USB */
    RNWF_OTA_MODE_UART,     /**<FW file is from Host UART*/
}RNWF_OTA_MODES_t;

/**
 * @ingroup ota_service
 * @brief   OTA image types
 * @enum    RNWF_OTA_IMAGE_t
*/
typedef enum
{
    RNWF_OTA_LOW_FW,        /**<FW at lower slot */
    RNWF_OTA_HIGH_FW,       /**<FW at higher slot */
    RNWF_OTA_FILESYSTEM,    /**<Files system slot*/
}RNWF_OTA_IMAGE_t;

/**
 * @ingroup ota_service
 * @brief   OTA Service lists
 * @enum    RNWF_OTA_SERVICE_t
*/
typedef enum
{
    RNWF_OTA_ENABLE,                /**<Connected to MQTT broker event */
    RNWF_OTA_DISABLE,               /**<Connected to MQTT broker event */
    RNWF_OTA_REQUEST,                /**<Connected to MQTT broker event */
    RNWF_OTA_SET_CALLBACK,          /**<Register OTA application callback*/
    RNWF_OTA_SET_SRVC_CALLBACK,
    RNWF_OTA_AUTO_MODE,             /**<OTA Auto mode, triggered reqularly*/
    RNWF_OTA_DFU_INIT,           /**<OTA Trigger, Actual programming start*/
    RNWF_OTA_DFU_WRITE,             /**<OTA Write, Writes the FW max 4096 bytes*/
    RNWF_OTA_DFU_ERASE,             /**<OTA Erase, Erases the given size*/
}RNWF_OTA_SERVICE_t;

/**
 * @ingroup ota_service
 * @brief   OTA events
 * @enum    RNWF_OTA_EVENT_t
*/
typedef enum
{
    RNWF_EVENT_MAKE_UART,       /**<Change to UART mode */
    RNWF_EVENT_IMAGE_INFO,      /**<Inform Image details */
    RNWF_EVENT_FILE_CHUNK,      /**<FW Downloaded file chunk */
    RNWF_EVENT_DWLD_START,      /**<FW Download complete */
    RNWF_EVENT_DWLD_DONE,       /**<FW Download complete */
    RNWF_EVENT_DWLD_FAIL,       /**<FW Download failed */
    RNWF_EVENT_CONFIG_INFO,  
}RNWF_OTA_EVENT_t;

/**
 * @ingroup     ota_service
 * @brief       OTA GPIO Control Function
 * @param[in]   pin       GPIO pin number
 * @param[in]   output
 * @return      None
*/
typedef void (*RNWF_OTA_GPIO_CTRL_FUNC_t)(uint32_t pin, uint8_t output);

/**
 * @ingroup     ota_service
 * @brief       OTA UART Tx Function
 * @param[in]   buffer       Buffer to Tx
 * @param[in]   length        Length of the buffer
 * @return None
*/
typedef void (*RNWF_OTA_UART_TX_FUNC_t)(uint8_t *buffer, uint32_t length);

/**
 * @ingroup     ota_service
 * @brief       OTA Flash write function
 * @param[in]   buffer       Buffer to write to flash
 * @param[in]   length        Length of the buffer
 * @return      None
*/
typedef void (*RNWF_OTA_FLASH_WR_FUNC_t)(uint8_t *buffer, uint32_t length);

/**
 * @ingroup     ota_service
 * @brief       OTA Flash Read Function
 * @param[in]   buffer       Buffer
 * @param[in]   length        Length of the data read
 * @return      None
*/
typedef void (*RNWF_OTA_DLASH_RD_FUNC_t)(uint8_t *buffer, uint32_t length);

/**
 * @ingroup     ota_service
 * @brief       OTA Callback Function
 * @param[in]   RNWF_OTA_EVENT_t OTA event type
 * @param[in]   OTA config parameters needed for service
 * @return      None
*/
typedef void (*RNWF_OTA_CALLBACK_t)(RNWF_OTA_EVENT_t, void *);

/**
 * @ingroup ota_service
 * @brief OTA chunk header struct
 * @struct RNWF_OTA_CHUNK_t
*/
typedef struct
{
    uint32_t chunk_addr; 
    uint32_t chunk_size;    
    uint8_t  *chunk_ptr;    
}RNWF_OTA_CHUNK_t;

/**
 * @ingroup ota_service
 * @brief OTA Header struct
 * @struct RNWF_OTA_HDR_t
*/
typedef struct
{
    uint32_t seq_num;
    uint32_t fw_ver;
    uint32_t start_addr;    
    uint32_t img_len;
}RNWF_OTA_HDR_t;

/**
 * @ingroup ota_service
 * @brief   OTA configuration struct
 * @struct  RNWF_OTA_CFG_t
*/
typedef struct
{
    RNWF_NET_SOCKET_t socket;               /**<Socket handler for HTTP link*/
    RNWF_OTA_MODES_t mode;                  /**<Active OTA mode */
    RNWF_OTA_IMAGE_t type;                  /**<Image type */
    const char      *file;                  /**<Image File Name */
    const char      *certificate;
}RNWF_OTA_CFG_t;


/**
 * @ingroup ota_service
 * @brief   MQTT Callback Function handler
 * @var     gOta_CallBack_Handler OTA callback handler function
*/
extern RNWF_OTA_CALLBACK_t gOta_CallBack_Handler;

extern uint32_t g_Ota_SocketId;

extern RNWF_OTA_CFG_t gOta_CfgData;

extern uint32_t otaFileSize;

/**
 * @ingroup     ota_service
 * @brief       OTA Service Layer API to handle system operations.
 * @param[in]   request       Requested service of type RNWF_OTA_SERVICE_t
 * @param[in]   input         Input/Output data for the requested service 
 * @retval      RNWF_PASS Requested service is handled successfully
 * @retval      RNWF_FAIL Requested service has failed
*/
RNWF_RESULT_t RNWF_OTA_SrvCtrl( RNWF_OTA_SERVICE_t request, void *input);


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

RNWF_RESULT_t DFU_PROGRAM_Task(uint32_t otaBinSize);

/**
 * @ingroup ota_service
 * @brief   Send signal to MCLR pin for DFU reset
 * @param[in] None
 * @return None
*/
void     DFU_Reset(void);

/**
 * @ingroup     ota_service
 * @brief       Function to send Programming Executive (PE) test pattern
 * @param[in]   None
 * @return      None
*/
void     DFU_PE_InjectTestPattern(void);

/**
 * @ingroup     ota_service
 * @brief       Function to get PE version
 * @param[in]   None
 * @retval      peVersion    PE Version when successful
 * @return      0    when fails
*/
uint8_t  DFU_PE_Version(void);

/**
 * @ingroup     ota_service
 * @brief       Function to get chip ID
 * @param[in]   None
 * @retval      chipID      Chip ID when successful
 * @retval      0 when fails
*/
uint32_t DFU_PE_Chip_ID(void);

/**
 * @ingroup     ota_service
 * @brief       Function to erase provided size of memory
 * @param[in]   address       memory address from where to erase
 * @param[in]   length        size of memory to erase
 * @retval      TRUE DFU erase successful
 * @retval      FALSE DFU erase failed
*/
bool     DFU_PE_Erase(uint32_t address, const uint32_t length);

/**
 * @ingroup     ota_service
 * @brief       Function to write PE data to RNWF device
 * @param[in]   address       memory address
 * @param[in]   length        size of data to write (max chunk size is 4096 bytes)
 * @param[in]   PE_writeBuffer OTA chunk pointer
 * @retval      TRUE Write successful
 * @retval      FALSE Write failed
*/
bool     DFU_PE_Write(uint32_t address, const uint32_t length, uint8_t *PE_writeBuffer);


#endif	/* XC_HEADER_TEMPLATE_H */

/** @}*/
