/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_ota_service.c
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

#include "rnwf_interface.h"
#include "rnwf_net_service.h"
#include "rnwf_ota_service.h"
#include "rnwf_system_service.h"
#include "../timer/delay.h"



#define HTTP_CONTENT_OK    "200 OK"
#define HTTP_CONTENT_LEN    "Content-Length:"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


RNWF_OTA_CALLBACK_t gOta_CallBack_Handler;

uint8_t *g_ota_buf = NULL;//[OTA_BUF_LEN_MAX];
#ifdef RNWF11_SERVICE
uint32_t g_crc_tab[256];
uint32_t g_crc = 0xffffffff;
#endif
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

volatile uint32_t total_rx = 0;

uint32_t otaFileSize = 0;

#ifdef RNWF11_SERVICE
RNWF_RESULT_t RNWF_OTA_CRC32_Init(uint32_t crc_tab[256])
{
    uint32_t   i, j, value;

    for (i = 0; i < 256; i++)
    {
        value = i;
        for (j = 0; j < 8; j++)
        {
            if (value & 1)
            {
                value = (value >> 1) ^ 0xEDB88320;
            }
            else
            {
                value >>= 1;
            }
        }
        crc_tab[i] = value;
    }

    return RNWF_PASS;
}

RNWF_RESULT_t RNWF_OTA_CRC32_Generate(uint8_t* data_buf, uint32_t size, uint32_t* crc, uint32_t crc_tab[256])
{
    uint32_t   i;

    for (i = 0; i < size; i++)
    {
        *crc = crc_tab[(*crc ^ data_buf[i]) & 0xff] ^ (*crc >> 8);
    }

    return RNWF_PASS;
}
#endif

RNWF_RESULT_t RNWF_OTA_CONF_Process(uint32_t socket, uint16_t rx_len)
{
    int32_t read_size, result;
    uint8_t *tmpPtr, *buffer = &g_ota_buf[OTA_BUF_LEN_MAX/2];
    RNWF_OTA_CFG_t otaCfg;
    while(rx_len > 0)
    {
        if((result = RNWF_NET_TCP_SOCK_Read(socket, rx_len, buffer)) > 0 )
        {
            rx_len -= result;
            read_size += result;
        }
        else
        {
            break;
        }
    }
    if(strncmp((char *)buffer, OTA_CONF_FW_HDR, strlen(OTA_CONF_FW_HDR)) || strncmp((char *)buffer, OTA_CONF_FS_HDR, strlen(OTA_CONF_FS_HDR)))
    {
        char *args[OTA_CFG_PARAM_MAX] = {0, 0, 0, 0}, *token;
        uint8_t idx = 0;
        tmpPtr = buffer + strlen(OTA_CONF_FW_HDR);
        token = (char *)strtok((char *)tmpPtr, ", ");
        while(token != NULL)
        {
            printf("%s\r\n", token);
            args[idx++] = token;
            token = (char *)strtok(NULL, ", ");
        }
        otaCfg.socket.tls_conf = 0;
        otaCfg.socket.bind_type = RNWF_BIND_REMOTE;
        otaCfg.socket.sock_type = RNWF_SOCK_TCP;
        sscanf(args[OTA_CFG_PARAM_PORT], "%u", &otaCfg.socket.sock_port);
        otaCfg.socket.sock_addr = args[OTA_CFG_PARAM_SERVER];
        otaCfg.file = args[OTA_CFG_PARAM_FILE];
        otaCfg.type = strtol(args[OTA_CFG_PARAM_TYPE], NULL, 10);
        gOta_CallBack_Handler(RNWF_EVENT_IMAGE_INFO, (uint8_t *)&otaCfg);
        RNWF_OTA_SrvCtrl(RNWF_OTA_REQUEST, (void *)&otaCfg);
    }
    
    return RNWF_PASS;
}

