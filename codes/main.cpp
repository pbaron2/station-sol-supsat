/*
 *  Programme principal de suivi de la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  main.cpp
 */

#include "main.h"



// L'IDF a besoin du protoype de la fonction app_main() sous cette forme si cette fonction est compilée en C++
extern "C" {
   void app_main();
}



extern EventGroupHandle_t wifi_event_group;


// Fonction principale
void app_main()
{
	printf("\n\tDemarrage ESP32\n\n");
	printf("\n\tStation Sol Supsat\n\t\trealisee par Paul Baron, Alexandre Delfosse et Adel M'foukh\n\t\tpour l'association Supsat de Supelec Rennes\n\n");

    ESP_ERROR_CHECK( nvs_flash_init() );

    // -------------------------------------------------------------------------------------

    // Initialisation des I2C
    i2c_master_init(SDA_PIN_ECRAN, SCL_PIN_ECRAN, I2C_NUM_1);
    i2c_master_init(SDA_PIN_STEPP, SCL_PIN_STEPP, I2C_NUM_0);
    printf("\n\tI2C initialise\n\n");

    // -------------------------------------------------------------------------------------

    // Initialisation de l'ecran
    ssd1306_init(PIN_RST_SCREEN);
    printf("\n\tEcran initialise\n\n");

    // -------------------------------------------------------------------------------------

    // Affichage des informations initiales sur l'ecran
#ifdef USE_PORTAL
    dataScreen data = {getTime(), WIFI_SSID_PORTAL, UNKNOWN, UNKNOWN, SAT_NAME, UNKNOWN, 0, 0, UNKNOWN, {UNKNOWN, UNKNOWN}}; // pour supelec-invite
#else
    dataScreen data = {getTime(), WIFI_SSID, UNKNOWN, UNKNOWN, SAT_NAME, UNKNOWN, 0, 0, UNKNOWN, {UNKNOWN, UNKNOWN}}; // pour reseau personnel
#endif

    affichage(data, "  Connex  Auth  ");

    // -------------------------------------------------------------------------------------

    // Initialisation de la connexion au WiFi

#ifdef USE_PORTAL
    initialise_wifi(WIFI_SSID_PORTAL, WIFI_PASS_PORTAL); // pour supelec-invite
#else
    initialise_wifi(WIFI_SSID, WIFI_PASS); // pour reseau personnel
#endif

    EventBits_t uxBits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, 120000 / portTICK_PERIOD_MS);
    if( ( uxBits & CONNECTED_BIT ) == 0 )
	{
    	printf("\n\t/!\\ WiFi non initialisee /!\\\n\n");
    	data.etatWifi = ERROR;
    	affichage(data, "!  WiFi Error  !");
    	return;
	}
    printf("\n\tWiFi initialisee\n\n");
    data.etatWifi = WORKS;
    affichage(data, " Synchroni. RTC ");

#ifdef USE_PORTAL
    // Identification aupres du portail captif de supelec-invite
    if(!connectCaptivePortal())
	{
		printf("\n\t/!\\ Echec de connexion au portail captif /!\\\n\n");
		data.etatWifi = ERROR;
		affichage(data, "! Port Capt Er !");
		return;
	}
