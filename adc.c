#include <pthread.h>
#include <unistd.h>

#include "adc.h"
#include "main.h"

void *adc_thread_handler(void *arg)
{
	int adc_value;
	int gprs_flag;
	int led_flag;
	int beep_flag;

	while (1)
	{
		read(*((int *)arg), &adc_value, sizeof(adc_value));

		pthread_mutex_lock(&SHM->status_mutex_temperature);
		SHM->status_temperature = adc_value / 10;
		pthread_mutex_unlock(&SHM->status_mutex_temperature);

		if (420 < adc_value)
		{
			pthread_mutex_lock(&SHM->gprs_mutex_start);
			gprs_flag = (0 == SHM->gprs_emit_start);
			SHM->gprs_emit_start++;
			pthread_mutex_unlock(&SHM->gprs_mutex_start);

			if (1 == gprs_flag)
			{
				pthread_cond_signal(&SHM->gprs_cond_start);
			}

			pthread_mutex_lock(&SHM->gprs_mutex_end);

			while (0 == SHM->gprs_emit_end)
			{
				pthread_cond_wait(&SHM->gprs_cond_end, &SHM->gprs_mutex_end);
			}

			if (0 < SHM->gprs_emit_end)
			{
				SHM->gprs_emit_end--;
			}

			pthread_mutex_unlock(&SHM->gprs_mutex_end);

			pthread_mutex_lock(&SHM->led_mutex_status);

			if (0 == SHM->led_emit_status)
			{
				SHM->led_emit_status++;
			}

			pthread_mutex_unlock(&SHM->led_mutex_status);

			pthread_mutex_lock(&SHM->led_mutex_start);
			led_flag = (0 == SHM->led_emit_start);

			if (0 == SHM->led_emit_start)
			{
				SHM->led_emit_start++;
			}

			pthread_mutex_unlock(&SHM->led_mutex_start);

			if (1 == led_flag)
			{
				pthread_cond_signal(&SHM->led_cond_start);
			}

			pthread_mutex_lock(&SHM->led_mutex_end);

			while (0 == SHM->led_emit_end)
			{
				pthread_cond_wait(&SHM->led_cond_end, &SHM->led_mutex_end);
			}

			if (0 < SHM->led_emit_end)
			{
				SHM->led_emit_end--;
			}

			pthread_mutex_unlock(&SHM->led_mutex_end);

			pthread_mutex_lock(&SHM->beep_mutex_status);

			if (0 == SHM->beep_emit_status)
			{
				SHM->beep_emit_status++;
			}

			pthread_mutex_unlock(&SHM->beep_mutex_status);

			pthread_mutex_lock(&SHM->beep_mutex_start);
			beep_flag = (0 == SHM->beep_emit_start);

			if (0 == SHM->beep_emit_start)
			{
				SHM->beep_emit_start++;
			}

			pthread_mutex_unlock(&SHM->beep_mutex_start);

			if (1 == beep_flag)
			{
				pthread_cond_signal(&SHM->beep_cond_start);
			}

			pthread_mutex_lock(&SHM->beep_mutex_end);

			while (0 == SHM->beep_emit_end)
			{
				pthread_cond_wait(&SHM->beep_cond_end, &SHM->beep_mutex_end);
			}

			if (0 < SHM->beep_emit_end)
			{
				SHM->beep_emit_end--;
			}

			pthread_mutex_unlock(&SHM->beep_mutex_end);
		}

		usleep(100000);
	}
}
