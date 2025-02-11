/**
 * Application source file
 * 
 * @file rnwf_app.c
 *
 * @ingroup rnwf_app
 * 
 * @brief Application file
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

#include "rnwf_app.h"
#include "rnwf_wifi_service.h"
#include "rnwf_system_service.h"
#include "rnwf_net_service.h"
#include "rnwf_ota_service.h"
#include "../../sst2x_driver.h"

/*
    Main application
*/

extern uint8_t app_buf[OTA_BUF_LEN_MAX];
uint32_t gOta_file_size = 0;

/* feature additions from FW v2.0.0*/
uint8_t appBuffer[APP_BUFFER_SZ];




void APP_OTA_Program(uint32_t flash_addr, uint32_t flash_size)
{   
    RNWF_OTA_CHUNK_t ota_chunk = { .chunk_addr = 0x60000000, .chunk_ptr = app_buf, .chunk_size = flash_size}; 
    printf("Triggering DFU %lu\r\n", flash_size);
    RNWF_OTA_SrvCtrl(RNWF_OTA_DFU_INIT, (void *)NULL);            
    while(RNWF_OTA_SrvCtrl(RNWF_OTA_DFU_ERASE, (void *)&ota_chunk) != RNWF_PASS);
    while(flash_size)
    {                
        ota_chunk.chunk_size = (flash_size < OTA_BUF_LEN_MAX)?flash_size:OTA_BUF_LEN_MAX; 

        /* User needs to read the stored OTA image and pass-in to RNWF_OTA_SrvCtrl() */                
        HighSpeed_Read_Cont(flash_addr, ota_chunk.chunk_size, (char *)app_buf);                    
        
        RNWF_OTA_SrvCtrl(RNWF_OTA_DFU_WRITE, (void *)&ota_chunk);
        flash_size -= ota_chunk.chunk_size;
        ota_chunk.chunk_addr += ota_chunk.chunk_size;
        flash_addr += ota_chunk.chunk_size;
        printf("Remaining %lu bytes\r\n", flash_size);  
    }
}

/**
 * @ingroup rnwf_app
 * @brief OTA callback function for certain download events
 * @param[in] event     download event
 * @param[in] p_str     string pointer
 * @return None
 */
void APP_OTA_Callback(RNWF_OTA_EVENT_t event, void *p_str)
{
    static uint32_t flash_addr = OTA_FLASH_IMAGE_START;
    switch(event)
    {
        case RNWF_EVENT_MAKE_UART:
            break;
        case RNWF_EVENT_DWLD_START:
        {
            printf("Total Size = %lu\r\n", *(uint32_t *)p_str); 
            printf("Erasing the SPI Flash\r\n");
            WREN();
            Chip_Erase();
            Wait_Busy();
            SPI_Global_Block_Protection_Unlock();
            printf("Erasing Complete!\r\n");
        }
        break;
        case RNWF_EVENT_DWLD_DONE:
        {                                                 
            printf("Download Success!= %lu bytes\r\n", *(uint32_t *)p_str);
            APP_OTA_Program(OTA_FLASH_IMAGE_START, *(uint32_t *)p_str);            
            APP_SW_RESET_Handler();
        }
        break;        
        case RNWF_EVENT_FILE_CHUNK:
        {
            volatile RNWF_OTA_CHUNK_t *ota_chunk = (RNWF_OTA_CHUNK_t *)p_str;               
            Sector_Program(flash_addr, ota_chunk->chunk_ptr, ota_chunk->chunk_size);            
            flash_addr += ota_chunk->chunk_size;
        }    
        break;
        case RNWF_EVENT_DWLD_FAIL:
        {
            WREN();
            Chip_Erase();
            Wait_Busy();            
        }
        break;

        default:
            break;
    }
}



/**
 * @ingroup rnwf_app
 * @brief Wi-Fi callback function for Wi-Fi events like scan, SNTP, DHCP
 * @param[in] event         Wi-Fi event type
 * @param[in] p_str         string pointer
 * @return None
 */
void APP_WIFI_Callback(RNWF_WIFI_EVENT_t event, uint8_t *p_str)
{
    switch(event)
    {
        case RNWF_SNTP_UP:
        {   
            break;
        }
        case RNWF_CONNECTED:
        {
            printf("Wi-Fi Connected\n");
            break;
        }
        case RNWF_DISCONNECTED:
        {
            printf("Wi-Fi Disconnected\nReconnecting... \n");
            RNWF_WIFI_SrvCtrl(RNWF_STA_CONNECT, NULL);
            break;
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_DHCP_IPV4_DONE:
        {
            printf("\n\rDHCP IPv4: %s\n\r", &p_str[2]);
        // Enable OTA by passing the OTA buffer space
            if(RNWF_OTA_SrvCtrl(RNWF_OTA_ENABLE, (void *)app_buf) == RNWF_PASS)
            {
                printf("Successfully Enabled the OTA\r\n");
            }
            else
            {
                printf("Failed to enable the OTA\r\n");
            }
            break;       
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_DHCP_LINK_LOCAL_IPV6_DONE:
        {
            printf("\n\rDHCP link-local IPv6:%s\n\r", &p_str[2]);
            break;
        }
    /* feature additions from FW v2.0.0*/
        case RNWF_DHCP_GLOBAL_IPV6_DONE:
        {
            printf("\n\rDHCP global IPv6:%s\n\r", &p_str[2]);
            break;
        }
    /* feature additions from FW v2.0.0*/    
        case RNWF_SET_REGDOM:
        {
            RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_REGDOM, (void *)COUNTRY_CODE);
            break;
        }
        case RNWF_SCAN_INDICATION:
        {
            break;
        }
        case RNWF_SCAN_DONE:
        {
            break;
        }
        case RNWF_CONNECT_FAILED:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}


void RNWF_APP_Initialize(void)
{
/* feature additions from FW v2.0.0*/
    // Disable NTP Client
        RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_RMV_SNTP, NULL);
        RNWF_WIFI_SrvCtrl(RNWF_GET_WIFI_REGDOM, appBuffer);
    /* Wi-Fi Connectivity */
        RNWF_WIFI_PARAM_t wifi_sta_cfg = {RNWF_WIFI_MODE_STA, HOME_AP_SSID, HOME_AP_PASSPHRASE, HOME_AP_SECURITY, STA_AUTOCONNECT};    
        printf("Connecting to %s\r\n", HOME_AP_SSID);
        RNWF_WIFI_SrvCtrl(RNWF_WIFI_SET_CALLBACK, APP_WIFI_Callback);
        RNWF_WIFI_SrvCtrl(RNWF_SET_WIFI_PARAMS, &wifi_sta_cfg);
    /* RNWF Application Callback register */
        RNWF_OTA_SrvCtrl(RNWF_OTA_SET_CALLBACK, (void *)APP_OTA_Callback);

    while(1)
    {  
        RNWF_EVENT_Handler();
    }    
}