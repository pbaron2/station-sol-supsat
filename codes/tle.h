/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  tle.h
 */
#ifndef MAIN_TLE_H_
#define MAIN_TLE_H_

#include "sgp4/Sgp4.h"


typedef struct Position Position;
struct Position
{
	double azimuth;
	double elevation;
	double distance;
	double satLat;
	double satLon;
	double satAlt;
	bool visible;
};



void setupTLE();
void getPosition(unsigned long time, Position *toActualise);
struct tm getTLEInfo();
void printInfoSat(Position data);

#endif /* MAIN_TLE_H_ */
