/**
 * INF Wi-Fi interface source file
 * 
 * @file rnwf_interface.c
 *
 * @ingroup rnwf_interface
 * 
 * @brief This file contains APIs and data types needed for Wi-Fi interface
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

/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../timer/delay.h"

#include "rnwf_interface.h"
#include "rnwf_wifi_service.h"
#include "rnwf_net_service.h"
#include "rnwf_system_service.h"
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


RNWF_INTERFACE_STATE_t   g_interface_state = RNWF_INTERFACE_FREE;

uint32_t g_interface_timeout = RNWF_INTERFACE_TIMEOUT;


uint8_t g_async_buffer[RNWF_IF_ASYCN_BUF_MAX];


/* Queue used by the interface bus to full the incoming frames*/
IF_QUEUE_t g_if_free_q;
/* Queue used push the read bytes to process incoming frames*/
IF_QUEUE_t g_if_rx_q;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */


/* Circular Queue implementation */

/**
 * @ingroup     rnwf_interface
 * @brief       Circular queue
 * @param[in]   if_q    queue pointer
 * @return      None
*/
void if_q_init(IF_QUEUE_t *if_q)
{
    if_q->tail = -1;
    if_q->head = 0;
    if_q->size = 0;
}

/**
 * @ingroup     rnwf_interface
 * @brief       Queue full API
 * @param[in]   if_q    queue pointer
 * @retval      true    When buffer is full
 * @retval      false   Buffer is not full
*/
bool if_q_full(IF_QUEUE_t *if_q)
{
    if(if_q->size >= RNWF_IF_BUF_MAX)
        return true;

    return false;
}

/**
 * @ingroup     rnwf_interface
 * @brief       Queue empty API
 * @param[in]   if_q    queue pointer
 * @retval      true    queue is empty
 * @retval      false   queue is not empty
*/
bool if_q_empty(IF_QUEUE_t *if_q)
{
    if(if_q->size)
        return false;

    return true;
}

/**
 * @ingroup     rnwf_interface
 * @brief       Queue Enqueue API
 * @param[in]   if_q    queue pointer
 * @retval      false   when queue is full
 * @retval      true    enqueue was successful
*/
bool if_q_enqueue(IF_QUEUE_t *if_q, uint8_t *mem_idx)
{

    if(if_q_full(if_q))
    {
        return false;
    }
    if(if_q_empty(if_q))
    {
        if_q->tail = -1;
        if_q->head = 0;
    }
    if_q->tail = ((if_q->tail + 1) % RNWF_IF_BUF_MAX);
    if_q->queue[if_q->tail] = mem_idx;
    if_q->size++;

    return true;
}

/**
 * @ingroup     rnwf_interface
 * @brief       if_q_dequeue()  Queue Dequeue API
 * @param[in]   if_q      queue pointer
 * @retval      false     if queue is empty
 * @retval      true      dequeue successful  
*/
bool if_q_dequeue(IF_QUEUE_t *if_q, uint8_t **mem_idx)
{
    if(if_q_empty(if_q))
    {
        return false;
    }

    *mem_idx = if_q->queue[if_q->head];
    if_q->head = ((if_q->head + 1) % RNWF_IF_BUF_MAX);
    if_q->size--;

    return true;    
}