RNWF_RESULT_t RNWF_OTA_DWNLD_Process(uint32_t socket, uint16_t rx_len)
{
    int32_t read_size;
    RNWF_OTA_CHUNK_t ota_chunk = {.chunk_ptr = g_ota_buf};
    static uint16_t result = 0;
    char *tmpPtr;
    //Parse the First HTTP Response
    if((total_rx > 0) && (total_rx == otaFileSize))
    {
        return RNWF_PASS;
    }

    while(rx_len > 0)
    {
        uint16_t readCnt = ((rx_len + result) > OTA_BUF_LEN_MAX)?(OTA_BUF_LEN_MAX-result):rx_len;
        if((read_size = RNWF_NET_TCP_SOCK_Read(socket, readCnt, (uint8_t *)&g_ota_buf[result])) > 0 )
        {
            if(!otaFileSize)
            {
                tmpPtr = (char *)strstr((const char *)g_ota_buf, (const char *)HTTP_CONTENT_OK);
                if(tmpPtr)
                {
                    DBG_MSG_OTA("%s\r\n", g_ota_buf);
                    tmpPtr = (char *)strstr((const char *)g_ota_buf, (const char *)HTTP_CONTENT_LEN);
                    if(tmpPtr == NULL)
                        break;
                    char *token = strtok(tmpPtr, "\r\n");
                    otaFileSize = strtol((token+sizeof(HTTP_CONTENT_LEN)), NULL, 10);
                    gOta_CallBack_Handler(RNWF_EVENT_DWLD_START, (uint8_t *)&otaFileSize);
                }
                else
                {
                    DBG_MSG_OTA("File Not Found!\r\n");
                    RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, &socket);
                    gOta_CallBack_Handler(RNWF_EVENT_DWLD_FAIL, (uint8_t *)NULL);
                }
                break;
            }            
            result += read_size;
            total_rx = total_rx + read_size;
            rx_len -= read_size;
            if(result == OTA_BUF_LEN_MAX)
            {
                ota_chunk.chunk_size = OTA_BUF_LEN_MAX;  
                DBG_MSG_OTA("%lu\r\n", total_rx);
                gOta_CallBack_Handler(RNWF_EVENT_FILE_CHUNK, (uint8_t *)&ota_chunk);                
                result = 0;
            }
            if(total_rx == otaFileSize)
            {
                ota_chunk.chunk_size = result;                                
                gOta_CallBack_Handler(RNWF_EVENT_FILE_CHUNK, (uint8_t *)&ota_chunk);
                gOta_CallBack_Handler(RNWF_EVENT_DWLD_DONE, (uint8_t *)&total_rx);
                return RNWF_PASS;
            }
        }
        else
        {
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, &socket);
            gOta_CallBack_Handler(RNWF_EVENT_DWLD_FAIL, (uint8_t *)NULL);
            return RNWF_FAIL;
        }
    }
    return RNWF_PASS;
}

void RNWF_OTA_SOCKET_Callback(uint32_t sock, RNWF_NET_SOCK_EVENT_t event, uint8_t *p_str)
{
    static uint32_t cfg_client_id = 0;
    switch(event)
    {
        case RNWF_NET_SOCK_EVENT_CONNECTED:
        {
            if(!cfg_client_id && strstr((const char *)p_str, TOSTR(OTA_CONF_PORT)))
            {
                cfg_client_id = sock;
                DBG_MSG_OTA("Connection received on configuration Tunnel %lu\r\n", cfg_client_id);
                RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_DEV_INFO, g_ota_buf);
            }
            RNWF_NET_TCP_SOCK_Write(sock, strlen((char *)g_ota_buf), (uint8_t *)g_ota_buf);
        }
        break;

        case RNWF_NET_SOCK_EVENT_DISCONNECTED:
        {
            DBG_MSG_OTA("Close OTA Socket!\r\n");
            cfg_client_id = 0;
            RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_CLOSE, &sock);
            break;
        }
        case RNWF_NET_SOCK_EVENT_READ:
        {
            uint16_t rx_len = *(uint16_t *)p_str;
            if(cfg_client_id == sock)
                RNWF_OTA_CONF_Process(sock, rx_len);
            else
                RNWF_OTA_DWNLD_Process(sock, rx_len);
            break;
        }
        case RNWF_NET_SOCK_EVENT_ERROR:
        {
            DBG_MSG_OTA("Socket Error! Re Trigger OTA operation!\r\n");
        }
        break;
        default:
            break;
    }
}



