/*
 *  Fichier de configuration de la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  conf.h
 */

#ifndef CONF_H_
#define CONF_H_



//-----------------------------------------------------------------
// Configuration du WiFi

//#define USE_PORTAL 						// Commentez pour ne pas utiliser supelec-invite (par defaut : non commente)

#define WIFI_USERNAME "sessio8"		// LOGIN d'authentification via supelec-invite (par defaut : "sessio8")
#define WIFI_PASSWORD "t4a3s2.1"	// PASSWORD d'authentification via supelec-invite (par defaut : "t4a3s2.1")

#ifndef USE_PORTAL
	#define WIFI_SSID "JAMBON"			// SSID du reseau personnel si pas de connexion via supelec-invite
	#define WIFI_PASS "superpaulo"		// PASSWORD du reseau personnel
#endif

//-----------------------------------------------------------------
// Configuration du NTP

#define SERVEUR_NTP "pool.ntp.org"		// Adresse du serveur NTP pour synchronisation du RTC (par defaut : "pool.ntp.org")

//-----------------------------------------------------------------
// Configuration des TLE

// Configuration du site de collecte des TLE
#define WEB_SERVER "celestrak.com"				// Nom de domaine (par defaut : "celestrak.com"	)
#define WEB_PORT 80								// Port (par defaut : 80 (HTTP))
#define WEB_URL "/NORAD/elements/cubesat.txt"	// URL de la page txt listant les TLE (par defaut : "/NORAD/elements/cubesat.txt")

#define SAT_NAME "TIGRISAT"				// Nom du satellite suivi (< 24 caracteres)

// Configuration de l'emplacement de la station sol
#define SITE_LATITUDE	48.1254			// Latitude (par defaut : 48.125)
#define SITE_LONGITUDE	-1.6234			// Longitude (par defaut : -1.623)
#define SITE_ALTITUDE	65				// Altitude par rapport au niveau de la mer (par defaut : 65)

//-----------------------------------------------------------------


#endif /* CONF_H_ */
