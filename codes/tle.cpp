/*
 *  Gestion et conversion des TLE pour la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  tle.cpp
 */
#include "main.h"

extern char tle[3][TLE_LENGTH + 1];

Sgp4 sat;

void setupTLE()
{

	//set site latitude[°], longitude[°] and altitude[m]
	sat.site(SITE_LATITUDE, SITE_LONGITUDE, SITE_ALTITUDE);

	//initialize satellite parameters
	sat.init(tle[0], tle[1], tle[2]);
}

void getPosition(unsigned long time, Position *toActualise)
{
	sat.findsat(time);

	toActualise->azimuth = sat.satAz;
	toActualise->elevation = sat.satEl;
	toActualise->distance = sat.satDist;


	toActualise->satLat = sat.satLat;
	toActualise->satLon = sat.satLon;
	toActualise->satAlt = sat.satAlt;

	//satVis == -2 => under horizon / satVis == -1 => daylight / satVis == [0, 1000] => [eclipsed, visible]
	toActualise->visible = (sat.satVis >= 0);

}

void printInfoSat(Position data)
{
	printf("\t%s : Az:%f' / El:%f' / Distance:%fkm\n\tLat:%f' / Long:%f' / Altitude:%fkm\n\n",
			SAT_NAME, data.azimuth, data.elevation, data.distance, data.satLat, data.satLon, data.satAlt);
}


struct tm getTLEInfo()
{
	struct tm tle_info = {};
	struct tm time_info;
	double sec = 0;

	time_t timeAct;
	time(&timeAct);
	localtime_r(&timeAct, &time_info);

	invjday(sat.satJd , 1,time_info.tm_isdst, tle_info.tm_year, tle_info.tm_mon, tle_info.tm_mday, tle_info.tm_hour, tle_info.tm_min, sec);
	tle_info.tm_sec = int(sec);
	tle_info.tm_year += 4830;

	return tle_info;
}