RNWF_RESULT_t RNWF_OTA_SrvCtrl( RNWF_OTA_SERVICE_t request, void *input)
{
    RNWF_RESULT_t result = RNWF_PASS;    

    switch(request)
    {
        case RNWF_OTA_ENABLE:
        {
            // Bind a lock port to set the OTA configuration
            RNWF_NET_SOCKET_t ota_cfgSock;            
            if(input == NULL)
            {
                result = RNWF_FAIL;
                break;                
            }
            g_ota_buf = (uint8_t *)input;
            ota_cfgSock.bind_type = RNWF_BIND_LOCAL;
            ota_cfgSock.sock_port = 6666;
            ota_cfgSock.sock_type = RNWF_SOCK_TCP;
            ota_cfgSock.tls_conf = 0;
            /* RNWF Application Callback register */
            if(gOta_CallBack_Handler != NULL)
            {
                RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_SET_SRVC_CALLBACK, RNWF_OTA_SOCKET_Callback);
                result = RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_TCP_OPEN, &ota_cfgSock);
            }
        }
        break;

        case RNWF_OTA_REQUEST:
        {
            RNWF_OTA_CFG_t *otaCfg = (RNWF_OTA_CFG_t *)input;
            sprintf((char *)g_ota_buf, OTA_HTTP_REQ, otaCfg->file);
            if(gOta_CallBack_Handler != NULL)
            {
                result = RNWF_NET_SOCK_SrvCtrl(RNWF_NET_SOCK_TCP_OPEN, &otaCfg->socket);
            }
        }
        break;

        case RNWF_OTA_SET_CALLBACK:
        {
            gOta_CallBack_Handler = (RNWF_OTA_CALLBACK_t)(input);
        }
        break;

        case RNWF_OTA_DFU_INIT:
        {
#ifndef RNWF11_SERVICE      /* RNWF02 */
            uint8_t  peVersion = 0;
            uint32_t chipID = 0;

            DFU_PE_InjectTestPattern();

            DELAY_milliseconds(UART_DELAY_MSEC);

            /* Verify PE version */
            peVersion = DFU_PE_Version();

            if(peVersion != RIO0_PE_VERSION)
            {
                DBG_MSG_OTA("PE version didn't match");
                result = RNWF_FAIL;
            }

            /* Verify chip ID */
            chipID = DFU_PE_Chip_ID();

            if ((chipID & 0xFFFF00FF) != RIO0_CHIP_ID)
            {
                DBG_MSG_OTA("Chip ID didn't match\r\n");
                result = RNWF_FAIL;
            }   
#else   /* RNWF11_SERVUCE */
            
            uint16_t BTL_Ver = 0;
            int8_t btl_chk_cnt = 5;

            RNWF_OTA_CRC32_Init(&g_crc_tab);
            g_crc = 0xffffffff;

            RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_TBL, NULL);  
            DELAY_milliseconds(UART_DELAY_MSEC);

            // Verify bootloader version
            while (btl_chk_cnt)
            {
                BTL_Ver = DFU_UART_Read_Bootloader_Version();
                DBG_MSG_OTA("BTL_Ver = 0x%x\r\n", BTL_Ver);
                if (BTL_Ver != 0)
                    break;

                btl_chk_cnt--;
                DELAY_milliseconds(500);
            }
            if (btl_chk_cnt == 0)
            {
                result = RNWF_FAIL;
            }
