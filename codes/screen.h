/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  screen.h
 */
#ifndef SCREEN_H_
#define SCREEN_H_

enum etatScreen
{
	UNKNOWN,
	WORKS,
	ERROR
};
typedef enum etatScreen etatScreen;

#define AZ 0
#define EL 1

typedef struct dataScreen dataScreen;
struct dataScreen
{
	struct tm time;
	char wifiName[41];
	etatScreen etatTLE;
	etatScreen etatRTC;
	char satName[SAT_NAME_LENGTH + 1];
	etatScreen visible;
	double az;
	double el;
	etatScreen etatWifi;
	etatScreen etatAxes[2];
};


void affichage(dataScreen data);
void affichage(dataScreen data, const char *lastLine);
void affichage(struct tm time, char *wifiName, etatScreen etatTLE, etatScreen etatRTC, char* satName, etatScreen visible, double az, double el, etatScreen etatWifi, etatScreen etatAxes[], const char *lastLine);


#endif /* SCREEN_H_ */
