#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "led.h"
#include "main.h"

void *led_thread_handler(void *arg)
{
	int led_flag;

	while (1)
	{
		pthread_mutex_lock(&SHM->led_mutex_start);

		while (0 == SHM->led_emit_start)
		{
			pthread_cond_wait(&SHM->led_cond_start, &SHM->led_mutex_start);
		}

		led_flag = SHM->led_emit_status;

		if (0 < led_flag)
		{
			SHM->led_emit_status--;
		}

		SHM->led_emit_start--;
		pthread_mutex_unlock(&SHM->led_mutex_start);

		if (0 == led_flag)
		{
			ioctl(*((int *)arg), LED_OFF, 0);
		}
		else if (0 < led_flag)
		{
			ioctl(*((int *)arg), LED_ON, 0);
		}

		pthread_mutex_lock(&SHM->led_mutex_end);
		led_flag = (0 == SHM->led_emit_end);
		SHM->led_emit_end++;
		pthread_mutex_unlock(&SHM->led_mutex_end);

		if (1 == led_flag)
		{
			pthread_cond_broadcast(&SHM->led_cond_end);
		}

		usleep(100000);
	}
}
