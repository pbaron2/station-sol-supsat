/*
 *  Station Sol Supsat
 *  	Projet long / Supelec / 2018
 *  		par Paul Baron
 *
 *  rtc.h
 */
#ifndef MAIN_RTC_H_
#define MAIN_RTC_H_



int wait_for_setting();
void initialize_sntp();
int initialize_time();

void affiche_time(struct tm timeinfo, const char *txt);
struct tm getTime();
time_t getTimePosix();

bool ntp_task();



#endif /* MAIN_RTC_H_ */
