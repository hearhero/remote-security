#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>

#define MAXCMDLEN 256
#define MAXSHMSIZE 8192

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

	pthread_mutex_t led_mutex_status;

	pthread_mutex_t cgi_mutex_start;
	pthread_cond_t cgi_cond_start;

	pthread_mutex_t status_mutex_temperature;

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

	int status_temperature;
}*SHM;

key_t key;
int shmid;

int main()
{
	int i, n;
	int cgi_flag;
	char data[100];

	printf("Content-Type:text/html;charset=utf-8\n\n");

	key = 0x34567890;

	if (-1 == (shmid = shmget(key, MAXSHMSIZE, IPC_CREAT | IPC_EXCL | 0666)))
	{
		if (EEXIST == errno)
		{
			shmid = shmget(key, MAXSHMSIZE, 0666); 
			SHM = shmat(shmid, NULL, 0);
		}
		else
		{
			perror("fail to shmget");
			exit(-1);
		}
	}
	else
	{
		SHM = shmat(shmid, NULL, 0);
	}

	if (getenv("CONTENT_LENGTH"))
	{
		n = atoi(getenv("CONTENT_LENGTH"));
	}

	for (i = 0; i < n; i++)
	{
		data[i] = getc(stdin);

		if (data[i] == '=')
		{
			data[i] = '\0';
		}
	}

	if (!strcmp(data, "LED_ON"))
	{
		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);

		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		memset(SHM->cgi_cmd, 0, sizeof(SHM->cgi_cmd));
		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "LED_OFF"))
	{
		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);

		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		memset(SHM->cgi_cmd, 0, sizeof(SHM->cgi_cmd));
		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "BEEP_ON"))
	{
		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);

		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		memset(SHM->cgi_cmd, 0, sizeof(SHM->cgi_cmd));
		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "BEEP_OFF"))
	{
		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);

		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		memset(SHM->cgi_cmd, 0, sizeof(SHM->cgi_cmd));
		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "CAM_ON"))
	{
#if 0
		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);

		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		memset(SHM->cgi_cmd, 0, sizeof(SHM->cgi_cmd));
		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}
#endif
		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctll.html\"/>");
	}

	if (!strcmp(data, "CAM_OFF"))
	{
#if 0
		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);

		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		memset(SHM->cgi_cmd, 0, sizeof(SHM->cgi_cmd));
		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}
#endif
		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	return 0;
}
