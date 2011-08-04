#ifndef __MAIN_H__
#define __MAIN_H__

#define MAXCMDLEN 256

struct shm
{
	pthread_mutex_t gprs_mutex_start;
	pthread_cond_t gprs_cond_start;

	pthread_mutex_t gprs_mutex_end;
	pthread_cond_t gprs_cond_end;

	pthread_mutex_t beep_mutex_start;
	pthread_cond_t beep_cond_start;
	
	pthread_mutex_t beep_mutex_end;
	pthread_cond_t beep_cond_end;

	pthread_mutex_t beep_mutex_status;

	pthread_mutex_t led_mutex_start;
	pthread_cond_t led_cond_start;
	
	pthread_mutex_t led_mutex_end;
	pthread_cond_t led_cond_end;

	pthread_mutex_t cgi_mutex_start;
	pthread_cond_t cgi_cond_start;

	int gprs_emit_start;
	int gprs_emit_end;

	int led_emit_start;
	int led_emit_end;
	int led_emit_status;
	
	int beep_emit_start;
	int beep_emit_end;
	int beep_emit_status;

	int cgi_emit_start;
	char cgi_cmd[MAXCMDLEN];
}*SHM;

#endif