RNWF_RESULT_t RNWF_IF_SW_Reset(void)
{    
    return RNWF_CMD_RSP_Send(NULL, NULL, NULL, "\r\n", NULL);    
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/* feature additions from FW v2.0.0*/
/**
 * @ingroup     rnwf_interface
 * @brief       Determines type of IP address
 * @param[in]   input   ip address string buffer
 * @retval              RNWF_IP_UNKNOWN         IP address error code
 * @retval              RNWF_LINK_LOCAL_IPv6    Link-local IPv6 address
 * @retval              RNWF_GLOBAL_IPv6        Global IPv6 address
 * @retval              RNWF_IPv4               IPv4 address
*/
RNWF_DHCP_IP_ADD_TYPE_t getipAddType(const char *input) {
    uint8_t count = 0;
    RNWF_DHCP_IP_ADD_TYPE_t type = RNWF_IP_UNKNOWN;
    
    if(strstr(input, "::") || (strstr(input, ":")))
    {
        if(strstr(input, "FE80"))
        {
            type = RNWF_LINK_LOCAL_IPv6;
        }
        else if(!(strstr(input, "FC00"))){
            type = RNWF_GLOBAL_IPv6;
        }   
    } else if (strstr(input, '.')) 
    {
        for (uint8_t index = 0; input[index] != '\0'; index++)
        {
            if(input[index] == '.')
            {
                count++;
            }
        }
        if(count == 3){
            type = RNWF_IPv4;
        }
    }
    return type;
}
 
// *****************************************************************************

/**
 * @ingroup     rnwf_interface
 * @brief       Handles async event/message received from RNWFxx firmware and triggers respective callback functions
 * @param[in]   p_msg   message buffer for async event/message received from RNWFxx firmware
 * @retval              RNWF_PASS Requested event is handled successfully
 * @retval              RNWF_FAIL Requested event has failed
 * @retval              RNWF_COTN Requires retries
*/
RNWF_RESULT_t RNWF_IF_ASYNC_Handler(uint8_t *p_msg)
{    
    uint16_t arg_len = 0;
    RNWF_RESULT_t result = RNWF_COTN;     
    uint8_t *p_arg = (uint8_t *)(strstr((char *)p_msg, RNWF_ARG_DELIMETER) + 1);
    while(p_arg[arg_len] != '\0')
    {
        (p_arg[arg_len] == ',') ? p_arg[arg_len++] = ' ':arg_len++;        
    }
    RNWF_RESPONSE_Trim(p_arg);
    
#ifdef RNWF_INTERFACE_DEBUG    
    DBG_MSG_IF("Async Message -> %s\n", p_msg);
    DBG_MSG_IF("Async Arguments-> %s\n", p_arg);
#endif
    
    switch(p_msg[0])                        
    {
        /** Wi-Fi DHCP/DNS, Ping or Net based Async Events*/
        case 'W':
        case 'D':
        case 'N':
        case 'T':
     
        {            
            for(int i = 0; i < RNWF_WIFI_SERVICE_CB_MAX; i++)
            {
                if(gWifi_CallBack_Handler[i] == 0)
                    continue;
                
                RNWF_WIFI_CALLBACK_t wifi_cb_func = gWifi_CallBack_Handler[i];
                if(strstr((char *)p_msg, RNWF_EVENT_STA_AUTO_IP) || strstr((char *)p_msg, RNWF_EVENT_AP_AUTO_IP))
                {               
                    const char *ipAddStr = (const char*) &p_arg[2];
                    RNWF_DHCP_IP_ADD_TYPE_t ipAddType = RNWF_IP_UNKNOWN;
                    ipAddType = getipAddType(ipAddStr);

                    if (ipAddType == RNWF_IPv4)
                    {
                        wifi_cb_func(RNWF_DHCP_IPV4_DONE, p_arg);                        
                    }
                    else if (ipAddType == RNWF_LINK_LOCAL_IPv6) 
                    {
                        wifi_cb_func(RNWF_DHCP_LINK_LOCAL_IPV6_DONE, p_arg);
                    }
                    else if (ipAddType == RNWF_GLOBAL_IPv6)
                    {
                        wifi_cb_func(RNWF_DHCP_GLOBAL_IPV6_DONE, p_arg);
                    }
                }
             
    
                else if(strstr((char *)p_msg, RNWF_EVENT_LINK_LOSS) || strstr((char *)p_msg, RNWF_EVENT_ERROR))
                {        
                    wifi_cb_func(RNWF_DISCONNECTED, p_arg);
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_LINK_UP))
                {                
                    wifi_cb_func(RNWF_CONNECTED, p_arg);                     
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_ERROR))
                {                
                    wifi_cb_func(RNWF_CONNECT_FAILED, p_arg);   
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_SCAN_IND))
                {
                    wifi_cb_func(RNWF_SCAN_INDICATION, p_arg);   
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_SCAN_DONE))
                {                     
                    wifi_cb_func(RNWF_SCAN_DONE, p_arg); 
                    result = RNWF_PASS;
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_TIME))
                {
                    wifi_cb_func(RNWF_SNTP_UP, p_arg);   
                }
             
            }
            break;
        }
        /** Socket based Async Events*/
        case 'S':
        {
            uint32_t socket_id;
            RNWF_NET_SOCK_EVENT_t event = RNWF_NET_SOCK_EVENT_UNDEFINED;
                        
            /** Service Layer Handler */
            for(uint8_t i = 0; i < RNWF_NET_SOCK_SERVICE_CB_MAX; i++)
            {
                RNWF_NET_SOCK_CALLBACK_t netSock_cb_func = gSocket_CallBack_Handler[i];
                
                if(netSock_cb_func == NULL)
                    continue;
                
                sscanf((char *)p_arg, "%lu %*s", &socket_id);
        /* feature additions from FW v2.0.0*/    
            #ifdef RNWF_INTERFACE_DEBUG
                DBG_MSG_IF("\n\n\n\r############ Socket ID: %lu ############\n\n\n\r", socket_id);
            #endif      
                                                   
                if(strstr((char *)p_msg, RNWF_EVENT_SOCK_TLS_DONE))
                {          
                    event = RNWF_NET_SOCK_EVENT_TLS_DONE;
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_SOCK_CONNECTED))
                {
                        event = RNWF_NET_SOCK_EVENT_CONNECTED;
                }
                else if(strstr((char *)p_msg, RNWF_EVENT_SOCK_CLOSE))
                {                          
                    event = RNWF_NET_SOCK_EVENT_DISCONNECTED;                    
                } 
                else if(strstr((char *)p_msg, RNWF_EVENT_SOCK_TCP_RECV))
                {
                    uint16_t rx_len = 0;                       
                    sscanf((char *)p_arg, "%*lu %u", &rx_len);  
                    p_arg = (uint8_t *)&rx_len;                    
                    event = RNWF_NET_SOCK_EVENT_READ;
                }    
                else if(strstr((char *)p_msg, RNWF_EVENT_SOCK_ERROR))
                {
                    event = RNWF_NET_SOCK_EVENT_ERROR;
                }

                netSock_cb_func(socket_id, event, p_arg);                    
            }
            break;
        }

        /** MQTT based Async Events*/
        case 'M':
        {
            break;
        }
        default:
        {
            break;  
        }
               
    }
    
    return result;
}


