/*
 *  Gestion des capteurs à effet Hall pour la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  capteur_hall.cpp
 */
#include "main.h"

int isAzInitReach()
{
	return !gpio_get_level(PIN_AZ_CAPTOR);
}

int isElInitReach()
{
	return !gpio_get_level(PIN_EL_CAPTOR);

}

void initHallCaptor()
{
	ESP_ERROR_CHECK(gpio_set_direction(PIN_AZ_CAPTOR, GPIO_MODE_INPUT));
	ESP_ERROR_CHECK(gpio_set_direction(PIN_EL_CAPTOR, GPIO_MODE_INPUT));
}


bool setInitialPosAz(posStepper *posMoteurs)
{
	while(!isAzInitReach() && posMoteurs->az < STEPS_RED-1)
	{
		//printf("%d / %d\n", posMoteurs->az, isAzInitReach());
		oneStepAz(posMoteurs, FORWARD);
		vTaskDelay(5 / portTICK_PERIOD_MS);
	}

	return isAzInitReach();
}


bool setInitialPosEl(posStepper *posMoteurs)
{
	while(!isElInitReach() && posMoteurs->el < STEPS_RED-1)
	{
		//printf("%d / %d\n", posMoteurs->el, isElInitReach());
		oneStepEl(posMoteurs, FORWARD);
		vTaskDelay(5 / portTICK_PERIOD_MS);
	}

	return isElInitReach();
}


