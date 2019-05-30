/*
 *  Gestion de l'ecran de la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  screen.cpp
 */
#include "main.h"



// Actualise l'affichage sans derniere ligne
void affichage(dataScreen data)
{
	affichage(data, "                ");
}



// Actualise l'affichage avec derniere ligne
void affichage(dataScreen data, const char *lastLine)
{
	affichage(data.time, data.wifiName, data.etatTLE, data.etatRTC, data.satName, data.visible, data.az, data.el, data.etatWifi, data.etatAxes, lastLine);
}



// Actualise l'affichage sans la struct data
void affichage(struct tm time, char *wifiName, etatScreen etatTLE, etatScreen etatRTC, char* satName, etatScreen visible, double az, double el, etatScreen etatWifi, etatScreen etatAxes[], const char *lastLine)
{
	char txt[136] = "StationSolSupsat\n ";
	char temp[9] = "";

	// Efface l'ecran
	task_ssd1306_display_clear(NULL);


	sprintf(temp, "%02d/%02d/%02d %02d:%02d \n", time.tm_mday, time.tm_mon+1, abs(time.tm_year-100), time.tm_hour, time.tm_min);
	strcat(txt, temp);

	strcat(txt, "WiFi:");
	if(etatWifi == UNKNOWN)
		strcat(txt, "?|");
	else if(etatWifi == WORKS)
		strcat(txt, "*|");
	else
		strcat(txt, "x|");

	if(strlen(wifiName) > 8) // Si le nom du reseau est trop long pour l'affichage
	{
		wifiName[9] = '\0'; // On le tronque
		strcat(txt, wifiName);
	}
	else
	{
		strcat(txt, wifiName);
		for(int i = strlen(txt); i < 50; i++) // Sinon, on complète avec des espaces
		{
			txt[i] = ' ';
		}
		txt[51] = '\0';
	}

	strcat(txt, "\nRTC: ");
	if(etatRTC == UNKNOWN)
		strcat(txt, "?    TLE: ");
	else if(etatRTC == WORKS)
		strcat(txt, "*    TLE: ");
	else
		strcat(txt, "x    TLE: ");

	if(etatTLE == UNKNOWN)
		strcat(txt, "?\n");
	else if(etatTLE == WORKS)
		strcat(txt, "*\n");
	else
		strcat(txt, "x\n");

	strcat(txt, "Sat: ");

	if(strlen(satName) > 10)  // Si le nom du satellite est trop long pour l'affichage
	{
		satName[11] = '\0'; // On le tronque
		strcat(txt, satName);
	}
	else
	{
		strcat(txt, satName);
		for(int i = strlen(txt); i < 84; i++) // Sinon, on complète avec des espaces
		{
			txt[i] = ' ';
		}
		txt[85] = '\0';
	}

	strcat(txt, "\nVisible: ");
	if(visible == UNKNOWN)
		strcat(txt, "?      \n");
	else if(visible == WORKS)
		strcat(txt, "*      \n");
	else
		strcat(txt, "x      \n");


	if(etatAxes[AZ] == UNKNOWN)
		strcat(txt, "Az: ?    El:");
	else if(etatAxes[AZ] == ERROR)
		strcat(txt, "Az: x    El:");
	else
	{
		sprintf(temp, "Az:%3d'  El:", int(az));
		strcat(txt, temp);
	}

	if(etatAxes[EL] == UNKNOWN)
		strcat(txt, " ?  \n");
	else if(etatAxes[EL] == ERROR)
		strcat(txt, " x  \n");
	else
	{
		if(el >= 0)
		{
			sprintf(temp, "%2d '\n", int(el));
		}
		else
		{
			sprintf(temp, "-%2d'\n", abs(int(el)));
		}

		strcat(txt, temp);
	}


	strcat(txt, lastLine);


	// Envoie l'instruction a l'ecran
	task_ssd1306_display_text((void *)txt);
}


