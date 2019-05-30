/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  wifi.h
 */
#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_


#define WIFI_SSID_PORTAL "supelec-invite"
#define WIFI_PASS_PORTAL ""


//#define WIFI_SSID "STARTRAIL 9"
//#define WIFI_PASS "rennes25"

//#define WIFI_SSID "IoT-Broker-N3"
//#define WIFI_PASS "raspberry"

//#define WIFI_SSID "iPhone de DELFOSSE"
//#define WIFI_PASS "9my58buvc6pks"



/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
static const int CONNECTED_BIT = BIT0;


void initialise_wifi(const char ssid[], const char pass[]);

esp_err_t event_handler(void *ctx, system_event_t *event);



#endif /* MAIN_WIFI_H_ */
