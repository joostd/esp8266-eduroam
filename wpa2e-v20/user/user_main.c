#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "gpio.h"
#include "user_interface.h"

#include "user_config.h"
#include "ca.h"

#define WIFI_LED_IO_MUX     PERIPHS_IO_MUX_GPIO2_U
#define WIFI_LED_IO_NUM     2
#define WIFI_LED_IO_FUNC    FUNC_GPIO2

void wifi_handle_event_cb(System_Event_t *evt)
{
    switch (evt->event) {
        case EVENT_STAMODE_CONNECTED:
            os_printf("[connect to ssid %s, channel %d]\n",
                evt->event_info.connected.ssid,
                evt->event_info.connected.channel);
            break;
        case EVENT_STAMODE_DISCONNECTED:
            os_printf("[disconnect from ssid %s, reason %d]\n",
                evt->event_info.disconnected.ssid,
                evt->event_info.disconnected.reason);
            break;
        case EVENT_STAMODE_AUTHMODE_CHANGE:
            os_printf("[mode: %d -> %d]\n",
                evt->event_info.auth_change.old_mode,
                evt->event_info.auth_change.new_mode);
            break;
        case EVENT_STAMODE_GOT_IP:
            os_printf("[ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR "]\n",
                IP2STR(&evt->event_info.got_ip.ip),
                IP2STR(&evt->event_info.got_ip.mask),
                IP2STR(&evt->event_info.got_ip.gw));
            break;
        default:
            os_printf("[event: %x]\n", evt->event);
            break;
    }
}

void ICACHE_FLASH_ATTR user_init(void)
{
    struct station_config wifi_config;

    uart_div_modify(0, UART_CLK_FREQ/9600);
    os_delay_us(1000000);

    wifi_status_led_install(WIFI_LED_IO_NUM, WIFI_LED_IO_MUX, FUNC_GPIO0); 
    os_printf("[SDK version:%s]\n", system_get_sdk_version());

    // station mode: 
    if( !wifi_set_opmode(STATION_MODE) ) {
        os_printf("[failed setting station mode]\n");
    }

    // set station configuration, save to flash
    os_memset(&wifi_config,0,sizeof(wifi_config));
    os_strcpy(wifi_config.ssid, SSID, os_strlen(SSID));
    //os_strcpy(wifi_config.password, PSK, os_strlen(PSK));	// only for WPA2-PSK
    os_strcpy(wifi_config.password, "", os_strlen(""));	// only for WPA2-Enterprise
    wifi_station_set_config(&wifi_config);

    // WPA2-Enterprise
    wifi_station_set_wpa2_enterprise_auth(1);
    // ignore CA Certificate when testing
    wifi_station_set_enterprise_ca_cert(ca_pem, ca_pem_len + 1);
    wifi_station_set_enterprise_username("espressif", os_strlen("espressif"));
    wifi_station_set_enterprise_password("test11", os_strlen("test11"));

    // no need to call connect() in user_init()
    //if( !wifi_station_connect() );

    wifi_set_event_handler_cb(wifi_handle_event_cb);
}
