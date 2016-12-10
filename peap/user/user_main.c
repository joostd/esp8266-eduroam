#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

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
        os_printf("\rNo IP found");
    }
    os_delay_us(100000);
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
    static u8 ent_username[] = "john";
    static u8 ent_password[] = "secret";

    // Set serial baud rate to 9600
    uart_div_modify(0, UART_CLK_FREQ/9600);

    os_printf("SDK version:%s\n", system_get_sdk_version());

    free = system_get_free_heap_size();

    //Set station mode
    wifi_set_opmode(STATION_MODE); 

    //Set ap settings
    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 64); // skip this
    wifi_station_set_config(&stationConf);

    wifi_station_set_wpa2_enterprise_auth(1); 

    if(wifi_station_set_enterprise_username (ent_username, sizeof(ent_username))){
      os_printf("\r\nusername set fail\r\n");
    }
    if(wifi_station_set_enterprise_password (ent_password, sizeof(ent_password))){
      os_printf("\r\npassword set fail\r\n");
    }


    wifi_station_connect();

    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );

}
