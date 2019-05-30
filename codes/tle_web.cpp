/*
 *  Collecte sur le web des TLE pour la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  tle_web.cpp
 */
#include "main.h"

char tle[3][TLE_LENGTH + 1];

extern EventGroupHandle_t wifi_event_group;

const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
	    "Host: " WEB_SERVER "\r\n"
	    "User-Agent: esp-idf/1.0 esp32\r\n"
		"Connection: keep-alive\r\n"
	    "\r\n";



bool get_tle_http()
{
    int s;

    // Variables pour le nombre d'essais
    bool tleAct = false;
    int retry = 1;
    const int retry_count = 10;

    while(!tleAct && retry < retry_count)
    {
    	printf("\n\tTRY : %d\n", retry);
    	retry++;

    	s = createSocket();

        if (write(s, REQUEST, strlen(REQUEST)) < 0)
        {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");


        tleAct = searchSat(s, SAT_NAME);

        if(tleAct)
        {
        	printf("\nTLE :\n\t%s\n\t%s\n\t%s\n\n", tle[0], tle[1], tle[2]);
        }

        close(s);
    }

    printf("\nend\n");

    return !(retry == retry_count);

}


int searchSat(int s, string name)
{
	char line[TLE_LENGTH + 1];
	int etat = 1; // vaut 0 si eof ou -1 si erreur

	int len = name.length();
	char substr[SAT_NAME_LENGTH + 1];

	while(name.compare(substr) != 0 && etat != 0)
	{
		bzero(line, sizeof(line));
		etat = readLine(s, line);
		//printf("\nLECTURE : %d, %s (%d)\n", etat, line, name.compare(substr));
		strncpy(substr, line, len);
	}

	if (etat == 1)
	{
		printf("\n\tTLE trouvees\n\n");
		strcpy(tle[0], line);

		readLine(s, tle[1]);
		readLine(s, tle[2]);

		return true;
	}
	else
	{
		printf("\n\tTLE INTROUVABLES\n\n");
		return false;
	}
}

int readLine(int s, char line[])
{
    char c;
    int r = 0;
    int etat = 1;// vaut 0 si eof ou -1 si erreur ou 2 si fin de ligne
    int i = 0;
    do
    {
    	r = read(s, &c, sizeof(c));
    	if(r <= 0)
    	{
    		etat = r;
    	}
    	else
    	{
			if( c != '\r')
			{
				line[i] = c;
				i++;
			}
			else
			{
				read(s, &c, sizeof(c)); // lit le \n
				line[i] = '\0';
				etat = 2;
			}

    	}
    } while(etat == 1);

    /*
    for(int j = 0; j < TLE_LENGTH + 1; j++)
	{
		printf("\tCHAR: %d, %c (%d)\n", j, line[j], (int) line[j]);
	}
	*/

    return etat == 2; // return false si eof ou erreur
}


