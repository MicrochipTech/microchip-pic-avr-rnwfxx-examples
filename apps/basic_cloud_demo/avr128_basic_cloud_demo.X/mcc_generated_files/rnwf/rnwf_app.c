 /*
 * MAIN Generated Driver File
 * 
 * @file rnwf_app.c
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

#include "rnwf_app.h"
#include "rnwf_wifi_service.h"
#include "rnwf_mqtt_service.h"

/*
    Main application
*/


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
#define MQTT_PUBLISH_INTERVAL   600      //100msec units

RNWF_MQTT_CFG_t mqtt_cfg = {
    .url = "test.mosquitto.org",
    .clientid = CLIENT_ID,
    .username = "",
    .password = "",
    .port = 1883,
    .azure_dps = 0
};

void APP_Cloud_SUB_Handler(char *p_str)
{
    if(p_str != NULL) {
        DBG_MSG_MQTT("RNWF02 <- %s\r\n", p_str);
    } else {
        DBG_MSG_MQTT("%s\r\n", p_str);
    }
    return;
}

void APP_Cloud_SUBACK_Handler(void) 
{
    if(subscribe_list[subCnt] != NULL) {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(SUBSCRIBE_QOS_VALUE, app_buf);
    }    
    return;
}

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

RNWF_RESULT_t APP_MQTT_Callback(RNWF_MQTT_EVENT_t event, uint8_t *p_str)
{   
  
    switch(event)
    {
        case RNWF_MQTT_CONNECTED:
        {                        
            printf("MQTT Connected\r\n");
            g_isMqttConnected = MQTT_CONNECTED;                   
            APP_MQTT_Publish(MQTT_PUB_TOPIC, "RNWF02 -> I am Up!");
            if(subscribe_list[subCnt] != NULL) {
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


void APP_WIFI_Callback(RNWF_WIFI_EVENT_t event, uint8_t *p_str)
{
    switch(event)
    {
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
        case RNWF_DHCP_DONE:
        {
            printf("DHCP IP:%s\n", &p_str[2]); 
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_SET_CALLBACK, APP_MQTT_Callback);
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONFIG, (void *)&mqtt_cfg);
            RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, APP_MQTT_Callback);
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
        default:
        {
            break;
        }
    }
}


void RNWF_APP_Initialize(void)
{    

 
    /* Wi-Fii Connectivity */
    RNWF_WIFI_PARAM_t wifi_sta_cfg = {RNWF_WIFI_MODE_STA, HOME_AP_SSID, HOME_AP_PASSPHRASE, HOME_AP_SECURITY, 1};    
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