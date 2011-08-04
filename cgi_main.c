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

pid_t pid;
key_t key;
int shmid;
int cgi_flag = 0;

int main()
{
	int i,n;
	char data[100];

	printf("Content-Type:text/html;charset=utf-8\n\n");
	printf("<html>");
	printf("<body>");

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

	for (i = 0; i < n; i++)
	{
		data[i] = getc(stdin);

		while (data[i] == '=')
		{
			data[i] = '\0';
		}

		printf("%c", data[i]);
	}

	printf(">>>>>>>>%s\n",data);

	if (!strcmp(data, "LED_ON"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"10;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "LED_OFF"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "PWM_ON"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "PWM_OFF"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

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
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctll.html\"/>");
	}

	if (!strcmp(data, "CAM_OFF"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "ADC_ON"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "ADC_OFF"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "LOOK"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	if (!strcmp(data, "SAVE"))
	{
		printf("write to shm : ");

		pthread_mutex_lock(&SHM->cgi_mutex_start);
		cgi_flag = (0 == SHM->cgi_emit_start);
	
		if (0 == SHM->cgi_emit_start)
		{
			SHM->cgi_emit_start++;
		}

		strcpy(SHM->cgi_cmd, data);
		pthread_mutex_unlock(&SHM->cgi_mutex_start);

		if (1 == cgi_flag)
		{
			pthread_cond_signal(&SHM->cgi_cond_start);
		}

		printf ("<meta http-equiv=\"refresh\" content=\"0;URL=../ioctl.html\"/>");
	}

	printf("</body>");
	printf("</html>");

	return 0;
}