#endif            
        }
        break;

        case RNWF_OTA_DFU_ERASE:
        {         
            
#ifndef RNWF11_SERVICE      /* RNWF02 */
            
            RNWF_OTA_CHUNK_t *otaChunk = (RNWF_OTA_CHUNK_t *)input;             
            /* Erase */
            if (DFU_PE_Erase(otaChunk->chunk_addr, otaChunk->chunk_size) == false)
            {
                /* Error handling */
                DBG_MSG_OTA("PE erase reported error\r\n");
                result = RNWF_FAIL;
            }
            
#else   /* RNWF11_SERVUCE */
            
            RNWF_OTA_CHUNK_t *otaChunk = (RNWF_OTA_CHUNK_t *)input; 
            bool result = false;
            // Unlock the RNWF11 flash
            result = DFU_UART_Unlock(otaChunk->chunk_addr, otaChunk->chunk_size);
            result = DFU_UART_ERASE_APP();
            
#endif         
        }
        break;

        case RNWF_OTA_DFU_WRITE:
        {
            RNWF_OTA_CHUNK_t *otaChunk = (RNWF_OTA_CHUNK_t *)input;
#ifndef RNWF11_SERVICE      /* RNWF02 */
            
            if(DFU_PE_Write(otaChunk->chunk_addr, otaChunk->chunk_size, otaChunk->chunk_ptr))
            {                
                result = RNWF_FAIL;
            }
            
#else   /* RNWF11_SERVUCE */
            
            RNWF_OTA_CRC32_Generate(otaChunk->chunk_ptr, otaChunk->chunk_size, &g_crc, &g_crc_tab);

            if(DFU_UART_Write(otaChunk->chunk_addr, otaChunk->chunk_size, otaChunk->chunk_ptr) == false)
            {                
                result = RNWF_FAIL;
            }
#endif            
        }
        break;
 
#ifdef RNWF11_SERVICE
        case RNWF_OTA_DFU_RESET:
        {
            if (DFU_UART_Verify(g_crc) == true)
            {
                DBG_MSG_OTA("crc32 verify pass\r\n");
                DFU_UART_Reset();
            }
            
        }
        break;
#endif

        default:
            break;
    }
    return result;
}


/***********************RNWF02 DFU API's*************************/
#ifndef RNWF11_SERVICE      /* RNWF02 */
#ifdef DFU_DEBUG
static uint32_t DFU_PE_htonl (uint32_t x)
{
    uint32_t ret = x;

    if (BYTE_ORDER == BIG_ENDIAN)
    {
    }
    else if (BYTE_ORDER == LITTLE_ENDIAN)
    {
      ret = __builtin_bswap32 (x);
    }
    else
    {
        DBG_MSG_OTA("ERROR: Unknown system\r\n");
        DFU_ASSERT(false);
    }

    return ret;
}
#endif
#endif

#ifndef RNWF11_SERVICE      /* RNWF02 */

void DFU_PE_InjectTestPattern(void)
{
    /* Programming Executive (PE) test pattern 'MCHP' PGC->Tx PGC->Rx*/
    const char *PE_TP_MCLR = "100000000000000000000000000000000000000000000000000000000000000001";
    const char *PE_TP_PGC  = "001010101010101010101010101010101010101010101010101010101010101011";
    const char *PE_TP_PGD  = "000110000111100110011000000001111001100001100000000110011000000000";

    DBG_MSG_OTA("* Sending test pattern *\r\n%s\r\n%s\r\n%s\r\n", PE_TP_MCLR, PE_TP_PGC, PE_TP_PGD);
    UART2.Deinitialize();

    MCLR_SetDigitalOutput();
    PGC_SetDigitalOutput();
    PGD_SetDigitalOutput();

    DELAY_milliseconds(MSEC_TO_SEC);

    for (volatile int i=0; i<(int)strlen(PE_TP_MCLR); i++)
    {
        /* MCLR - CHipEnable*/
        if((PE_TP_MCLR[i] - '0'))
            MCLR_SetHigh();
        else
            MCLR_SetLow();
        /* UART TX */
        if((PE_TP_PGC[i] - '0'))
            PGC_SetHigh();
        else
            PGC_SetLow();

        /* UART RX */
        if((PE_TP_PGD[i] - '0'))
            PGD_SetHigh();
        else
            PGD_SetLow();

        DELAY_microseconds(TP_DELAY_USEC);
    }

    PGD_SetDigitalInput();
    MCLR_SetDigitalInput();
    UART2.Initialize();
}

