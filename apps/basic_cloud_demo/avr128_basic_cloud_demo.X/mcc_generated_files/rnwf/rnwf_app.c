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

#include "rnwf_app.h"
#include "rnwf_wifi_service.h"
#include "rnwf_system_service.h"
#include "rnwf_mqtt_service.h"

/*
    Main application
*/


/* feature additions from FW v2.0.0*/
uint8_t appBuffer[APP_BUFFER_SZ];


/* feature addition from FW v2.0.0*/
/* Holds IP address of the device */
//static uint8_t g_DevIp[50];

/* MQTT Connection States */
#define MQTT_DISCONNECTED       0
#define MQTT_CONNECTING         1
#define MQTT_CONNECTED          2

uint8_t g_isMqttConnected = MQTT_DISCONNECTED;

/* Application buffer */
uint8_t app_buf[APP_BUFFER_SIZE_MAX];
/* MQTT Subscribe Topic Name List */
static const char *subscribe_list[] = {"mchp/rnwf02/to", NULL, NULL, NULL, NULL};
/* Application specific Global variables */
static uint8_t subCnt;                  //index to iterate subscribe topic list
uint32_t gSysTickCount;
#define MQTT_PUBLISH_INTERVAL   30000      //100msec units
RNWF_MQTT_CFG_t mqtt_cfg = {
    .url = "test.mosquitto.org",
    .clientid = CLIENT_ID,
    .username = "",
    .password = "",
    .port = 1883,
    .azure_dps = 0
};


/**
 * @ingroup rnwf_app
 * @brief Function to handle subscribing to topics
 * @param[in] p_str         string pointer
 * @return None
 */
void APP_Cloud_SUB_Handler(char *p_str)
{
    /* Complete app-specific implementation here */
        if(p_str != NULL) {
        DBG_MSG_MQTT("RNWF02 <- %s\r\n", p_str);
    } else {
        DBG_MSG_MQTT("%s\r\n", p_str);
}
    return;
}

/**
 * @ingroup rnwf_app
 * @brief Function to subscribe to topics in subscribe_list
 * @param None
 * @return None
 */
void APP_Cloud_SUBACK_Handler(void)
{
    if(subscribe_list[subCnt] != NULL) {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(SUBSCRIBE_QOS_VALUE, app_buf);
}
    return;
}

/**
 * @ingroup rnwf_app
 * @brief Cloud task function
 * @param None
 * @return None
 */
void APP_Cloud_Task(void)
{
    uint8_t pub_buf[64];
    static uint32_t pub_cnt = 0;

    if((gSysTickCount % MQTT_PUBLISH_INTERVAL) == 0)
    {                                                   
        pub_cnt++;
        sprintf(pub_buf, "RNWF -> Msg #%d", pub_cnt);
        APP_MQTT_Publish(MQTT_PUB_TOPIC, pub_buf);                    
}
    return;
}

/* feature addition from FW v2.0.0*/
    static uint8_t isSockOpen = 0;      //guard condition to open a socket


RNWF_RESULT_t APP_MQTT_Publish(const char *top, const char *msg)
{    
    RNWF_MQTT_FRAME_t mqtt_pub;    
    mqtt_pub.isNew = NEW_MSG;
    mqtt_pub.qos = MQTT_QOS0;
    mqtt_pub.isRetain = NO_RETAIN;
    mqtt_pub.topic = top;
    mqtt_pub.message = msg;        
    return RNWF_MQTT_SrvCtrl(RNWF_MQTT_PUBLISH, (void *)&mqtt_pub);              
}     

/**
 * @ingroup rnwf_app
 * @brief MQTT Callback function for connect, disconnect, subscribe events
 * @param[in] event     MQTT event
 * @param[in] p_str     string pointer
 * @retval              RNWF_PASS Requested event is handled successfully
 * @retval              RNWF_FAIL Requested event has failed
 */
RNWF_RESULT_t APP_MQTT_Callback(RNWF_MQTT_EVENT_t event, uint8_t *p_str)
{   
    switch(event)
    {
        case RNWF_MQTT_CONNECTED:
        {
            printf("\n\rMQTT Connected\r\n");
            g_isMqttConnected = MQTT_CONNECTED;
            APP_MQTT_Publish(MQTT_PUB_TOPIC, "RNWF02 -> I am Up!");
            if(subscribe_list[subCnt] != NULL)
            {
                sprintf(app_buf, "%s", subscribe_list[subCnt++]);
                RNWF_MQTT_SrvCtrl(SUBSCRIBE_QOS_VALUE, app_buf);
        }
        }
        break;
        case RNWF_MQTT_SUBCRIBE_ACK:
        {
            CLOUD_SUBACK_HANDLER();           
        }
        break;
        case RNWF_MQTT_SUBCRIBE_MSG:
        {
//            printf("RNWF02 <- %s\r\n", p_str);
            CLOUD_SUBMSG_HANDLER(p_str);
        }
        break;
        case RNWF_MQTT_DISCONNECTED:
        {
            printf("MQTT - Reconnecting...\r\n"); 
            g_isMqttConnected = MQTT_DISCONNECTED;          
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);
            g_isMqttConnected = MQTT_CONNECTING;         
        }
        break;
        default:
        break;
    }
    return RNWF_PASS;
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
            if(isSockOpen == 0)
            {
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_SET_CALLBACK, APP_MQTT_Callback);
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONFIG, (void *)&mqtt_cfg);
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);
                isSockOpen = 1;
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

    while(1)
    {  
        if(g_isMqttConnected == MQTT_CONNECTED)     
        {                
            CLOUD_STATE_MACHINE();                                                              
        }
        RNWF_EVENT_Handler();
    }    
}