RNWF_RESULT_t RNWF_RESPONSE_Trim(uint8_t *buffer)
{
    int16_t idx = strlen((char *)buffer);

    while(idx-- >= 0)
    {
        if(buffer[idx] == '\r')
        {
            buffer[idx] = '\0';
            return RNWF_PASS;
        }
    }
    return RNWF_FAIL;
}

uint16_t RNWF_IF_Read(uint8_t *buffer, uint16_t len)
{
    uint16_t read_cnt = 0;
    g_interface_timeout = RNWF_INTERFACE_TIMEOUT;
    while((read_cnt < len) && g_interface_timeout)
    {
        g_interface_timeout--;
        //later make it timeout        
        if(UART2.IsRxReady())
        {
            buffer[read_cnt++] = UART2.Read();
        }
    }
    return read_cnt;
}

RNWF_RESULT_t RNWF_RAW_Write(uint8_t *buffer, uint16_t len)
{
    // Write the bytes to Interface
    RNWF_IF_Write(buffer, len);
    //check the response
    return RNWF_CMD_RSP_Send(NULL, NULL, NULL, NULL);
}

void RNWF_EXIT_RAW_Mode(void)
{
    uint8_t rawExitCmd[] = "+++";
    RNWF_RAW_Write(rawExitCmd, 3);
}


int16_t RNWF_RAW_Read(uint8_t *buffer, uint16_t len)
{
    uint8_t tempBuf[4] = {0, 0, 0, 0};
    int16_t result = RNWF_TIMEOUT;

    if((result = RNWF_IF_Read(buffer, len)) == len) // Complete response is copied
    {
        if(RNWF_IF_Read(tempBuf, 4) == 4)
        {
            if(memcmp(tempBuf, "OK\r\n", 4) == 0)
            {
                return result;
            }                       
        }
    }
    RNWF_EXIT_RAW_Mode();
    return result;
}