uint8_t DFU_PE_Version(void)
{
    uint32_t data = 0;
    uint8_t  peVersion = 0;
    uint8_t  byteResp[4];

    data = PE_CMD_EXEC_VERSION;
    data = (data << 16) | 0x1;

    DBG_MSG_OTA("Sending PE version request\r\n");
#ifdef DFU_DEBUG
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(data));
#endif

    RNWF_IF_Write((uint8_t *)&data, 4);
    
    if(RNWF_IF_Read(byteResp, 4) == 4)
    {
        peVersion = byteResp[0];
        DBG_MSG_OTA("PE version: %d\r\n\r\n", (unsigned int)peVersion);
        return peVersion;
    }
    return 0;
}

uint32_t DFU_PE_Chip_ID(void)
{
    uint32_t data = 0;    
    uint32_t  byteResp[2];

    data = PE_CMD_GET_DEVICE_ID;
    data = (data << 16) | 0x01;

    DBG_MSG_OTA("Sending PE chip ID request\r\n");
#ifdef DFU_DEBUG
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(data));
#endif
    RNWF_IF_Write((uint8_t *)&data, 4);

    /* Response */
    RNWF_IF_Read((uint8_t *)byteResp, 8);

    DBG_MSG_OTA("Chip ID: %lX\r\n", (uint32_t)byteResp[1]);
    return byteResp[1];
}

bool DFU_PE_Erase(const uint32_t address, const uint32_t length)
{
    uint32_t data = 0;
    uint32_t pages = length / (uint32_t)PE_ERASE_PAGE_SIZE;
    uint8_t  byteResp[4];

    if (length % (uint32_t)PE_ERASE_PAGE_SIZE > (uint32_t)0)
    {
        pages += (uint32_t)1;
    }
    DBG_MSG_OTA("PE erase pages = %d\r\n", pages);

    data = PE_CMD_PAGE_ERASE;
    data = data << 16;
    data |= (pages &= 0x0000ffff);


    DBG_MSG_OTA("Sending PE erase\r\n");
#ifdef DFU_DEBUG
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(data));
#endif
    RNWF_IF_Write((uint8_t *)&data, 4);
    DELAY_microseconds(WRITE_DELAY_USEC);

#ifdef DFU_DEBUG
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(address));
#endif
    RNWF_IF_Write((uint8_t *)&address, 4);
    DELAY_microseconds(WRITE_DELAY_USEC);
                
    DELAY_milliseconds(3500);
    /* Response */
    RNWF_IF_Read(byteResp, 4);
        
    if (((char)byteResp[2] != (char)PE_CMD_PAGE_ERASE) || ((char)byteResp[0] != (char)0) || ((char)byteResp[1] != (char)0))
    {
        DBG_MSG_OTA("Error: PE erase failed\r\n");
        return false;
    }

    DBG_MSG_OTA("\r\nErase done!\r\n");
    return true;
}

