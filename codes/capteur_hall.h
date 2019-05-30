/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  capteur_hall.h
 */
#ifndef CAPTEUR_HALL_H_
#define CAPTEUR_HALL_H_

#include "driver/gpio.h"

#define PIN_AZ_CAPTOR GPIO_NUM_35
#define PIN_EL_CAPTOR GPIO_NUM_34

int isAzInitReach();
int isElInitReach();
void initHallCaptor();
bool setInitialPosAz(posStepper *posMoteurs);
bool setInitialPosEl(posStepper *posMoteurs);

#endif /* CAPTEUR_HALL_H_ */
