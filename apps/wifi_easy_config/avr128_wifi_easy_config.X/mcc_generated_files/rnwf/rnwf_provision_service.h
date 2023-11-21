/*
 * MAIN Generated Driver File
 * 
 * @file rnwf_provision_service.h
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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RNWF_PROVISION_SERVICE_H
#define	RNWF_PROVISION_SERVICE_H

#define DBG_MSG_PROV(args, ...)    printf("[PROV]:"args, ##__VA_ARGS__)

/* Wi-Fi Configuration */
#define PROV_AP_SSID        "RNWF-ProvAP"
#define PROV_AP_PASSPHRASE  "password"
#define PROV_AP_SECURITY     RNWF_WPA2_MIXED

/**
 @brief Wi-Fi Provision service List
 
 */
typedef enum 
{
    RNWF_PROV_ENABLE,               /**<Configure the MQTT Broker parameters*/            
    RNWF_PROV_DISABLE,
    RNWF_PROV_SET_CALLBACK,     /**<Set the Application callback for Provisioning Events*/
            
}RNWF_PROV_SERVICE_t;


/**
 @brief Provisioning events
 
 */
typedef enum {
    RNWF_PROV_COMPLTE,                  /**<Provisionging complete*/
    RNWF_PROV_FAILURE,                  /**<Provisionging Failure*/    
}RNWF_PROV_EVENT_t;


/**
 * @brief       Wi-Fi callback function type
 * 
 * @param[out]  event One of the ::RNWF_WIFI_EVENT_t event   
 * @param[out]  msg Received data related to the passed event   
 * 
 */
typedef void (*RNWF_PROV_CALLBACK_t)(RNWF_PROV_EVENT_t event, uint8_t *msg);


RNWF_RESULT_t RNWF_PROV_SrvCtrl(RNWF_PROV_SERVICE_t request, void *input);
   

#define PROV_BUF_LEN_MAX     128

#define APP_WIFI_PROV_CONFIG_ID     "apply"
#define APP_WIFI_PROV_DONE_ID       "finish"

         
#endif	/* RNWF_PROVISION_SERVICE_H */