bool DFU_PE_Write(uint32_t address, const uint32_t length, uint8_t *PE_writeBuffer)
{
    /* The address must be 32-bit aligned, and the number of bytes (length) must be a
    multiple of a 32-bit word. */
    uint32_t data = 0;
    uint32_t checksumValue = 0;
    uint8_t byteResp[4];

    if (length>(uint16_t)MAX_PE_WRITE_SIZE)
    {
        DBG_MSG_OTA("ERROR: Length exceeds MAX_PE_WRITE_SIZE\r\n");
        return false;
    }

    /* Length should be integer factor of 4096 and divisible by 4 */
    if ((((uint16_t)MAX_PE_WRITE_SIZE % length) != (uint16_t)0) || ((length % (uint16_t)4) != (uint16_t)0))
    {
        DBG_MSG_OTA("ERROR: Length should be integer factor of 4096 and divisible by 4\r\n");
        return false;
    }

    /* Assemble PE write command */
    data |= ((uint32_t)0x0000ffff & (uint32_t)PE_CMD_PGM_CLUSTER_VERIFY);
    data = data << 16;
    data |= (CFGMethod & 0x0000ffff);
    
#ifdef DFU_DEBUG
    DBG_MSG_OTA("ID:\r\n");
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(data));
#endif
    RNWF_IF_Write((uint8_t *)&data, sizeof(uint32_t));
    DELAY_microseconds(WRITE_DELAY_USEC);
        
#ifdef DFU_DEBUG
    /* Address */
    DBG_MSG_OTA("Address:\r\n");
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(address));
#endif
    RNWF_IF_Write((uint8_t *)&address, sizeof(uint32_t));
    DELAY_microseconds(WRITE_DELAY_USEC);
    
#ifdef DFU_DEBUG
    /* Length */
    DBG_MSG_OTA("Length: %d\r\n", (unsigned int)length);
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(length));
#endif
    RNWF_IF_Write((uint8_t *)&length, sizeof(uint32_t));
    DELAY_microseconds(WRITE_DELAY_USEC);

    /* Checksum */
    for (uint16_t i=0; i<length; i++)
    {
        checksumValue += PE_writeBuffer[i];
    }

    
#ifdef DFU_DEBUG
    DBG_MSG_OTA("Checksum:\r\n");
    DBG_MSG_OTA("%08x\r\n", (unsigned int)DFU_PE_htonl(checksumValue));
#endif
    RNWF_IF_Write((uint8_t *)&checksumValue, sizeof(uint32_t));
    DELAY_microseconds(WRITE_DELAY_USEC);

#ifdef DFU_DEBUG
    /* Data */
    DBG_MSG_OTA("PE_writeBuffer:\r\n");
#endif        
//    for (uint16_t i=0; i<length; i++)
    {
#ifdef DFU_DEBUG
        DBG_MSG_OTA("%02x ", PE_writeBuffer[i]);
#endif       
        RNWF_IF_Write((uint8_t *)PE_writeBuffer, length);
//        RNWF_IF_Write((uint8_t *)&PE_writeBuffer[i], 1);

        DELAY_microseconds(60);
    }
#ifdef DFU_DEBUG
    DBG_MSG_OTA("\r\n");
#endif        
    
    /* Response */
    RNWF_IF_Read(byteResp, 4);

    /* Verify response for errors */
    if (((char)byteResp[2] != (char)PE_CMD_PGM_CLUSTER_VERIFY) || ((char)byteResp[0] != (char)0) || ((char)byteResp[1] != (char)0))
    {
        DBG_MSG_OTA("Error: PE write failed[%02X %02X %02X %02X]\r\n", byteResp[0], byteResp[1], byteResp[2], byteResp[3]);
        return false;
    }

    return true;
}

#else   /* RNWF11_SERVICE */

