 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/reset/rstctrl.h"
#include "mcc_generated_files/rnwf/rnwf_app.h"
#include "mcc_generated_files/rnwf/rnwf_system_service.h"
#include "mcc_generated_files/rnwf/rnwf_net_service.h"
#include "mcc_generated_files/rnwf/rnwf_ota_service.h"
#include "sst2x_driver.h"

/*
    Main application
*/

uint8_t app_buf[OTA_BUF_LEN_MAX];

void APP_SW_RESET_Handler(void)
{
//    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_RESET, NULL);    
    DFU_Reset();            
    DELAY_milliseconds(5000);    
    RSTCTRL_reset();    
}

int main(void)
{
    uint8_t Device_Id=0x00;
    uint8_t Device_Type=0x00;    
    uint8_t Manufacturer_Id=0x00;    
    RNWF_OTA_HDR_t otaHdr;
    
    SYSTEM_Initialize();
    PB2_SetInterruptHandler(APP_SW_RESET_Handler);
    // Main Section();

    printf("%s", "##############################################\n");
    printf("%s", "  Welcome RNWF02 WiFi Host Assisted OTA Demo  \n");
    printf("%s", "##############################################\n"); 
    
    Jedec_ID_Read(&Manufacturer_Id, &Device_Type, &Device_Id); 
    printf("SPI Manufacturer ID = 0x%02X\r\n", Manufacturer_Id);
    printf("SPI Device Type = 0x%02X\r\n", Device_Type);
    printf("SPI Device ID = 0x%02X\r\n", Device_Id); 
    if((Device_Id != SST26VF032B_ID) && (Device_Id != SST26VF016B_ID) && (Device_Id != SST26VF064B_ID))    
    {
        printf("No valid SPI Flash found!\r\nConnect SPI MikroBus on slot1 and reset!\r\n");
        while(1);
    }
    
    RNWF_IF_Init();
    
    if(RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SW_REV, app_buf) != RNWF_PASS)
    {        
        HighSpeed_Read_Cont(OTA_FLASH_IMAGE_START, sizeof(RNWF_OTA_HDR_t), (char *)&otaHdr.seq_num);
        printf("Image details in the Flash\r\n");
        printf("Sequence Number 0x%X\r\n", (unsigned int)otaHdr.seq_num);
        printf("Start Address 0x%X\r\n", (unsigned int)otaHdr.start_addr);
        printf("Image Length 0x%X\r\n", (unsigned int)otaHdr.img_len);
        if(otaHdr.seq_num != 0xFFFFFFFF && otaHdr.start_addr != 0xFFFFFFFF && otaHdr.img_len != 0xFFFFFFFF)        
        {                         
            APP_OTA_Program(OTA_FLASH_IMAGE_START, otaHdr.img_len);
        }
        else
        {
            printf("Error: Module is Bricked!");
        }        
    }
    else
    {
        printf("Software Revision: %s\n", app_buf);
    }

    RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_DEV_INFO, app_buf);
    printf("Device Info: %s\n", app_buf);            
        
    RNWF_SYSTEM_SrvCtrl(RWWF_SYSTEM_GET_WIFI_INFO, app_buf);    
    printf("%s\n", app_buf);
    
	RNWF_APP_Initialize();    // Note: This function call never returns because of an infinite while-loop

    while(1)
    {
    }    
}