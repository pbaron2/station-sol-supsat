/*
 *  Station Sol du Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  main.h
 */
#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_



#include <string>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "soc/soc.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "apps/sntp/sntp.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "errno.h"
#include "tcpip_adapter.h"
#include "driver/gpio.h"


#include "conf.h"
#include "wifi.h"
#include "rtc.h"
#include "tle.h"
#include "tle_web.h"
#include "motor.h"
#include "captive_portal.h"
#include "screen.h"
#include "capteur_hall.h"
#include "motor/Adafruit_MotorShield.h"
#include "sgp4/Sgp4.h"


#include "screen/ssd1306.h"

static const char *TAG = "StationSolSupsat";


// Pins de l'I2C du Shield Adafruit Motor Driver
#define SDA_PIN_STEPP GPIO_NUM_33
#define SCL_PIN_STEPP GPIO_NUM_25

// Pins de l'I2C et du reset de l'ecran SSD1306
#define SDA_PIN_ECRAN GPIO_NUM_16
#define SCL_PIN_ECRAN GPIO_NUM_4
#define PIN_RST_SCREEN GPIO_NUM_17


void i2c_master_init(gpio_num_t sda_pin, gpio_num_t scl_pin, i2c_port_t port);

#endif /* MAIN_MAIN_H_ */
