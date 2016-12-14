#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

#include "testuser_key.c"
#include "testuser_cert.c"

unsigned char username[] = "testuser";

os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void loop(os_event_t *events);

//Main code function
static void ICACHE_FLASH_ATTR
loop(os_event_t *events)
{
    struct ip_info ipconfig;
    wifi_get_ip_info(STATION_IF, &ipconfig);
    if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
        os_printf("IP found\n\r");
    } else {
        os_printf("No IP found\n\r");
    }
    os_delay_us(10000);
    system_os_post(user_procTaskPrio, 0, 0 );
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    char ssid[32] = SSID;
    char password[64] = SSID_PASSWORD;
    struct station_config stationConf;
    uint32 free;

    // Set serial baud rate to 9600
    uart_div_modify(0, UART_CLK_FREQ/9600);

    os_printf("SDK version:%s\n", system_get_sdk_version());

    free = system_get_free_heap_size();

    //Set station mode
    wifi_set_opmode(STATIONAP_MODE); 

    if( wifi_station_set_cert_key(testuser_cert_pem, testuser_cert_pem_len, testuser_key_pem, testuser_key_pem_len, NULL, 0) == 0 ) {
        os_printf("certificate and key set\n\r");
    }

    //Set ap settings
    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 64);
    wifi_station_set_config(&stationConf);

    wifi_station_connect();

    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );

    wifi_station_clear_cert_key();
    wifi_station_clear_username();
}