uint8_t send_request_cmd_data(uint8_t cmd, uint8_t *data, uint32_t dataLen, uint32_t dataAddr) {
    uint8_t req[HEADER_SIZE];
    int resp;
    uint8_t byteResp;

    if ( BL_CMD_DATA == cmd )
    {
        dataLen += 4;
    }
    
    memset (req, 0, sizeof(req));
    
    req[0] = (uint8_t)(BL_GUARD & 0xFF);        // store the least significant byte
    req[1] = (uint8_t)((BL_GUARD >> 8) & 0xFF); 
    req[2] = (uint8_t)((BL_GUARD >> 16) & 0xFF);
    req[3] = (uint8_t)((BL_GUARD >> 24) & 0xFF);
    req[4] = (uint8_t)(dataLen & 0xFF);        // store the least significant byte
    req[5] = (uint8_t)((dataLen >> 8) & 0xFF); 
    req[6] = (uint8_t)((dataLen >> 16) & 0xFF);
    req[7] = (uint8_t)((dataLen >> 24) & 0xFF);
    req[8] = cmd;
 
    RNWF_IF_Write((uint8_t *)&req, HEADER_SIZE);
    DELAY_microseconds(WRITE_DELAY_USEC);
    
    if ( 0 == dataLen)
    {
        uint8_t extraData[4];
        memset(extraData, 0, sizeof(extraData));
        RNWF_IF_Write(data, CMD_ONLY_EXTRA_SIZE);
        DELAY_microseconds(WRITE_DELAY_USEC);
    }
    else
    {
        uint8_t addrBuf[ADDR_SIZE];

    
        addrBuf[0] = (uint8_t)(dataAddr & 0xFF);        // store the least significant byte
        addrBuf[1] = (uint8_t)((dataAddr >> 8) & 0xFF); 
        addrBuf[2] = (uint8_t)((dataAddr >> 16) & 0xFF);
        addrBuf[3] = (uint8_t)((dataAddr >> 24) & 0xFF);
    
        RNWF_IF_Write(addrBuf, ADDR_SIZE);
        DELAY_microseconds(WRITE_DELAY_USEC);
    
        RNWF_IF_Write(data, dataLen);
        DELAY_microseconds(WRITE_DELAY_USEC);
    }

    resp = RNWF_IF_Read(&byteResp, 1);
    
    if (resp == 0)
    {
        DBG_MSG_OTA("ERROR: no response after write\r\n");
        byteResp = BL_RESP_NONE;
    }

        
    return byteResp;
}


uint8_t send_request(uint8_t cmd, uint8_t *data, int dataLen) {
    uint8_t req[HEADER_SIZE + CMD_ONLY_EXTRA_SIZE + dataLen];
    int resp;
    int cnt = 10;
    uint8_t byteResp;
    
    memset (req, 0, sizeof(req));
    
    req[0] = (uint8_t)(BL_GUARD & 0xFF);        // store the least significant byte
    req[1] = (uint8_t)((BL_GUARD >> 8) & 0xFF); 
    req[2] = (uint8_t)((BL_GUARD >> 16) & 0xFF);
    req[3] = (uint8_t)((BL_GUARD >> 24) & 0xFF);
    req[4] = (uint8_t)(dataLen & 0xFF);        // store the least significant byte
    req[5] = (uint8_t)((dataLen >> 8) & 0xFF); 
    req[6] = (uint8_t)((dataLen >> 16) & 0xFF);
    req[7] = (uint8_t)((dataLen >> 24) & 0xFF);
    req[8] = cmd;
    
    RNWF_IF_Write((uint8_t *)&req, HEADER_SIZE);
    DELAY_microseconds(WRITE_DELAY_USEC);
    
    if ( 0 == dataLen)
    {
        uint8_t extraData[4];
        memset(extraData, 0, sizeof(extraData));
        RNWF_IF_Write(data, CMD_ONLY_EXTRA_SIZE);
        DELAY_microseconds(WRITE_DELAY_USEC);
    }
    else
    {  
        RNWF_IF_Write(data, dataLen);
        //DBG_MSG_OTA("data is 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        DELAY_microseconds(WRITE_DELAY_USEC);
    }
 
    
    resp = 0;
    while (resp == 0 && cnt > 0)
    {
        resp = RNWF_IF_Read(&byteResp, 1);
        
        if (resp == 0)
        {
            DBG_MSG_OTA("ERROR: no response after write, %d\r\n", cnt);
            byteResp = BL_RESP_NONE;
        }
        cnt--;
    }
    
    return byteResp;
}


