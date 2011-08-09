#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "status.h"
#include "main.h"

void *status_thread_handler(void *arg)
{
	int led;
	int beep;
	int temperature;
	char buf[4];

	while (1)
	{
		pthread_mutex_lock(&SHM->led_mutex_start);
		led = SHM->led_emit_status;
		pthread_mutex_unlock(&SHM->led_mutex_start);

		pthread_mutex_lock(&SHM->beep_mutex_status);
		beep = SHM->beep_emit_status;
		pthread_mutex_unlock(&SHM->beep_mutex_status);

		pthread_mutex_lock(&SHM->status_mutex_temperature);
		temperature = SHM->status_temperature;
		pthread_mutex_unlock(&SHM->status_mutex_temperature);

		lseek(*((int *)arg), 60, SEEK_SET);

		if (1 == led)
		{
			write(*((int *)arg), "ON ", 3);
		}
		else
		{
			write(*((int *)arg), "OFF", 3);
		}

		lseek(*((int *)arg), 82, SEEK_SET);

		if (1 == beep)
		{
			write(*((int *)arg), "ON ", 3);
		}
		else
		{
			write(*((int *)arg), "OFF", 3);
		}

		lseek(*((int *)arg), 106, SEEK_SET);

		if (100 <= temperature)
		{
			buf[0] = temperature / 100 + 0x30;
			buf[1] = (temperature % 100) / 10 + 0x30;
			buf[2] = temperature % 10 + 0x30;
			buf[3] = '\0';
			write(*((int *)arg), buf, 3);
		}
		else if (10 <= temperature && 100 > temperature)
		{
			buf[0] = temperature / 10 + 0x30;
			buf[1] = temperature % 10 + 0x30;
			buf[2] = 0x20;
			buf[3] = '\0';
			write(*((int *)arg), buf, 3);
		}
		else if (0 <= temperature && 9 >= temperature)
		{
			buf[0] = temperature + 0x30;
			buf[1] = 0x20;
			buf[2] = 0x20;
			buf[3] = '\0';
			write(*((int *)arg), buf, 3);
		}

		usleep(500000);
	}
}
