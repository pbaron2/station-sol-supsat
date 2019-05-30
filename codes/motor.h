/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  motor.h
 */
#ifndef MAIN_MOTOR_H_
#define MAIN_MOTOR_H_

#include "motor/Adafruit_MotorShield.h"

class posStepper
{
  public:
  int az;
  int el;
};

#define STEPS 200  // Pas par tour
#define MAXRPM 50  // Vitesse de rotation maximale
const float RAPPORT_RED = 1.0/54;  // Rapport de reduction R=Vm/Vantenne 1 deg d'antenne vaut a peu pres 29 pas
const int STEPS_RED = STEPS/RAPPORT_RED; // Nombre de pas pour faire un tour en sortie du reducteur



posStepper initialise_motors();
posStepper calculPosMoteurs(float angleAz, float angleEl);
posStepper calculDeltaMoteurs(posStepper posVoulu, posStepper posActuel);
void commandeAz(int stepAz, posStepper *posActuel);
void commandeEl(int stepEl, posStepper *posActuel);
void oneStepAz(posStepper *posActuel, uint8_t sens);
void oneStepEl(posStepper *posActuel, uint8_t sens);
void commandeAbs(float angleAz, float angleEl, posStepper *posActuel);

void stopMotors();

#endif /* MAIN_MOTOR_H_ */
