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
#include "rnwf_net_service.h"

/*
    Main application
*/


/* feature additions from FW v2.0.0*/
uint8_t appBuffer[APP_BUFFER_SZ];


/* feature addition from FW v2.0.0*/
/* Holds IP address of the device */
static uint8_t g_DevIp[50];

/* Button Press Event */
bool    g_ButtonPress = false;
 
/* MQTT Connection States */
#define MQTT_DISCONNECTED       0
#define MQTT_CONNECTING         1
#define MQTT_CONNECTED          2

uint8_t g_isMqttConnected = MQTT_DISCONNECTED;

/* System Tick Counter for 1mSec*/
uint32_t g_SysTickCount;

static int16_t g_RebootDelay = -1;
static uint16_t g_ReportRate = APP_CLOUD_REPORT_INTERVAL;

/* Application buffer */
uint8_t app_buf[APP_BUFFER_SIZE_MAX];
/* MQTT Subscribe Topic Name List */
static const char *subscribe_list[] = {"devices/rnwf02-temp-node1/messages/devicebound/#", "$iothub/twin/res/#", "$iothub/methods/POST/#", NULL, NULL};
static uint8_t subCnt;
/* TLS Configuration details */
const char *cloud_tls_cfg[] = {"DigiCertGlobalRootG2", "rnwf02-temp-node1", "rnwf02-temp-node1", 0, "apatel.azure-devices.net", "*.azure-devices.net", true, true};
RNWF_MQTT_CFG_t mqtt_cfg = {
    .url = "apatel.azure-devices.net",
    .clientid = CLIENT_ID,
    .username = "apatel.azure-devices.net/rnwf02-temp-node1/?api-version=2021-04-12",
    .password = "",
    .port = 8883,
    .tls_conf = cloud_tls_cfg,
    .tls_idx = RNWF_NET_TLS_CONFIG_2,
    .azure_dps = 0
};

uint8_t *APP_GET_IP_Address(void)
{
    return g_DevIp;
}

void APP_SYS_Tick(void)
{       
    g_SysTickCount++;     
}

void APP_SW_Handler(void)
{
    static uint32_t press_ticks;    
    if(!IO_PB2_GetValue())
    {
        press_ticks = g_SysTickCount;
    }
    else
    {
        if((g_SysTickCount - press_ticks) > APP_SW_LONG_PRESS_DELAY)
        {
            APP_RESET_Device();
        }
        else
        {
            g_ButtonPress = 1;            
        }
    }    
}

void APP_AZURE_BUTTON_Telemetry(uint32_t press_count)
{            
    snprintf(app_buf, sizeof(app_buf), AZURE_FMT_BUTTON_TEL, press_count);
    printf("Telemetry ->> buttonEvent count %d\r\n", press_count);
    APP_MQTT_Publish(AZURE_PUB_TELEMETRY, app_buf);
}

/**
 * @ingroup rnwf_app
 * @brief   Function to publish telemetry count data to Azure cloud
 * @param[in] counter       counter value
 * @return None
 */
void APP_AZURE_COUNTER_Telemetry(uint32_t counter)
{            
    snprintf(app_buf, sizeof(app_buf), AZURE_FMT_COUNTER_TEL, counter);
    printf("Telemetry ->> counter count %d\r\n", counter);
    APP_MQTT_Publish(AZURE_PUB_TELEMETRY, app_buf);
}

/**
 * @ingroup rnwf_app
 * @brief   Function to subscribe to topics in subscribe_list
 * @param None
 * @return None
 */
void APP_AZURE_SUBACK_Handler(void)
{    
    if(subscribe_list[subCnt] != NULL)
    {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, app_buf);
    }
    else
    {        
        // get device twin
        APP_MQTT_Publish(AZURE_PUB_TWIN_GET, "");
    }
}

/**
 * @ingroup rnwf_app
 * @brief Azure task function
 * @param None
 * @return None
 */
