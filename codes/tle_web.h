/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  tle_web.h
 */
#ifndef MAIN_TLE_WEB_H_
#define MAIN_TLE_WEB_H_



#include <string>

using namespace std;

#define TLE_LENGTH 69
#define SAT_NAME_LENGTH 24


bool get_tle_http();



int readLine(int s, char line[]);
int searchSat(int s, string name);


#endif /* MAIN_TLE_WEB_H_ */