#endif

    // -------------------------------------------------------------------------------------

    printf("\n\tSynchronisation du RTC par NTP\n\n");
	if(!ntp_task())
	{
		printf("\n\t/!\\ Synchronisation echouee : /!\\\n\n");
		affiche_time(getTime(), "\tHeure en memoire: %s\n\n");
		data.etatRTC = ERROR;
		affichage(data, "!  RTC  Echec  !");
		return;
	}
	printf("\n\tSynchronisation reussie :\n\n");
	affiche_time(getTime(), "\tThe current date/time in France is: %s\n\n");
	data.time = getTime();
	data.etatRTC = WORKS;
	affichage(data, "  Collecte TLE  ");

    // -------------------------------------------------------------------------------------

	printf("\n\tRecuperation des TLE\n\n");
	if(!get_tle_http())
	{
		printf("\n\t/!\\ Echec de recuperation /!\\\n\n");
		data.etatTLE = ERROR;
		affichage(data, "!  TLE  Echec  !");
		return;
	}
	printf("\n\tRecuperation reussie\n\n");
	data.etatTLE = WORKS;
	data.time = getTime();
	affichage(data, " Pos. Init. Az. ");

	// -------------------------------------------------------------------------------------

	printf("\n\tDemarrage du controle des moteurs\n\n");
	posStepper posMoteurs =  initialise_motors();

	// -------------------------------------------------------------------------------------

	printf("\n\tMise en position initiale des axes\n\n");
    initHallCaptor();

    if(!setInitialPosAz(&posMoteurs))
	{
		printf("\n\t/!\\ Echec de l'initialisation de l'axe d'azimuth /!\\\n\n");
		data.etatAxes[AZ] = ERROR;
		data.time = getTime();
		affichage(data, "!Pos Init Az Er!");
		stopMotors();
		return;
	}
	printf("\n\tInitialisation de l'axe d'azimuth reussie\n\n");
	data.etatAxes[AZ] = WORKS;
	posMoteurs.az = 0;
	data.time = getTime();
	affichage(data, " Pos. Init. El. ");

    if(!setInitialPosEl(&posMoteurs))
	{
		printf("\n\t/!\\ Echec de l'initialisation de l'axe d'elevation /!\\\n\n");
		data.etatAxes[EL] = ERROR;
		data.time = getTime();
		affichage(data, "!Pos Init El Er!");
		stopMotors();
		return;
	}
	printf("\n\tInitialisation de l'axe d'elevation reussie\n\n");
	data.etatAxes[EL] = WORKS;
	posMoteurs.el = 0;
	data.time = getTime();
	affichage(data, "Axes Initialises");

	stopMotors();

	// -------------------------------------------------------------------------------------

	printf("\n\tCalcul de la position du satellite : %s\n\n", SAT_NAME);
    setupTLE();

    affiche_time(getTLEInfo(), "\tDate d'actualisation des TLE : %s\n\n");

    Position satPos = {};
    getPosition(getTimePosix(), &satPos);

	data.visible= ERROR;
	data.time = getTime();
	affichage(data);

    while(satPos.elevation < 0) // Tant que le satellite n'est pas visible
    {
    	printf("\tSatellite non visible\n\n");
    	affiche_time(getTime(), "\tHeure: %s\n");
    	printInfoSat(satPos);

    	data.az = satPos.azimuth;
    	data.el = satPos.elevation;
    	data.time = getTime();
    	affichage(data, " Attente Visib. ");

    	commandeAbs(satPos.azimuth, 0, &posMoteurs);
    	stopMotors();

    	vTaskDelay(5000 / portTICK_PERIOD_MS); // Attend 5 secondes

    	getPosition(getTimePosix(), &satPos);
    }

    // -------------------------------------------------------------------------------------

    printf("\n\tSuivi du satellite : %s\n\n", SAT_NAME);

    data.visible= WORKS;
    data.time = getTime();
    affichage(data);

    while(satPos.elevation >= 0)
    {
    	getPosition(getTimePosix(), &satPos);
    	affiche_time(getTime(), "\tHeure: %s\n");
    	printInfoSat(satPos);

    	data.az = satPos.azimuth;
    	data.el = satPos.elevation;
    	data.time = getTime();
    	affichage(data, "    Suivi...    ");

    	commandeAbs(satPos.azimuth, satPos.elevation, &posMoteurs);

    	vTaskDelay(100 / portTICK_PERIOD_MS); // Attend 100ms
    }

    data.visible = ERROR;
    stopMotors();
    data.time = getTime();
    affichage(data, " Suivi  Termine ");

    printf("\n\tFin de suivi\n\n");




    /*const int deep_sleep_sec = 10;
    ESP_LOGI(TAG, "Entering deep sleep for %d seconds", deep_sleep_sec);
    esp_deep_sleep(1000000LL * deep_sleep_sec);*/

}

// Initialisation d'une liaison I2C
void i2c_master_init(gpio_num_t sda_pin, gpio_num_t scl_pin, i2c_port_t port)
{
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = sda_pin;
	conf.scl_io_num = scl_pin;
	conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
	conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
	conf.master.clk_speed = 100000;
	i2c_param_config(port, &conf);
	i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);
}

