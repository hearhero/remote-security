#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "cgi.h"
#include "main.h"

void *cgi_thread_handler(void *arg)
{
	char buf[MAXCMDLEN];
	int led_flag = 0;
	int beep_flag = 0;

	while (1)
	{
		pthread_mutex_lock(&SHM->cgi_mutex_start);

		while (0 == SHM->cgi_emit_start)
		{
			pthread_cond_wait(&SHM->cgi_cond_start, &SHM->cgi_mutex_start);
		}

		SHM->cgi_emit_start--;
		memset(buf, 0, MAXCMDLEN);
		strcpy(buf, SHM->cgi_cmd);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (0 == strcmp(buf, "LED_ON"))
		{
			pthread_mutex_lock(&SHM->led_mutex_start);
			led_flag = (0 == SHM->led_emit_start);

			if (0 == SHM->led_emit_status)
			{
				SHM->led_emit_status++;
			}

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
		}

		if (0 == strcmp(buf, "LED_OFF"))
		{
			pthread_mutex_lock(&SHM->led_mutex_start);
			led_flag = (0 == SHM->led_emit_start);
			SHM->led_emit_status = 0;

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
		}

		if (0 == strcmp(buf, "BEEP_ON"))
		{
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

		if (0 == strcmp(buf, "BEEP_OFF"))
		{
			pthread_mutex_lock(&SHM->beep_mutex_status);
			SHM->beep_emit_status = 0;
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
	}
}