bool DFU_UART_Write(uint32_t address, const uint32_t length, uint8_t *writeBuffer)
{
    uint8_t resp;
    
    resp = send_request_cmd_data(BL_CMD_DATA, writeBuffer, length, address);
    
    if ( BL_RESP_OK == resp)
        return true;
    else
        return false;
}

bool DFU_UART_Verify(uint32_t crc)
{
    uint8_t resp;
    uint8_t dataBuf[4];
    bool res = false;
    
    memcpy(dataBuf, &crc, sizeof(crc));
    resp = send_request(BL_CMD_VERIFY, dataBuf, sizeof(dataBuf));
    
    if ( BL_RESP_CRC_OK == resp)
    {
        DBG_MSG_OTA("crc32 verify success\r\n");
        res = true;
    }
    else
    {
        DBG_MSG_OTA("crc32 verify fail\r\n");
    }

    return res;
}

bool DFU_UART_ERASE_APP(void)
{
    uint8_t resp;
    uint8_t dataBuf[16];
    bool res = false;
    
    memset(dataBuf, 0, sizeof(dataBuf));

    resp = send_request(BL_CMD_ERASE_APP, dataBuf, 16);
    
    if ( BL_RESP_OK == resp)
    {
        res = true;
    }
    return res;
}

bool DFU_UART_Reset(void)
{
    uint8_t resp;
    uint8_t dataBuf[16];
    bool res = false;
    
    memset(dataBuf, 0, sizeof(dataBuf));

    resp = send_request(BL_CMD_RESET, dataBuf, 16);
    
    if ( BL_RESP_OK == resp)
    {
        res = true;
    }
    return res;
}

bool DFU_UART_Unlock(uint32_t address, uint32_t size)
{
    uint8_t resp;
    uint8_t dataBuf[8];
    bool res = false;
    

    dataBuf[0] = (uint8_t)(address & 0xFF);        // store the least significant byte
    dataBuf[1] = (uint8_t)((address >> 8) & 0xFF); 
    dataBuf[2] = (uint8_t)((address >> 16) & 0xFF);
    dataBuf[3] = (uint8_t)((address >> 24) & 0xFF);
    
    dataBuf[4] = (uint8_t)(size & 0xFF);        // store the least significant byte
    dataBuf[5] = (uint8_t)((size >> 8) & 0xFF); 
    dataBuf[6] = (uint8_t)((size >> 16) & 0xFF);
    dataBuf[7] = (uint8_t)((size >> 24) & 0xFF);
    
    
    resp = send_request(BL_CMD_UNLOCK, dataBuf, 8);
    
    if ( BL_RESP_OK == resp)
    {
        res = true;
    }
    return res;
}

uint16_t DFU_UART_Read_Bootloader_Version(void)
{
    uint8_t resp, majorVer, minorVer;
    uint8_t res;
    uint16_t version = 0;

    resp = send_request(BL_CMD_READ_VERSION, NULL, 0);
    
    if ( BL_RESP_OK == resp)
    {
        res = RNWF_IF_Read(&majorVer, 1);
        res = RNWF_IF_Read(&minorVer, 1);

        version = (uint16_t)majorVer << 8 | minorVer;
    }
    else
    {
        DBG_MSG_OTA("ERROR: Fail to read the bootloader version\r\n");
    }
    return version;
}

#endif

void DFU_Reset(void)
{
    const char *MCLR_RESET = "101";

    DBG_MSG_OTA("Resetting RNWF Module\r\n");

    MCLR_SetDigitalOutput();
    for (uint8_t i=0; i<(uint8_t)strlen(MCLR_RESET); i++)
    {
        if((MCLR_RESET[i] - '0') == '1')
            MCLR_SetHigh();
        else
            MCLR_SetLow();

        DELAY_microseconds(TP_DELAY_USEC);
    }
    MCLR_SetDigitalInput();
}