/*
 *  Gestion des moteurs de la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  motor.cpp
 */
#include "main.h"

Adafruit_MotorShield AFMS;

Adafruit_StepperMotor *stepperEl;
Adafruit_StepperMotor *stepperAz;



// Initialise les deux moteurs pas-a-pas
posStepper initialise_motors()
{
	// Create the motor shield object with the default I2C address
	AFMS = Adafruit_MotorShield(0x60, I2C_NUM_0);

	AFMS.begin(1600);  // create with the default frequency 1.6kHz

	stepperEl = AFMS.getStepper(STEPS, 1);
	stepperAz = AFMS.getStepper(STEPS, 2);

	stepperEl->setSpeed(MAXRPM);  // vitesse de rotation
	stepperAz->setSpeed(MAXRPM);

	posStepper pos = {0, 0};

	return pos;
}

// Convertie angle en nombre de pas
posStepper calculPosMoteurs(float angleAz, float angleEl)
{
  posStepper posVoulu;
  posVoulu.az = ((int) (angleAz*STEPS_RED/360) - 90) % STEPS_RED;
  posVoulu.el = ((int) (angleEl*STEPS_RED/360)) % STEPS_RED;
  return posVoulu;
}



// Donne le nombre de pas et le sens de rotation entre deux positions
posStepper calculDeltaMoteurs(posStepper posVoulu, posStepper posActuel)
{
  posStepper posDelta;

  posDelta.az = (posVoulu.az - posActuel.az);
  if(abs(posDelta.az) > STEPS_RED/2)
  {
    int signe = ((posDelta.az < 0) * 2 - 1);
    posDelta.az = posDelta.az + signe * STEPS_RED;
  }

  posDelta.el = (posVoulu.el - posActuel.el);
  if(abs(posDelta.el) > STEPS_RED/2)
  {
    int signe = ((posDelta.el < 0) * 2 - 1);
    posDelta.el = posDelta.el + signe * STEPS_RED;
  }

  return posDelta;
}



// Commande le moteur d'azimuth
void commandeAz(int stepAz, posStepper *posActuel)
{
  uint8_t sens;

  if(stepAz > 0)
    sens = FORWARD;
  else
    sens = BACKWARD;

  stepperAz->step(abs(stepAz), sens, DOUBLE);
  posActuel->az = (posActuel->az + stepAz) % STEPS_RED;
}



// Commande le moteur d'azimuth d'un pas
void oneStepAz(posStepper *posActuel, uint8_t sens)
{
	stepperAz->onestep(sens, DOUBLE);
	posActuel->az = (posActuel->az + 1) % STEPS_RED;
}



// Commande le moteur d'elevation d'un pas
void oneStepEl(posStepper *posActuel, uint8_t sens)
{
	stepperEl->onestep(sens, DOUBLE);
	posActuel->el = (posActuel->el + 1) % STEPS_RED;
}



// Commande le moteur d'elevation
void commandeEl(int stepEl, posStepper *posActuel)
{
  uint8_t sens;

  if(stepEl > 0)
    sens = FORWARD;
  else
    sens = BACKWARD;

  stepperEl->step(abs(stepEl), sens, DOUBLE);
  posActuel->el = (posActuel->el + stepEl) % STEPS_RED;
}



// Commande des moteurs en angles absolus
void commandeAbs(float angleAz, float angleEl, posStepper *posActuel)
{
  posStepper pos = calculDeltaMoteurs(calculPosMoteurs(angleAz, angleEl), *posActuel);
  commandeAz(pos.az, posActuel);
  commandeEl(pos.el, posActuel);
}



// Arrete les moteurs (pas de couple de mantien)
void stopMotors()
{
	stepperAz->release();
	stepperEl->release();
}
