/*
 *  Gestion de la WiFi de la Station Sol du Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  wifi.cpp
 */
#include "main.h"

/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

// Initialise le WiFi
void initialise_wifi(const char ssid[], const char pass[])
{
	//Initialize the TCP Stack
	tcpip_adapter_init();
	wifi_event_group = xEventGroupCreate();

	//Initialize the system event handler
	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

	//Config the wifi
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

	//Config RAM as the wifi parameters storage
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );

	//configure the wifi connection
	wifi_config_t wifi_config;
	::memset(&wifi_config, 0, sizeof(wifi_config));
	::memcpy(wifi_config.sta.ssid, ssid, strlen(ssid));
	::memcpy(wifi_config.sta.password, pass, strlen(pass));
	wifi_config.sta.bssid_set = 0;

	ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);

	//Wifi as Station Mode
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );

	printf("\nStarting to Connect to Wifi Acess Point %s\n",wifi_config.sta.ssid);
	ESP_ERROR_CHECK( esp_wifi_start() );

}


esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
    	printf("Connected to A WiFi AP!\n");
		//print the local IP address
		tcpip_adapter_ip_info_t ip_info;

		ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
		printf("IP Address: %s\n", ip4addr_ntoa(&ip_info.ip));
		printf("Subnet Mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
		printf("Gateway:     %s\n", ip4addr_ntoa(&ip_info.gw));

        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        // This is a workaround as ESP32 WiFi libs don't currently auto-reassociate
    	printf("Disconnected !\n");
    	printf("\tReason : %d\n", event->event_info.disconnected.reason);


        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK( esp_wifi_connect() );
        break;
    default:
        break;
    }
    return ESP_OK;
}