void APP_AZURE_Task(void)
{     
    static uint32_t press_count = 0;
    static uint32_t counter = 0;  
    /* Implement app specific Azure_Task() method here */  
    /* set periodic events for telemetry */
    if(!(g_SysTickCount % APP_SYS_TICK_COUNT_1SEC))
    {            
        if(!(g_SysTickCount % g_ReportRate))
        {
            APP_AZURE_COUNTER_Telemetry(counter++);     //this will continuously trigger telemetry action
            APP_AZURE_BUTTON_Telemetry(press_count);
        }
        if(g_RebootDelay > 0)
        {            
            printf("Rebooting in %d seconds..\r\n", g_RebootDelay);
            g_RebootDelay--;
            if(g_RebootDelay == 0)
                APP_RESET_Device();            
        }
    }
    if(g_ButtonPress)
    {        
        APP_AZURE_BUTTON_Telemetry(++press_count);
        g_ButtonPress = 0;
    }
    if(!subCnt && subscribe_list[subCnt] != NULL)
    {
        sprintf(app_buf, "%s", subscribe_list[subCnt++]);
        RNWF_MQTT_SrvCtrl(RNWF_MQTT_SUBSCRIBE_QOS0, app_buf);            
    }
}

/**
 * @ingroup rnwf_app
 * @brief Function that executes based on input string event
 * @param[in] p_str         pointer to string
 * @return None
 */
void APP_AZURE_SUB_Handler(char *p_str)
{
    char *end_ptr = NULL;
    const char *ver_ptr = NULL, *led_ptr = NULL, *rate_ptr = NULL;
    if(strstr(p_str, "twin/res/200"))
    {
        /* Complete app-specific implementation here */
        sprintf(app_buf, "{"AZURE_MSG_IPADDRESS"}", APP_GET_IP_Address());
        APP_MQTT_Publish(AZURE_PUB_PROPERTY, app_buf);
    }
    if(strstr(p_str, "POST"))
    {
        /* Complete app-specific implementation here */
        char *reboot_ptr = (char *)strstr(p_str, AZURE_DEALY_TAG);
        char *echo_ptr = (char *)strstr(p_str, AZURE_ECHO_TAG);   
        char *rid_ptr = (char *)strstr(p_str, "rid="); 
        if(rid_ptr != NULL)
        {
            end_ptr = (char *)strstr(rid_ptr, "\" \"");
            *end_ptr = '\0';
            //+1 for null character
            uint16_t pubLen = sprintf(app_buf, AZURE_PUB_CMD_RESP, rid_ptr) + 1;             
            if(reboot_ptr != NULL)
            {                 
                end_ptr = (char *)strstr(reboot_ptr, "S\"}");
                *end_ptr = '\0';
                g_RebootDelay = atoi((char *)reboot_ptr+strlen(AZURE_DEALY_TAG));
                printf("Reboot delay = %d Sec\r\n", g_RebootDelay);
                sprintf(app_buf+pubLen, AZURE_FMT_DELAY_RSP, g_RebootDelay);                
                APP_MQTT_Publish(app_buf, app_buf+pubLen);
            }
            if(echo_ptr != NULL)
            {                 
                echo_ptr += strlen(AZURE_ECHO_TAG);
                end_ptr = (char *)strstr(echo_ptr, "\\\"}");
                *end_ptr = '\0';            
                printf("Echo = %s\r\n", echo_ptr);
                sprintf(app_buf+pubLen, AZURE_FMT_ECHO_RSP, echo_ptr);                
                APP_MQTT_Publish(app_buf, app_buf+pubLen);
            }
        }  
    }
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
            DBG_MSG_MQTT("Azure IoT-Hub Connection Successful!\r\n");
            g_isMqttConnected = MQTT_CONNECTED;
        }
        break;
        case RNWF_MQTT_SUBCRIBE_ACK:
        {
            CLOUD_SUBACK_HANDLER();           
        }
        break;
        case RNWF_MQTT_SUBCRIBE_MSG:
        {
            printf("RNWF02 <- %s\r\n", p_str);
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
            if(g_isMqttConnected < MQTT_CONNECTING)
            {            
                printf("SNTP UP:%s\n", &p_str[2]);
                printf("Connecting to the Cloud\r\n");
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_SET_CALLBACK, APP_MQTT_Callback);
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONFIG, (void *)&mqtt_cfg);
                RNWF_MQTT_SrvCtrl(RNWF_MQTT_CONNECT, NULL);
                g_isMqttConnected = MQTT_CONNECTING;
            }
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
    // Configure and Enable NTP Client                                        
        const char sntp_url[] =  "216.239.35.4";    
        RNWF_SYSTEM_SrvCtrl(RNWF_SYSTEM_SET_SNTP, sntp_url);           
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