uint16_t RNWF_IF_Write(uint8_t *buffer, uint16_t len)
{
    while(len > 0)
    {
        if(UART2.IsTxReady()) 
        {
            UART2.Write(*buffer++); 
            while(!UART2.IsTxDone());            
            len--;
        }
    }

    return RNWF_PASS;
}


uint8_t async_buf[RNWF_IF_ASYCN_MSG_MAX];

RNWF_RESULT_t RNWF_EVENT_Handler(void)
{   
    uint8_t *ptr_async;            
    while(IF_RX_Q_DEQUEUE(&ptr_async) != false)
    {                   
        strcpy((char *)async_buf, (const char *)ptr_async);        
        IF_BUF_Q_ENQUEUE(ptr_async);        
        
        char *token = strtok((char *)async_buf, "\r+");        
        while(token != NULL)
        {            
            if(strlen(token) > 1)
            {
                //DBG_MSG_IF("[Async token %s:%d]\n", token, strlen(token));
                RNWF_IF_ASYNC_Handler((uint8_t *)token);            
            }
            token = strtok(NULL, "\r+");            
        }
    }
    if(UART2.IsRxReady())
        RNWF_CMD_RSP_Send(NULL, NULL, NULL, NULL);
    return RNWF_PASS;
}

int16_t RNWF_CMD_RSP_Send(const char *cmd_complete, const char *delimeter, uint8_t *response, const char *format, ...)
{
    static uint8_t g_if_buffer[RNWF_INTERFACE_LEN_MAX];
    uint8_t *p_frame = g_if_buffer;
    uint16_t result = RNWF_PASS;
    size_t cmd_len;
    volatile int16_t rsp_len = 0;        
    uint16_t offset =  0;
    va_list args;   
    //memset(g_interface_send_buffer, 0, RNWF_INTERFACE_LEN_MAX);
    /* Get the variable arguments in va_list */
    //memset(g_if_buffer, 0, RNWF_INTERFACE_LEN_MAX);
    if(format != NULL)
    {
        RNWF_IS_INTERFACE_BUSY();

        va_start( args, format );                  
        cmd_len = vsnprintf((char *)g_if_buffer, RNWF_INTERFACE_LEN_MAX, format, args);        
        va_end(args); 
#ifdef RNWF_INTERFACE_DEBUG        
        DBG_MSG_IF("cmd[%d] -> %s\n", cmd_len, p_frame);
#endif /* RNWF_INTERFACE_DEBUG */        

        while(*p_frame != '\0')
        {
            if(UART2.IsTxReady()) 
            {
                UART2.Write(*p_frame++); 
                while(!UART2.IsTxDone());
            }
        }
        memset(g_if_buffer, 0, cmd_len);
        if(response != NULL)
            response[0] = '\0';
        
    }    

    g_interface_timeout = RNWF_INTERFACE_TIMEOUT;
    while(g_interface_timeout--) //later make it timeout
    {        
        if(UART2.IsRxReady())
        {      
            if(rsp_len < RNWF_INTERFACE_LEN_MAX)
            {
                g_if_buffer[rsp_len++] = UART2.Read();                                  
            }
            else
            {
                //truncate the buffer till '\r\n'            
                g_if_buffer[rsp_len-3] = g_if_buffer[rsp_len-2];
                g_if_buffer[rsp_len-2] = g_if_buffer[rsp_len-1];
                rsp_len = rsp_len-1;    //
            }
            //RAW mode
            if(rsp_len == 1)
            {
                if(g_if_buffer[rsp_len-1] == '#')
                {                
                #ifdef RNWF_INTERFACE_DEBUG       
                    printf("RAW Mode!\n");
                #endif /* RNWF_INTERFACE_DEBUG */                                     
                    result = RNWF_RAW;
                    break;
                }
            }
            
            if((rsp_len > 1) && (g_if_buffer[rsp_len - 1] == '\n') && (g_if_buffer[rsp_len - 2] == '\r'))
            {                
                if((rsp_len > 3) && ((g_if_buffer[rsp_len - 3] == 'K') && (g_if_buffer[rsp_len - 4] == 'O')))
                {
                    g_if_buffer[rsp_len-1] = '\0';
                    g_if_buffer[rsp_len-2] = '\0';

#ifdef RNWF_INTERFACE_DEBUG
        DBG_MSG_IF("rsp[%d] -> %.*s\n", rsp_len, rsp_len, g_if_buffer);
#endif /* RNWF_INTERFACE_DEBUG */

                    if(response != NULL)
                    {
                        if(delimeter != NULL)
                        {
                            uint8_t *token, *rsp_ptr;

                            rsp_ptr = (uint8_t *)strstr((char *)g_if_buffer, delimeter);
                            offset =  strlen(delimeter);
                            token = (uint8_t *)strtok((char *)rsp_ptr, "\r\n");

                            /* walk through other tokens */
                            while( token != NULL ) {
                                if(strstr((char *)token, delimeter))
                                {
                                    strcat((char *)response, " \0");
                                    strcat((char *)response, (char *)token+offset);
                                }
                                token = (uint8_t *)strtok(NULL, "\r\n");
                            }
                        }
                        else
                        {
                            g_if_buffer[rsp_len-5] = '\0';
                            memcpy((char *)response, (char *)g_if_buffer, rsp_len-5);
                            result = rsp_len-5;
                        }
                    }
                    break;
                }
                else
                {
                    if((g_if_buffer[0] == 'E') && (g_if_buffer[1] == 'R' && (g_if_buffer[2] == 'R')))
                    {
                        g_if_buffer[rsp_len-1] = '\0';
                        g_if_buffer[rsp_len-2] = '\0';
#ifdef RNWF_INTERFACE_DEBUG
                        printf("%s\r\n", g_if_buffer);
#endif
                        if(response != NULL)
                        {
                            strcpy((char *)response, (char *)g_if_buffer+sizeof(RNWF_AT_ERROR));
                        }
                        result = RNWF_FAIL;
                        break;
                    }

                }
                
                if((g_if_buffer[0] == '\r') && (g_if_buffer[1] == '+'))
                {
                    uint8_t *async_buf;
                    g_if_buffer[rsp_len] = '\0';
                    if(IF_BUF_Q_DEQUEUE(&async_buf))
                    {
                        memset(async_buf, 0, RNWF_IF_ASYCN_MSG_MAX);
                        strncpy((char *)async_buf, (char *)g_if_buffer, RNWF_IF_ASYCN_MSG_MAX);
#ifdef RNWF_INTERFACE_DEBUG
                        DBG_MSG_IF("AEC[%d] -> %s\n", rsp_len, async_buf);
#endif /* RNWF_INTERFACE_DEBUG */
                        IF_RX_Q_ENQUEUE(async_buf);
                    }
                    else
                    {
#ifdef RNWF_INTERFACE_DEBUG
                        DBG_MSG_IF("No Free-Q\n");
#endif /* RNWF_INTERFACE_DEBUG */
                    }

                    if(response != NULL || format != NULL)
                    {
                        rsp_len = 0;
                        continue;
                    }
                    break;
                }
            }
        }
    }

    if (g_interface_timeout == -1)
        result = RNWF_TIMEOUT;

    RNWF_SET_INTERFACE_FREE();
    return result;
}



RNWF_RESULT_t RNWF_IF_Init(void)
{
    if_q_init(&g_if_free_q);
    if_q_init(&g_if_rx_q);
    
    /* Fill up the free queue will all the buffer entries */
    for(uint8_t *pMem_addr = g_async_buffer; pMem_addr < (g_async_buffer + RNWF_IF_ASYCN_BUF_MAX); pMem_addr+=RNWF_IF_ASYCN_MSG_MAX)
    {
        IF_BUF_Q_ENQUEUE(pMem_addr);
    }

    RNWF_IF_SW_Reset();
    while(RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_ECHO_OFF, NULL) != RNWF_PASS)

    printf("Waiting for RNWF...\r\n");
    
    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_ECHO_OFF, NULL);
    
    return RNWF_PASS;
}

/* *****************************************************************************
 End of File
 */
