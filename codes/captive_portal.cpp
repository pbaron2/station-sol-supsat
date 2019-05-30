/*
 *  Authentification via le portail captif de la Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  captive_portal.cpp
 */
#include "main.h"

extern EventGroupHandle_t wifi_event_group;

extern const char *REQUEST;


bool connectCaptivePortal()
{
	int s;

	// Variables pour le nombre d'essais
	bool connected = false;
	int retry = 1;
	const int retry_count = 10;

	while(!connected && retry <= retry_count)
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

		// Read HTTP response

		const char firstLineConnected[16] = "HTTP/1.1 200 OK";
		char line[256] = "";

		readLine(s, line);
		readLine(s, line);

		if(strcmp(line, "Server: nginx/1.12.2") == 0) // Si la connexion a reussie (portail deja connecte)
		{
			connected = true;
			continue;
		}
		printf("\nNOT CONNECTED YET\n\n");


		close(s);
		// ----------------------------------------------------------------------------
		/*s = createSocket();

		char REQUEST_GET[512] = "GET /reg.php?ah_goal=authentication.html&ah_login=true\r\n"
								"Host: 1.1.103.1\r\n"
								"Connection: keep-alive\r\n"
								"User-Agent: esp-idf/1.0 esp32\r\n"
								"\r\n";


		printf("\nREQUEST_GET : %s\n", REQUEST_GET);

		if (write(s, REQUEST_GET, strlen(REQUEST_GET)) < 0)
		{
			ESP_LOGE(TAG, "... socket send failed");
			close(s);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			continue;
		}

		//while(readLine(s, line))
		//{
		//	printf("LECTURE : %s\n", line);
		//}


		close(s);*/
		// ---------------------------------------------------------------------


		s = createSocket();

		char REQUEST_POST[1024] = "POST /reg.php HTTP/1.1\r\n"
								  "Host: 1.1.103.1\r\n"
								  "Connection: keep-alive\r\n"
								  "User-Agent: esp-idf/1.0 esp32\r\n"
								  "Content-Type: application/x-www-form-urlencoded\r\n"
								  "Content-Length: ";

		char contentLength[4] = "";
		sprintf(contentLength, "%d", (19 + strlen(WIFI_USERNAME) + strlen(WIFI_PASSWORD)));
		strcat(REQUEST_POST, contentLength);

		strcat(REQUEST_POST, "\r\n\r\nusername=" WIFI_USERNAME "&password=" WIFI_PASSWORD "\r\n");

		printf("\nREQUEST_POST : %s\n", REQUEST_POST);

		if (write(s, REQUEST_POST, strlen(REQUEST_POST)) < 0)
		{
			ESP_LOGE(TAG, "... socket send failed");
			close(s);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			continue;
		}

		/*while(readLine(s, line))
		{
			printf("LECTURE : %s\n", line);
		}*/

		if(strcmp(line, firstLineConnected) == 0) // Si la connexion a reussie
		{
			printf("\nCONNECTE !\n");
			connected = true;
			continue;
		}

		/*for(int k = 0; k < 10; k++)
		{
			readLine(s, line);
			printf("\nLECTURE : %s\n", line);
		}*/

		// -----------------------------------------------------------
		close(s);

		/*s = createSocket();

		char REQUEST_POST2[1024] = "POST /reg.php HTTP/1.1\r\n"
			"Host: 1.1.3.1\r\n"
			"Connection: keep-alive\r\n"
			"User-Agent: esp-idf/1.0 esp32\r\n"
			"\r\nurl=";
		strcat(REQUEST_POST2, id);
		strcat(REQUEST_POST2, "&username=" WIFI_USERNAME "&password=" WIFI_PASSWORD "&checkbox=checkbox\r\n");

		printf("\nREQUEST_POST2 : %s\n", REQUEST_POST2);

		if (write(s, REQUEST_POST2, strlen(REQUEST_POST2)) < 0)
		{
			ESP_LOGE(TAG, "... socket send failed");
			close(s);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			continue;
		}

		readLine(s, line);
		printf("\nLECTURE : %s\n", line);

		if(strcmp(line, firstLineConnected) == 0) // Si la connexion a reussie
		{
			printf("\nCONNECTE !\n");
			connected = true;
		}

		for(int k = 0; k < 25; k++)
		{
			readLine(s, line);
			printf("\nLECTURE : %s\n", line);
		}

		vTaskDelay(10000 / portTICK_PERIOD_MS);

		close(s);*/
	}

	printf("\nend\n");
	return connected;

}

int createSocket()
{
	struct addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *res;
	struct in_addr *addr;
	int s;
	// Wait for the callback to set the CONNECTED_BIT in the event group
	xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);

	ESP_LOGI(TAG, "Connected to AP");

	int err = getaddrinfo(WEB_SERVER, "80", &hints, &res);

	if(err != 0 || res == NULL)
	{
		ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		return -1;
	}

	// Code to print the resolved IP.
	// Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code
	addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
	ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

	s = socket(res->ai_family, res->ai_socktype, 0);
	if(s < 0)
	{
		ESP_LOGE(TAG, "... Failed to allocate socket.");
		freeaddrinfo(res);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		return -1;
	}
	ESP_LOGI(TAG, "... allocated socket\r\n");

	if(connect(s, res->ai_addr, res->ai_addrlen) != 0)
	{
		ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
		close(s);
		freeaddrinfo(res);
		vTaskDelay(4000 / portTICK_PERIOD_MS);
		return -1;
	}

	ESP_LOGI(TAG, "... connected");
	freeaddrinfo(res);

	return s;
}
