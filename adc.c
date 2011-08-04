#include <pthread.h>
#include <unistd.h>

#include "adc.h"
#include "main.h"

void *adc_thread_handler(void *arg)
{
	int adc_value;

	while (1)
	{
		read(*((int *)arg), &adc_value, sizeof(adc_value));
		//printf("adc_value=%d\n", adc_value);

		if (420 < adc_value)
		{
			pthread_mutex_lock(&SHM->alarm_mutex_start);
			gprs_flag_start = (0 == SHM->alarm_emit_start);
			SHM->alarm_emit_start++;
			pthread_mutex_unlock(&SHM->alarm_mutex_start);

			if (1 == gprs_flag_start)
			{
				pthread_cond_signal(&SHM->alarm_cond_start);
			}

			pthread_mutex_lock(&SHM->gprs_mutex_end);

			while (0 == SHM->gprs_emit_end)
			{
				pthread_cond_wait(&SHM->gprs_cond_end, &SHM->gprs_mutex_end);
			}

			SHM->gprs_emit_end--;
			pthread_mutex_unlock(&SHM->gprs_mutex_end);

			pthread_mutex_lock(&SHM->led_mutex_start);
			led_flag_start = (0 == SHM->led_emit_start);

			if (1 != keyno)
			{
				SHM->led_emit_status++;
			}

			SHM->led_emit_start++;
			pthread_mutex_unlock(&SHM->led_mutex_start);

			if (1 == led_flag_start)
			{
				pthread_cond_signal(&SHM->led_cond_start);
			}

			pthread_mutex_lock(&SHM->beep_mutex_status);

			if (1 != keyno)
			{
				SHM->beep_emit_status = 1;
			}
			else
			{
				SHM->beep_emit_status = 0;
			}

			pthread_mutex_unlock(&SHM->beep_mutex_status);

			pthread_mutex_lock(&SHM->beep_mutex_start);
			beep_flag_start = (0 == SHM->beep_emit_start);
			SHM->beep_emit_start++;
			pthread_mutex_unlock(&SHM->beep_mutex_start);

			if (1 == beep_flag_start)
			{
				pthread_cond_signal(&SHM->beep_cond_start);
			}

			keyno = 0;

			if (-1 == write(fd_keyboard, &keyno, sizeof(keyno)))
			{
				perror("Failed to write");
				exit(-1);
			}
		}

		usleep(100000);
	}
}
