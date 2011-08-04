#ifndef __MAIN_H__
#define __MAIN_H__

struct shm
{
	pthread_mutex_t alarm_mutex_start;
	pthread_mutex_t gprs_mutex_end;
	pthread_mutex_t beep_mutex_start;
	pthread_mutex_t beep_mutex_status;
	pthread_mutex_t led_mutex_start;
	pthread_mutex_t adc_mutex_start;
	pthread_cond_t alarm_cond_start;
	pthread_cond_t gprs_cond_end;
	pthread_cond_t beep_cond_start;
	pthread_cond_t led_cond_start;
	pthread_cond_t adc_cond_start;

	int alarm_emit_start;
	int gprs_emit_end;
	int led_emit_status;
	int led_emit_start;
	int beep_emit_start;
	int beep_emit_status;
	int adc_emit_status;
}*SHM;

#endif
