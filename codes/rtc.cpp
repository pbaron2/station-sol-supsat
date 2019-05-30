/*
 *  Gestion du temps (RTC et NTP) pour la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  rtc.cpp
 */
#include "main.h"


extern EventGroupHandle_t wifi_event_group;


// Attend que le RTC interne soit actualise
int wait_for_setting()
{
    time_t now = 0;
    struct tm timeinfo = {};

    // Variables pour le nombre d'essais
    int retry = 1;
    const int retry_count = 10;


    while(timeinfo.tm_year < (2016 - 1900) && retry < retry_count) // Verifie que le RTC est actualise avant le nombre d'essais max
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(3000 / portTICK_PERIOD_MS); // Attend 3 secondes

        // Recupere l'heure du RTC en structure tm
        time(&now);
        localtime_r(&now, &timeinfo);

        retry++;
    }

    return timeinfo.tm_year < (2016 - 1900);
}



// Se connecte au serveur NTP, recupere l'heure et la stocke dans le RTC
void initialize_sntp()
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, (char*)SERVEUR_NTP);
    sntp_init();
}



// Verifie que le RTC interne est initialise, et a defaut, l'initialise
int initialize_time()
{
	time_t now;
	struct tm timeinfo;
	int notSuccess = 0;

	time(&now); // Recupere l'heure stockee dans le rtc
	localtime_r(&now, &timeinfo); // Convertie en structure tm

	printf("\n\tTRY\n");
	xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
	ESP_LOGI(TAG, "Connecting to WiFi and getting time over NTP.");
	initialize_sntp(); // Connexion au NTP et stockage de l'heure dans le RTC
	notSuccess = wait_for_setting(); // Recupere l'heure par NTP et initalise le RTC
	sntp_stop();
	return notSuccess;

}



// Affiche l'heure donnee en POSIX
void affiche_time(struct tm timeinfo, const char *txt)
{
	// Variable pour le stockage de l'heure sous forme de texte
	char strftime_buf[64];

	// Convertie la structure tm en texte
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

	// Affichage
	printf(txt, strftime_buf);
}



// Renvoie l'heure du RTC interne en struc tm
struct tm getTime()
{
	time_t now;
	struct tm timeinfo;
	time(&now); // Recupere l'heure stockee dans le rtc
	localtime_r(&now, &timeinfo); // Convertie en structure tm
	return timeinfo;
}



// Renvoie l'heure du RTC interne en POSIX
time_t getTimePosix()
{
	time_t now;
	time(&now); // Recupere l'heure stockee dans le rtc
	return now;
}



// Initialise le RTC
bool ntp_task()
{
	bool success = !initialize_time();

	// Fixe la timezone de la France
	setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
	tzset();

	return success;

	//vTaskDelete( NULL );
}

