/**
 * Wi-Fi provisioning service header file
 * 
 * @file rnwf_provision_service.h
 * 
 * @defgroup provision_service Wi-Fi Provision Config Service
 * 
 * @brief This header file contains data types for Wi-fi provisioning service
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
#ifndef RNWF_PROVISION_SERVICE_H
#define	RNWF_PROVISION_SERVICE_H

#define DBG_MSG_PROV(args, ...)    printf("[PROV]:"args, ##__VA_ARGS__)

/* Wi-Fi Configuration */
#define PROV_AP_SSID        "ProvMode-AP"
#define PROV_AP_PASSPHRASE  "Password@123"
#define PROV_AP_SECURITY     RNWF_WPA2_MIXED

/**
 * @ingroup provision_service
 * @brief Wi-Fi Provision service list
 * @enum RNWF_PROV_SERVICE_t
 */
typedef enum 
{
    RNWF_PROV_ENABLE,               /**<Configure the MQTT Broker parameters*/            
    RNWF_PROV_DISABLE,
    RNWF_PROV_SET_CALLBACK,     /**<Set the Application callback for Provisioning Events*/
            
}RNWF_PROV_SERVICE_t;

/**
 * @ingroup provision_service
 * @brief Provisioning events
 * @enum RNWF_PROV_EVENT_t
 */
typedef enum {
    RNWF_PROV_COMPLTE,      /**<Provisionging complete*/
    RNWF_PROV_FAILURE,      /**<Provisionging Failure*/    
}RNWF_PROV_EVENT_t;

/**
 * @ingroup provision_service
 * @brief       Wi-Fi Prov callback function type
 * @param[in]   event   Event of type RNWF_WIFI_EVENT_t   
 * @param[in]   msg     Received data related to the passed event     
 * @return None
 */
typedef void (*RNWF_PROV_CALLBACK_t)(RNWF_PROV_EVENT_t event, uint8_t *msg);

/**
 * @ingroup provision_service
 * @brief       Wi-Fi Provision Service layer API
 * @param[in]   request      Prov Service request
 * @param[in]   input        Input parameters needed for requested service  
 * @retval      RNWF_PASS    When successful
 * @retval      RNWF_FAIL    When fails
 */
RNWF_RESULT_t RNWF_PROV_SrvCtrl(RNWF_PROV_SERVICE_t request, void *input);
   

#define PROV_BUF_LEN_MAX     128

#define APP_WIFI_PROV_CONFIG_ID     "apply"
#define APP_WIFI_PROV_DONE_ID       "finish"

         
#endif	/* RNWF_PROVISION_SERVICE_H */